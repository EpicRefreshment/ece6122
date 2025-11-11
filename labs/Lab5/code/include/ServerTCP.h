/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/09/2025
Date Last Modified: 11/11/2025

Description:

Lab 5

This is the header file for the ServerTCP object.

*/

#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <memory>

using namespace std;
using namespace sf;

class ServerTCP
{
public:
    // --- Public Methods ---
    ServerTCP(unsigned short port); // constructor
    ~ServerTCP(); // deconstructor
    void run(); // basically does everything for the server in "infinite" loop

private:
    // --- Private Methods ---
    void handleNewConnection();
    void handleClientData(TcpSocket* client, size_t clientIndex);

    // --- Private Members ---
    unsigned short port; // host port
    TcpListener listener; // listener for server
    SocketSelector selector; // useful for handling multiple clients
    vector<unique_ptr<TcpSocket>> clients; // vector of TcpClients!
    ofstream logFile; // log file
};
