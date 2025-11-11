/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/09/2025
Date Last Modified: 11/11/2025

Description:

Lab 5

SocketClient.cpp implements a simple TCP client that prompts the user for 
messages to send to a server. It takes in two arguments for the IP address and
the port the server will listen on. It parses and validates the command
line arguments and creates a ClientTCP object that manages the socket connection
entirely.

Arguments:
- IP address, i.e. 0.0.0.0 or localhost
- integer value between 61000 and 65535 that represents host port

*/

#include <iostream>
#include <string>

#include "ClientTCP.h"

using namespace std;
using namespace sf;

int main(int argc, char* argv[])
{
    if (argc != 3) // invalid number of arguments
    {
        cerr << "Invalid command line argument detected: Host <None>:<None> (incorrect number of arguments)" << endl;
        cerr << "Please check your values and press any key to end the program!" << endl;
        return 1;
    }

    IpAddress serverIp(argv[1]);
    if (serverIp == IpAddress::None) // invalid IP address
    {
        cerr << "Invalid command line argument detected: Host " << argv[1] << ":"
                  << argv[2] << " (invalid IP address)" << endl;
        cerr << "Please check your values and press any key to end the program!" << endl;
        return 1;
    }

    unsigned short port;
    try
    {
        int tempPort = stoi(argv[2]);
        if (tempPort < 61000 || tempPort > 65535) // invalid port number
        {
            cerr << "Invalid command line argument detected: Host " << argv[1] << ":"
                      << tempPort << " (port number out of range)" << endl;
            cerr << "Please check your values and press any key to end the program!" << endl;
            return 1;
        }
        port = static_cast<unsigned short>(tempPort); // SFML Socket expects unsigned short
    }
    catch (const exception& e) // invalid argument or invalid integer
    {
        cerr << "Invalid command line argument detected: Host " << argv[1] << ":" << argv[2]
                  << " (invalid port number: " << e.what() << endl;
        cerr << "Please check your values and press any key to end the program!" << endl;
        return 1;
    }

    int reconnectAttempts = 0; // count reconnect attempts
    int reconnectThreshold = 10; // try to reconnect to 10 times.

    // Create ClientTCP and connect to server
    ClientTCP client;
    while (!client.connect(serverIp, port))
    {   
        // Try to connect a few times.
        cout << "Unable to connect to server.";
        if (reconnectAttempts < reconnectThreshold)
        {
            cout << "Retrying." << endl;
            reconnectAttempts++;
        }
        else // give up
        {
            cout << "Closing program. Too many failed attempts." << endl;
            return 1;
        }
    }
    cout << "Connected to server." << endl;

    reconnectAttempts = 0;
    string message; // take in input as string
    while (true) // prompt user for messages until error occurs or user inputs 'q'
    {
        cout << "Please enter a message: ";
        getline(cin, message);
        if (message == "q") // quit if they put in a singular q
        {
            break;
        }
        if (!client.send(message))
        {
            // Some problem occurred here. Just assume. More comprehensive error checking out of scope
            cout << "Error occurred. Attempting to reconnect...";
            while (!client.connect(serverIp, port))
            {
                cout << "Unable to connect to server." << endl;
                if (reconnectAttempts < reconnectThreshold) // try a few times
                {
                    cout << "Retrying..." << endl;
                    reconnectAttempts++;
                }
                else // give up
                {
                    cout << "Closing program. Too many failed attempts." << endl;
                    return 1;
                }
            }
        }
    }

    client.disconnect(); // disconnect and quit.
    return 0;
}
