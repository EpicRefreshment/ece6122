/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/09/2025
Date Last Modified: 11/11/2025

Description:

Lab 5

This is the header file for the ClientTCP object.

*/

#pragma once

#include <iostream>
#include <string>

#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

class ClientTCP
{
public:
    ClientTCP(); // constructor
    bool connect(const IpAddress& address, unsigned short port); // connect to server
    bool send(const string& message); // send data to server
    void disconnect(); // disconnect from server

private:
    TcpSocket socket; // tcp socket
};
