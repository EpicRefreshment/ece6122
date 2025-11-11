/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/09/2025
Date Last Modified: 11/11/2025

Description:

Lab 5

This is the source file for the ServerTCP object.

*/

#include "ServerTCP.h"

/*
This is the constructor for ServerTCP.
Manages socket server and logs messages and client connections to logfile.

Arguments:
    port -- host port for server

Return Values:
    ServerTCP
*/
ServerTCP::ServerTCP(unsigned short port) 
    : port(port)
{
    logFile.open("server.log", ios::app);
    if (!logFile.is_open())
    {
        cerr << "Error: Could not open server.log for writing." << endl;
    }
}

/*
This is the deconstructor for ServerTCP.
Just closes the logfile

Arguments:
    N/A
Return Values:
    N/A
*/
ServerTCP::~ServerTCP()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

/*
This handles a new connection to the server.
It logs the connection and adds the client to a vector
and a socket selector

Arguments:
    N/A
Return Values:
    N/A
*/
void ServerTCP::handleNewConnection()
{
    auto client = make_unique<TcpSocket>(); // use a pointer to make sure we can use it after
    if (listener.accept(*client) == Socket::Status::Done) // if client connects
    {
        // get hostname
        IpAddress remoteAddress = client->getRemoteAddress();
        unsigned short remotePort = client->getRemotePort();

        // log connection
        cout << "Client connected from: " << remoteAddress.toString() << ":" << remotePort << endl;
        if (logFile.is_open())
        {
            logFile << "Client connected: " << remoteAddress.toString() << ":" << remotePort << endl;
        }

        // add client to vector and selector
        selector.add(*client);
        clients.push_back(move(client));
    }
    else
    {
        cerr << "Error: Could not accept new client connection" << endl;
    }
}

/*
This handles data received from a client
It logs the message to a logfile.
It also handles client disconnects and logs those as well.

Arguments:
    client -- TcpSocket object representing client. Used for host info in log
    clientIndex -- index of client in vector. Used when removing client
Return Values:
    N/A
*/
void ServerTCP::handleClientData(TcpSocket* client, size_t clientIndex)
{
    char buffer[1024];
    size_t received;

    Socket::Status status = client->receive(buffer, sizeof(buffer), received); // get data

    if (status == Socket::Status::Done)
    {
        // log message
        string message(buffer, received); 
        cout << "Received " << received << " bytes from " << client->getRemoteAddress().toString()
             << ":" << client->getRemotePort() << ": " << endl;
        if (logFile.is_open())
        {
            logFile << client->getRemoteAddress().toString() << ":" << client->getRemotePort()
                    << "\"" << message << "\" (" << received << " bytes)" << endl;
        }
    }
    else if (status == Socket::Status::Disconnected || status == Socket::Status::Error) // disconnected
    {
        // log disconnect
        IpAddress remoteAddress = client->getRemoteAddress();
        unsigned short remotePort = client->getRemotePort();

        cout << "Client disconnected: " << remoteAddress.toString() << ":" << remotePort << endl;
        if (logFile.is_open())
        {
            logFile << "Client disconnected: " << remoteAddress.toString() << ":" << remotePort << endl;
        }

        // remove client from vector and selector
        selector.remove(*client);
        clients.erase(clients.begin() + clientIndex);
    }
}

/*
This is the main running loop for the server.
It binds the listener and uses the selector to check for activity
on the sockets. It will call functions to handle connections, disconnections, 
and messages received from clients.

Arguments:
    N/A
Return Values:
    N/A
*/
void ServerTCP::run()
{
    if (listener.listen(port) != Socket::Status::Done) // bind listener
    {
        cerr << "Error: Could not bind listener to port " << port << endl;
        return;
    }

    selector.add(listener); // add listener to selector
    cout << "TCP Server started, listening on port: " << port << endl;

    while (true)
    {
        if (selector.wait()) // wait for socket activity
        {
            if (selector.isReady(listener)) // if listener ready, new client connection
            {
                handleNewConnection();
            }
            else
            {
                for (size_t i = 0; i < clients.size(); i++) // iterate through clients
                {
                    if (selector.isReady(*clients[i])) // if client[i] ready, we found the culprit!
                    {
                        handleClientData(clients[i].get(), i); // handle new message or client disconnect
                    }
                }
            }
        }
    }
}
