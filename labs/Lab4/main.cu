/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 10/24/2025
Date Last Modified: 10/24/2025

Description:

Lab 4

This is the main file for the conway's game of life simulation.
This handles simple overview of handling window and parses all optional command
line arguments.

Arguments:
-N: size of grid (N x N)
-I: number of iterations to run simulation
-q: quits application

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

using namespace std;

/*
This function validates a generic integer argument from the command line.

Arguments:
    value - string taken from command line

Return Values:
    int - 0 if invalid, 1 if valid
*/
int validateGridSize(string value)
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
        if (tempInt < 32 || tempInt > 8192)
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
        return -1; // invalid
    }
}

/*
This function validates a generic integer argument from the command line.

Arguments:
    value - string taken from command line

Return Values:
    int - 0 if invalid, 1 if valid
*/
int validateNumIterations(string value)
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
        if (tempInt < 100 || tempInt > 100000000)
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
        return -1; // invalid
    }
}

/*
This function parses the width argument from the command line.
It calls a helper function to validate input then returns an integer
for the width parameter

Arguments:
    value - string taken from command line associated with -x flag

Return Values:
    int - width
*/
int parseGridSize(string value)
{
    int tempIntArg = validateGridSize(value);
    if (tempIntArg < 0)
    {
        return -1;
    }
    else
    {
        return tempIntArg;;
    }
}

/*
This function parses the width argument from the command line.
It calls a helper function to validate input then returns an integer
for the width parameter

Arguments:
    value - string taken from command line associated with -x flag

Return Values:
    int - width
*/
int parseNumIterations(string value)
{
    int tempIntArg = validateNumIterations(value);
    if (tempIntArg < 0)
    {
        return -1;
    }
    else
    {
        return tempIntArg;;
    }
}

/*
This function initializes the temperature table for the insulated plate simulation.

Arguments:
    rowSize - size of one row of the temperature table
    tempTable - pointer to temperature table to initialize

Return Values:
    void
*/
void initInsulatedPlate(int rowSize, double* tempTable)
{
    for (int i = 0; i < rowSize; i++)
    {
        for (int j = 0; j < rowSize; j++)
        {
            // set our 100 degree segment at the top center of the plate
            int hotSegment = (j >= rowSize / 3) &&
                             (j <= 2 * (rowSize / 3)) &&
                             (i == 0);
            int index = i * rowSize + j; // calculate 1D index
            if (hotSegment)
            {
                tempTable[index] = 100.0; // hot segment
            }
            else
            {
                tempTable[index] = 20.0; // ambient temperature
            }
        }
    }
}

/*
This function (CUDA kernel) updates the temperature table for the insulated plate simulation.

Arguments:
    rowSize - size of one row of the temperature table
    tableG - pointer to temperature table to update
    tableH - pointer to temperature table to use during updating

Return Values:
    void
*/
__global__
void updateTemperature(int rowSize, double* tableG, double* tableH)
{
    // index for CUDA threads
    int i = blockIdx.x * blockDim.x + threadIdx.x + 1;
    int j = blockIdx.y * blockDim.y + threadIdx.y + 1;
    
    // update interior points
    if (i < (rowSize - 1) && j < (rowSize - 1))
    {
        int rowIndex = i * rowSize + j;
        tableG[rowIndex] = 0.25 * (tableH[(i - 1) * rowSize + j] +
                                   tableH[(i + 1) * rowSize + j] +
                                   tableH[rowIndex - 1] +
                                   tableH[rowIndex + 1]);
    }
}

/*
This function writes the temperature table to a CSV file.

Arguments:
    rowSize - size of one row of the temperature table
    tempTable - pointer to temperature table to update
    fileName - file name!

Return Values:
    void
*/
void writeDataToFile(int rowSize, double* tempTable, string fileName)
{
    ofstream outFile;
    outFile.open(fileName);

    // write data as a table
    for (int i = 0; i < rowSize; i++)
    {
        for (int j = 0; j < rowSize; j++)
        {
            int index = i * rowSize + j;
            outFile << tempTable[index] << ","; // comma for csv format
        }
        outFile << endl;
    }

    outFile.close();
}

/*
This is the main function. It handles command line arguments, user input
CUDA initialization, runs simulation, and writes data to file until user quits
or invalid input is detected.
*/
int main(int argc, char* argv[])
{
    //----- Get Initial Parameters -----//
    // Set values for first calculation
    int size = 256; // default value
    int numIterations = 10000; // default value
    int quitApplication = 0; // flag to quit application
    int invalidInput = 0; // flag for invalid input

    // Make sure there aren't too many arguments
    if (argc > 6)
    {
        invalidInput = 1;
    }

    vector<int> argRecv = {0, 0, 0}; // to prevent duplicate arguments

    if (invalidInput == 0) // don/t bother checking arguments if wrong amount is present
    {
        for (int i = 1; i < argc; i++)
        {
            string command = argv[i++];
            if (command == "-N") // window width
            {
                if (argRecv[0]) // check for duplicate command
                {
                    invalidInput = 1;
                }

                size = parseGridSize(argv[i]);

                if (size < 0) // validate
                {
                    invalidInput = 1;
                }

                argRecv[0] = 1; // command received. Used to check for duplicate commands.
            }
            else if (command == "-I") // window height
            {
                if (argRecv[1]) // check for duplicate command
                {
                    invalidInput = 1;
                }

                numIterations = parseNumIterations(argv[i]);

                if (numIterations < 0) // validate
                {
                    invalidInput = 1;
                }

                argRecv[1] = 1; // command received. Used to check for duplicate commands.
            }
            else if (command == "-q") // cell size
            {
                quitApplication = 1;
            }
        }
    }

    // this loop runs indefinitely until the user quits or inputs invalid data
    // It will repeatedly ask for new grid sizes and iteration counts
    // and run the simulation again. It will reuse the last valid inputs if not given.
    while (invalidInput == 0 && quitApplication == 0)
    {
        // initialize parameters for simulation
        int rowSize = size + 2;
        int gridSize = rowSize * rowSize;

        double* tableG;
        double* tableH;

        cudaMallocManaged(&tableG, gridSize * sizeof(double));
        cudaMallocManaged(&tableH, gridSize * sizeof(double));

        initInsulatedPlate(rowSize, tableH); // initialize temperature table

        // CUDA kernel launch parameters
        dim3 numThreads(16, 16);
        dim3 numBlocks((rowSize - 2 + numThreads.x - 1) / numThreads.x,
                       (rowSize - 2 + numThreads.y - 1) / numThreads.y);

        // Variables for timing calculations
        cudaEvent_t start, stop;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);

        float milliseconds = 0;

        cudaEventRecord(start); // start timer
        
        // Calculate values for specified number of iterations
        for (int iter = 0; iter < numIterations; iter++)
        {
            updateTemperature<<<numBlocks, numThreads>>>(rowSize, tableG, tableH);

            double* temp = tableG;
            tableG = tableH;
            tableH = temp;
        }

        cudaEventRecord(stop);

        // Wait for all CUDA threads to converge.
        cudaEventSynchronize(stop);

        // output timing results to 
        cudaEventElapsedTime(&milliseconds, start, stop);  // elapsed time in milliseconds
        cout << "A plate with " << (rowSize - 2) << "x" << (rowSize - 2) << " interior points" << endl;
        cout << "and " << numIterations << " iterations took " << milliseconds << "ms." << endl;

        // output data to file
        string filename = "tempTable.csv";
        writeDataToFile(rowSize, tableH, filename);

        // free cuda memory
        cudaFree(tableG);
        cudaFree(tableH);

        // initialize this string to reset any error flags.
        string userInput; // User input as string for validation.
        stringstream inputStream; // Stream to parse user input.
        string word; // word parsed from user input.
        vector<string> words; // vector to hold parsed words.

        // Prompt user for input.
        cout << "Please enter '-q' to quit or specify a new grid size and number of iterations in the cmd line format." << endl;
        cout << "If an argument is not specified, the last value will be used again." << endl;
        cout << "Example: -N <grid_size> -I <num_iterations> [-q]" << endl;

        getline(cin, userInput); // get input

        // Process user input.
        inputStream = stringstream(userInput);

        while (inputStream >> word)
        {
            words.push_back(word);
        }

        // Make sure there aren't too many arguments
        if (words.size() > 6)
        {
            invalidInput = 1;
        }

        vector<int> argRecv = {0, 0, 0}; // to prevent duplicate arguments

        if (invalidInput == 0) // don/t bother checking arguments if wrong amount is present
        {
            for (size_t k = 0; k < words.size(); k++)
            {
                string command = words[k];
                if (command == "-N") // window width
                {
                    if (argRecv[0]) // check for duplicate command
                    {
                        invalidInput = 1;
                        break;
                    }

                    k++;
                    size = parseGridSize(words[k]);

                    if (size < 0) // validate
                    {
                        invalidInput = 1;
                        break;
                    }

                    argRecv[0] = 1; // command received. Used to check for duplicate commands.
                }
                else if (command == "-I") // window height
                {
                    if (argRecv[1]) // check for duplicate command
                    {
                        invalidInput = 1;
                        break;
                    }

                    k++;
                    numIterations = parseNumIterations(words[k]);

                    if (numIterations < 0) // validate
                    {
                        invalidInput = 1;
                        break;
                    }

                    argRecv[1] = 1;  // command received. Used to check for duplicate commands.
                }
                else if (command == "-q")
                {
                    quitApplication = 1;
                    break;
                }
                else
                {
                    invalidInput = 1;
                    break;
                }
            }
        }
    }

    if (quitApplication == 1) // user quit application
    {
        cout << "User quit application. Exiting..." << endl;
    }

    if (invalidInput == 1) // invalid input detected
    {
        cout << "Invalid input!" << endl;
    }

    return 0;
}