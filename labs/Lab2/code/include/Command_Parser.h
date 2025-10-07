#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

enum class ProcessType
{
	SEQ = 1,
	THRD = 2,
	OMP = 3
};

int parseWidth(string value);
int parseHeight(string value);
int parseCellSize(string value, int width, int height);
int parseNumThreads(string value);
int parseProcessType(string value);

int validateWidth(string value);
int validateHeight(string value);
int validateCellSize(string value, int width, int height);
int validateNumThreads(string value);
int validateProcessType(string value);