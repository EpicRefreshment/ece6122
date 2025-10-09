/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/28/2025
Date Last Modified: 10/08/2025

Description:

Lab 2

This is the header file for a helper library that parses command line arguments
for the conway's game of life simulation.

*/

#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// enumeration for process type used
enum class ProcessType
{
	SEQ = 1,
	THRD = 2,
	OMP = 3
};

// parse command line arguments from given value
int parseWidth(string value);
int parseHeight(string value);
int parseCellSize(string value, int width, int height);
int parseNumThreads(string value);
int parseProcessType(string value);

// validate given value for each argument
int validateWidth(string value);
int validateHeight(string value);
int validateCellSize(string value, int width, int height);
int validateNumThreads(string value);
int validateProcessType(string value);