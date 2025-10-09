/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 09/28/2025
Date Last Modified: 10/08/2025

Description:

Lab 2

This is the source file for a helper library that parses command line arguments
for the conway's game of life simulation.

*/

#include "Command_Parser.h"

using namespace std;

/*
This function parses the width argument from the command line.
It calls a helper function to validate input then returns an integer
for the width parameter

Arguments:
    value - string taken from command line associated with -x flag

Return Values:
    int - width
*/
int parseWidth(string value)
{
    int width = validateWidth(value);
    if (width < 0)
    {
        return -1;
    }
    else
    {
        return width;
    }
}


/*
This function parses the height argument from the command line.
It calls a helper function to validate input then returns an integer
for the height parameter

Arguments:
    value - string taken from command line associated with -y flag

Return Values:
    int - height
*/
int parseHeight(string value)
{
    int height = validateHeight(value);
    if (height < 0)
    {
        return -1;
    }
    else
    {
        return height;
    }
}

/*
This function parses the cell size argument from the command line.
It calls a helper function to validate input then returns an integer
for the cell size parameter

Arguments:
    value - string taken from command line associated with -c flag

Return Values:
    int - cell size
*/
int parseCellSize(string value, int width, int height)
{
    int cellSize = validateCellSize(value, width, height);
    if (cellSize < 0)
    {
        return -1;
    }
    else
    {
        return cellSize;
    }
}

/*
This function parses the number of threads argument from the command line.
It calls a helper function to validate input then returns an integer
for the number of threads parameter

Arguments:
    value - string taken from command line associated with -n flag

Return Values:
    int - number of threads
*/
int parseNumThreads(string value)
{
    int numThreads = validateNumThreads(value);
    if (numThreads < 0)
    {
        return -1;
    }
    else
    {
        return numThreads;
    }

}

/*
This function parses the process type argument from the command line.
It calls a helper function to validate input then returns an integer
for the process type parameter

Arguments:
    value - string taken from command line associated with -t flag

Return Values:
    int - process type
*/
int parseProcessType(string value)
{
    int valid = validateProcessType(value);
    if (valid < 0)
    {
        return -1;
    }
    else
    {
        int processType;
        if (value == "SEQ")
        {
            processType = static_cast<int>(ProcessType::SEQ);
        }
        else if (value == "THRD")
        {
            processType = static_cast<int>(ProcessType::THRD);
        }
        else if (value == "OMP")
        {
            processType = static_cast<int>(ProcessType::OMP);
        }
        return processType;
    }
}

/*
This function validates the width argument from the command line.

Arguments:
    value - string taken from command line associated with -x flag

Return Values:
    int - 0 if invalid, 1 if valid
*/
int validateWidth(string value)
{
    // Check if input is an integer.
    int tempInt; // local variable for testing cast to integer.
    stringstream valueStream = stringstream(value);
    valueStream >> tempInt;

    // Check if any fail bits are set and if the eof bit is not set.
    // These are indicators that the input was not a valid integer.
    if (valueStream.eof() && !valueStream.fail())
    {
        // if input is a valid integer, check it's valid
        if (tempInt < 1 || tempInt > 3840)
        {
            return -1; // Out of range
        }
        else
        {
            return tempInt; // valid
        }
    }
    else 
    {
        return -1; // valid
    }
}

/*
This function validates the height argument from the command line.

Arguments:
    value - string taken from command line associated with -y flag

Return Values:
    int - 0 if invalid, 1 if valid
*/
int validateHeight(string value)
{
    // Check if input is an integer.
    int tempInt; // local variable for testing cast to integer.
    stringstream valueStream = stringstream(value);
    valueStream >> tempInt;

    // Check if any fail bits are set and if the eof bit is not set.
    // These are indicators that the input was not a valid integer.
    if (valueStream.eof() && !valueStream.fail())
    {
        // if input is a valid integer, check it's valid
        if (tempInt < 1 || tempInt > 2160)
        {
            return -1; // Out of range
        }
        else
        {
            return tempInt; // valid
        }
    }
    else 
    {
        return -1; // valid
    }
}

/*
This function validates the cell size argument from the command line.

Arguments:
    value - string taken from command line associated with -c flag

Return Values:
    int - 0 if invalid, 1 if valid
*/
int validateCellSize(string value, int width, int height)
{
    // Check if input is an integer.
    int tempInt; // local variable for testing cast to integer.
    stringstream valueStream = stringstream(value);
    valueStream >> tempInt;

    int widthLimit = width / 2;
    int heightLimit = height / 2;

    // Check if any fail bits are set and if the eof bit is not set.
    // These are indicators that the input was not a valid integer.
    if (valueStream.eof() && !valueStream.fail())
    {
        // if input is a valid integer, check it's valid
        if (tempInt < 1 || tempInt > widthLimit || tempInt > heightLimit)
        {
            return -1; // Out of range
        }
        else
        {
            return tempInt; // valid
        }
    }
    else 
    {
        return -1; // valid
    }
}

/*
This function validates the number of threads argument from the command line.

Arguments:
    value - string taken from command line associated with -n flag

Return Values:
    int - 0 if invalid, 1 if valid
*/
int validateNumThreads(string value)
{
    // Check if input is an integer.
    int tempInt; // local variable for testing cast to integer.
    stringstream valueStream = stringstream(value);
    valueStream >> tempInt;

    // Check if any fail bits are set and if the eof bit is not set.
    // These are indicators that the input was not a valid integer.
    if (valueStream.eof() && !valueStream.fail())
    {
        // if input is a valid integer, check it's valid
        if (tempInt < 2 || tempInt > 200)
        {
            return -1; // Out of range
        }
        else
        {
            return tempInt; // valid
        }
    }
    else 
    {
        return -1; // valid
    }
}

/*
This function validates the process type argument from the command line.

Arguments:
    value - string taken from command line associated with -t flag

Return Values:
    int - 0 if invalid, 1 if valid
*/
int validateProcessType(string value)
{
    if (value == "SEQ" || value == "THRD" || value == "OMP")
    {
        return 1; // valid
    }
    else 
    {
        return -1; // invalid
    }
}