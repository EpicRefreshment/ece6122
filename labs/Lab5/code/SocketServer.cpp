/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/09/2025
Date Last Modified: 11/11/2025

Description:

Lab 5

SocketServer.cpp implements a simple TCP server that logs client
connection statuses and messages from clients. It takes in one argument
for the port the server will listen on. It parses and validates the command
line arguments and creates a ServerTCP object that manages the socket connection
entirely.

Arguments:
- integer value between 61000 and 65535 that represents host port

*/

#include <iostream>
#include <string>

#include "ServerTCP.h"

using namespace std;

int main(int argc, char* argv[])
{
    // --- Validate Arguments ---
    if (argc != 2) // invalid number of arguments
    {
        cerr << "Invalid command line argument detected: Port <None> (incorrect number of arguments)" << endl;
        cerr << "Please check your values and press Enter to end the program!" << endl;
        
        // wait for user input before quitting.
        string message;
        getline(cin, message);
        return 1;
    }

    unsigned short port;
    try // try to take in argument as int
    {
        int tempPort = stoi(argv[1]);
        if (tempPort < 61000 || tempPort > 65535) // port number outside valid range
        {
            cerr << "Invalid command line argument detected: Port " << tempPort << " (port number out of range)" << endl;
            cerr << "Please check your values and press Enter to end the program!" << endl;
            
            // wait for user input before quitting.
            string message;
            getline(cin, message);
            return 1;
        }
        port = static_cast<unsigned short>(tempPort); // SFML Socket expects unsigned short
    }
    catch (const exception& e) // argument wasn't a valid int
    {
        cerr << "Invalid command line argument detected: Port " << argv[1] << " (invalid port number)"
                  << e.what() << endl;
        cerr << "Please check your values and press Enter to end the program!" << endl;
        
        // wait for user input before quitting.
        string message;
        getline(cin, message);
        return 1;
    }

    // Create ServerTCP object and tell it to run.
    ServerTCP server(port);
    server.run();

    return 0;
}
