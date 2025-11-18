/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 11/12/2025
Date Last Modified: 11/17/2025

Description:

Lab 6

Implements simple Monte Carlo integration using MPI for parallelization. It estimates the value of two integrals:
1. Integral of f(x) = x^2 over [0, 1]
2. Integral of f(x) = e^(-x^2) over [0, 1]

Arguments:
-P <1|2> : Selects which integral to estimate
-N <number_of_samples> : Total number of random samples to use for the estimation

*/

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <chrono>
#include <mpi.h>

using namespace std;

/*
Function for integral 1: f(x) = x^2

Arguments:
    x: input as double
Return Values:
    double: output f(x)
*/
double func1(double x)
{
    return x * x;
}

/*
Function for integral 1: f(x) = e^(-x^2)

Arguments:
    x: input as double
Return Values:
    double: output f(x)
*/
double func2(double x)
{
    return exp(-x * x);
}

int main(int argc, char** argv)
{
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int functionSelect = 0; // 1 or 2 selecting the integral function for estimation
    long long totalSamples = 0; // 64-bit int just for fun.

    // --- Argument Parsing (on root process) ---
    if (rank == 0)
    {
        if (argc != 5) // invalid number of arguments
        {
            cerr << "Usage: " << argv[0] << " -P <1|2> -N <number_of_samples>" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (int i = 1; i < argc; i++) // iterate through arguments
        {
            string arg = argv[i];
            if (arg == "-P" && i + 1 < argc) // function select
            {
                functionSelect = stoi(argv[++i]);
            }
            else if (arg == "-N" && i + 1 < argc) // number of samples
            {
                totalSamples = stoll(argv[++i]);
            }
        }
    }

    // --- Distribute parameters using collective communication ---
    MPI_Bcast(&functionSelect, 1, MPI_INT, 0, MPI_COMM_WORLD); // broadcast function selection
    MPI_Bcast(&totalSamples, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD); // broadcast total samples

    // --- Validate parameters ---
    if (functionSelect != 1 && functionSelect != 2) // invalid function selection
    {
        if (rank == 0)
        {
            cerr << "Error: -P must be 1 or 2." << endl;
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (totalSamples <= 0) // invalid number of samples
    {
        if (rank == 0)
        {
            cerr << "Error: -N must be a positive number." << endl;
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // --- Monte Carlo Integration ---

    // long long to support large sample sizes
    // Determine number of samples for this process
    long long samplesPerProcess = totalSamples / size;
    long long sampleRemainder = totalSamples % size;

    long long localSamples = samplesPerProcess;
    // Spread out remaining samples
    if (rank < sampleRemainder)
    {
        localSamples++;
    }

    // Seed the random number generator differently for each process
    unsigned int seed = chrono::high_resolution_clock::now().time_since_epoch().count() + rank; // Unique seed per process
    mt19937 generator(seed); // Mersenne Twister RNG
    uniform_real_distribution<double> distribution(0.0, 1.0); // Uniform distribution in [0, 1]

    // Initialize variables
    double localSum = 0.0; // Local sum for this process
    double (*func)(double); // function pointer

    if (functionSelect == 1)
    {
        func = func1; // f(x) = x^2
    }
    else
    {
        func = func2; // f(x) = e^(-x^2)
    }

    // long long in case of super large sample sizes
    for (long long i = 0; i < localSamples; i++)
    {
        // Generate random sample in [0, 1]
        double x = distribution(generator);
        // Add f(x) to local sum
        localSum += func(x);
    }

    // --- Gather results ---
    double totalSum = 0.0;
    MPI_Reduce(&localSum, &totalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); // Reduce local sums to total sum on root process

    // --- Final Calculation and Output (on root process) ---
    if (rank == 0)
    {
        // The integral is the average value of the function times the interval width
        double estimate = totalSum / totalSamples;

        cout.precision(8); // set precision for output
        cout << "The estimate for integral " << functionSelect << " is " << fixed << estimate << endl;
        cout << "Bye!" << endl;
    }

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}
