/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/09/2025
Date Last Modified: 11/11/2025

Description:

Lab 5

This is the source file for the ClientTCP object.

*/

#include "ClientTCP.h"

ClientTCP::ClientTCP() = default;  // empty constructor, no need to initialize

/*
This connects to the client to a server at the specified host.

Arguments:
    address -- server address
    port -- server port
Return Values:
    N/A
*/
bool ClientTCP::connect(const IpAddress& address, unsigned short port)
{
    cout << "TCP Client starting..." << endl;
    cout << "Connecting to: " << address.toString() << ":" << port << endl;

    Socket::Status status = socket.connect(address, port, seconds(5)); // connect
    if (status != Socket::Status::Done) // connection failed
    {
        return false;
    }

    cout << "Connected to server at " << address.toString() << " on " << port << "." << endl;
    return true;
}

/*
This sends a message to the server

Arguments:
    message -- message data to send
Return Values:
    N/A
*/
bool ClientTCP::send(const string& message)
{
    if (socket.send(message.c_str(), message.length()) != Socket::Status::Done)
    {
        cerr << "Failed to send message." << endl;
        return false;
    }
    return true;
}

/*
This disconnects client from server

Arguments:
    N/A
Return Values:
    N/A
*/
void ClientTCP::disconnect()
{
    socket.disconnect();
    cout << "Disconnected from server." << endl;
}
