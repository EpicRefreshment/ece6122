#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cuda_runtime.h>

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

void initInsulatedPlate(int rowSize, double* tempTable)
{
    for (int i = 0; i < rowSize; i++)
    {
        for (int j = 0; j < rowSize; j++)
        {
            int hotSegment = (j >= rowSize / 3) &&
                             (j <= 2 * (rowSize / 3)) &&
                             (i == 0);
            int index = i * rowSize + j;
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

__global__
void updateTemperature(int rowSize, double* tempTableG, double* tempTableH)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;
    for (int i = index + 1; i < (rowSize - 1); i += stride)
    {
        for (int j = 1; j < (rowSize - 1); j++)
        {
            int rowIndex = i * rowSize + j;
            tempTableG[rowIndex] = 0.25 * (tempTableH[(i - 1) * rowSize + j] +
                                           tempTableH[(i + 1) * rowSize + j] +
                                           tempTableH[rowIndex - 1] +
                                           tempTableH[rowIndex + 1]);
        }
    }
}


void updateTemperatureSeq(int rowSize, double* tempTableG, double* tempTableH)
{
    for (int i = 1; i < (rowSize - 1); i++)
    {
        for (int j = 1; j < (rowSize - 1); j++)
        {
            int index = i * rowSize + j;
            tempTableG[index] = 0.25 * (tempTableH[(i - 1) * rowSize + j] +
                                        tempTableH[(i + 1) * rowSize + j] +
                                        tempTableH[index - 1] +
                                        tempTableH[index + 1]);
        }
    }
}

void writeDataToFile(int rowSize, double* tempTable, string fileName)
{
    ofstream outFile;
    outFile.open(fileName);

    for (int i = 0; i < rowSize; i++)
    {
        for (int j = 0; j < rowSize; j++)
        {
            int index = i * rowSize + j;
            outFile << tempTable[index] << ",";
        }
        outFile << endl;
    }

    outFile.close();
}

int main(int argc, char* argv[])
{
    // Set values for first calculation
    int size = 256; // default value
    int numIterations = 10000; // default value
    int quitApplication = 0;
    int invalidInput = 0;

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

    int i = 0;
    while (invalidInput == 0 && quitApplication == 0)
    {
        int rowSize = size + 2;
        int gridSize = rowSize * rowSize;

        double* tempTableG;
        double* tempTableH;

        cudaMallocManaged(&tempTableG, gridSize * sizeof(double));
        cudaMallocManaged(&tempTableH, gridSize * sizeof(double));

        initInsulatedPlate(rowSize, tempTableH);

        int numThreads = 256;
        int numBlocks = (rowSize - 2 + numThreads - 1) / numThreads;

        for (int iter = 0; iter < numIterations; iter++)
        {
            updateTemperature<<<numBlocks, numThreads>>>(rowSize, tempTableG, tempTableH);
            double* temp = tempTableG;
            tempTableG = tempTableH;
            tempTableH = temp;
        }

        cudaDeviceSynchronize();

        string filename = "tempTable" + to_string(i) + ".csv";
        writeDataToFile(rowSize, tempTableH, filename);

        cudaFree(tempTableG);
        cudaFree(tempTableH);

        i++;

        // initialize this string to reset any error flags.
        string userInput; // User input as string for validation.
        stringstream inputStream; // Stream to parse user input.
        string word; // word parsed from user input.
        vector<string> words; // vector to hold parsed words.

        // Prompt user for input.
        cout << "Please enter '-q' to quit or specify a new grid size and number of iterations in the cmd line format." << endl;
        cout << "-N <grid_size> -I <num_iterations> [-q]" << endl;

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
            size = 256;
            numIterations = 10000;
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

                    argRecv[1] = 1; // command received. Used to check for duplicate commands.
                }
                else if (command == "-q") // cell size
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

    if (quitApplication == 1)
    {
        cout << "User quit application. Exiting..." << endl;
    }

    if (invalidInput == 1)
    {
        cout << "Invalid input!" << endl;
    }

    return 0;
}