#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
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
    return std::exp(-x * x);
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

        if (problem_choice != 1 && problem_choice != 2)
        {
            cerr << "Error: -P must be 1 or 2." << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        if (total_samples <= 0)
        {
            cerr << "Error: -N must be a positive number." << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // --- Distribute parameters using collective communication ---
    MPI_Bcast(&problem_choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&total_samples, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

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
    std::srand(static_cast<unsigned int>(std::time(nullptr)) + rank);

    long long count_inside = 0;
    double (*func)(double);

    if (problem_choice == 1)
    {
        func = func1;
    }
    else
    {
        func = func2;
    }

    // The bounding box for both integrals on [0,1] is a 1x1 square.
    double x_min = 0.0, x_max = 1.0;
    double y_min = 0.0, y_max = 1.0;

    for (long long i = 0; i < local_samples; i++)
    {
        double x = x_min + static_cast<double>(rand()) / RAND_MAX * (x_max - x_min);
        double y = y_min + static_cast<double>(rand()) / RAND_MAX * (y_max - y_min);

        if (y <= func(x))
        {
            count_inside++;
        }
    }

    // --- Gather results ---
    long long total_inside = 0;
    MPI_Reduce(&count_inside, &total_inside, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // --- Final Calculation and Output (on root process) ---
    if (rank == 0)
    {
        double bounding_box_area = (x_max - x_min) * (y_max - y_min);
        double estimate = (static_cast<double>(total_inside) / total_samples) * bounding_box_area;

        cout.precision(8);
        cout << "The estimate for integral " << problem_choice << " is " << fixed << estimate << endl;
        cout << "Bye!" << endl;
    }

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}
