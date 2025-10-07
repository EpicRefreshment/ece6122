#include "Command_Parser.h"

using namespace std;

int parseWidth(string value)
{
    int width = validateWidth(value);
    if (width < 0)
    {
        cout << "Invalid input!" << endl;
        return -1;
    }
    else
    {
        return width;
    }
}

int parseHeight(string value)
{
    int height = validateHeight(value);
    if (height < 0)
    {
        cout << "Invalid input!" << endl;
        return -1;
    }
    else
    {
        return height;
    }
}

int parseCellSize(string value, int width, int height)
{
    int cellSize = validateCellSize(value, width, height);
    if (cellSize < 0)
    {
        cout << "Invalid input!" << endl;
        return -1;
    }
    else
    {
        return cellSize;
    }
}

int parseNumThreads(string value)
{
    int numThreads = validateNumThreads(value);
    if (numThreads < 0)
    {
        cout << "Invalid input!" << endl;
        return -1;
    }
    else
    {
        return numThreads;
    }

}

int parseProcessType(string value)
{
    int valid = validateProcessType(value);
    if (valid < 0)
    {
        cout << "Invalid input!" << endl;
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