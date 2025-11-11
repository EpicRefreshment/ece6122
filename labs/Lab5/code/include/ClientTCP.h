#include <SFML/Network.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include "PacketData.h"

int main() {
    // Create a TCP socket
    sf::TcpSocket socket;
    
    // Connect to server
    sf::IpAddress serverAddress(127,0,0,1);  // Localhost
    unsigned short port = 54000;
    
    std::cout << "TCP Client starting..." << std::endl;
    std::cout << "Connecting to: " << serverAddress << ":" << port << std::endl;
    
    sf::Socket::Status status = socket.connect(serverAddress, port, sf::seconds(5));
    
    if (status != sf::Socket::Status::Done) {
        std::cerr << "Error: Could not connect to server" << std::endl;
        if (status == sf::Socket::Status::Error)
        {
            std::cerr << "Socket error occurred" << std::endl;
        }
        else if (status == sf::Socket::Status::Disconnected)
        {
            std::cerr << "Connection was disconnected" << std::endl;
        }
        return -1;
    }
    
    std::cout << "Connected to server!" << std::endl;
    std::cout << "Local endpoint: " << socket.getLocalPort() << std::endl;
    std::cout << "Starting communication...\n" << std::endl;
    
    // Set non-blocking mode for receiving
    socket.setBlocking(false);
    
    uint32_t sequenceNum = 0;
    uint32_t packetsSent = 0;
    uint32_t packetsReceived = 0;
    
    while (true) {
        // Send data to server every 500ms
        static auto lastSendTime = std::chrono::steady_clock::now();
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - lastSendTime).count();
        
        if (elapsed >= 500) {
            // Create and populate the custom data structure
            PacketData data;
            data.sequenceNumber = sequenceNum++;
            
            // Simulate some moving object (circular motion)
            float t = sequenceNum * 0.1f;
            data.position[0] = 10.0f * std::cos(t);
            data.position[1] = 10.0f * std::sin(t);
            data.position[2] = t * 0.5f;
            
            data.velocity[0] = -10.0f * std::sin(t) * 0.1f;
            data.velocity[1] = 10.0f * std::cos(t) * 0.1f;
            data.velocity[2] = 0.5f;
            
            data.status = (sequenceNum % 2 == 0) ? 1 : 0;
            
            char msg[64];
            snprintf(msg, sizeof(msg), "Client packet #%u", sequenceNum);
            data.setMessage(msg);
            
            // Get timestamp
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            data.timestamp = std::chrono::duration<double>(duration).count();
            
            // Send the data structure
            sf::Socket::Status sendStatus = socket.send(&data, sizeof(data));
            
            if (sendStatus != sf::Socket::Status::Done)
            {
                if (sendStatus == sf::Socket::Status::Disconnected)
                {
                    std::cerr << "Server disconnected" << std::endl;
                    break;
                }
                else if (sendStatus == sf::Socket::Status::Error)
                {
                    std::cerr << "Error sending packet " << sequenceNum << std::endl;
                }
            } else {
                packetsSent++;
                std::cout << "Sent packet #" << sequenceNum << std::endl;
            }
            
            lastSendTime = currentTime;
        }
        
        // Try to receive data from server
        PacketData receivedData;
        std::size_t received;
        sf::Socket::Status receiveStatus = socket.receive(&receivedData, sizeof(receivedData), received);
        
        if (receiveStatus == sf::Socket::Status::Done)
        {
            packetsReceived++;
            
            std::cout << "\n=== Packet Received from Server ===" << std::endl;
            std::cout << "Size: " << received << " bytes" << std::endl;
            receivedData.print();
            
            // Calculate latency
            auto now = std::chrono::system_clock::now();
            auto duration = now.time_since_epoch();
            double currentTime = std::chrono::duration<double>(duration).count();
            double latency = (currentTime - receivedData.timestamp) * 1000.0;
            
            std::cout << "Latency: " << latency << " ms" << std::endl;
            std::cout << "Total packets received: " << packetsReceived << "\n" << std::endl;
            
        }
        else if (receiveStatus == sf::Socket::Status::Disconnected)
        {
            std::cout << "\nServer disconnected" << std::endl;
            break;
        }
        else if (receiveStatus == sf::Socket::Status::Error)
        {
            std::cerr << "Error receiving data" << std::endl;
            break;
        }
        // NotReady is normal for non-blocking socket
        
        // Small sleep to prevent busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    socket.disconnect();
    
    std::cout << "\nSession summary:" << std::endl;
    std::cout << "Packets sent: " << packetsSent << std::endl;
    std::cout << "Packets received: " << packetsReceived << std::endl;
    
    return 0;
}
