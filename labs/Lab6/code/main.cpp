#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <cstdlib>
#include <chrono>
#include <mpi.h>

using namespace std;

// Function for integral 1: f(x) = x^2
double func1(double x)
{
    return x * x;
}

// Function for integral 2: f(x) = e^(-x^2)
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

    int problem_choice = 0;
    long long total_samples = 0;

    // --- Argument Parsing (on root process) ---
    if (rank == 0)
    {
        if (argc != 5)
        {
            cerr << "Usage: " << argv[0] << " -P <1|2> -N <number_of_samples>" << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (int i = 1; i < argc; i++)
        {
            string arg = argv[i];
            if (arg == "-P" && i + 1 < argc)
            {
                problem_choice = stoi(argv[++i]);
            }
            else if (arg == "-N" && i + 1 < argc)
            {
                total_samples = stoll(argv[++i]);
            }
        }
    }

    // --- Distribute parameters using collective communication ---
    MPI_Bcast(&problem_choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&total_samples, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    if (problem_choice != 1 && problem_choice != 2)
    {
        if (rank == 0)
        {
            cerr << "Error: -P must be 1 or 2." << endl;
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (total_samples <= 0)
    {
        if (rank == 0)
        {
            cerr << "Error: -N must be a positive number." << endl;
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // --- Monte Carlo Integration ---

    // Determine number of samples for this process
    long long samples_per_proc = total_samples / size;
    long long remainder_samples = total_samples % size;

    long long local_samples = samples_per_proc;
    if (rank < remainder_samples)
    {
        local_samples++;
    }

    // Seed the random number generator differently for each process
    unsigned int seed = chrono::high_resolution_clock::now().time_since_epoch().count() + rank;
    mt19937 generator(seed);
    uniform_real_distribution<double> distribution(0.0, 1.0);

    // Initialize variables
    double local_sum = 0.0;
    double (*func)(double);

    if (problem_choice == 1)
    {
        func = func1;
    }
    else
    {
        func = func2;
    }

    for (long long i = 0; i < local_samples; i++)
    {
        double x = distribution(generator);
        // Add the function's value at that x to our local sum
        local_sum += func(x);
    }

    // --- Gather results ---
    double total_sum = 0.0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // --- Final Calculation and Output (on root process) ---
    if (rank == 0)
    {
        // The integral is the average value of the function times the interval width
        double estimate = total_sum / total_samples;

        cout.precision(8);
        cout << "The estimate for integral " << problem_choice << " is " << fixed << estimate << endl;
        cout << "Bye!" << endl;
    }

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}
