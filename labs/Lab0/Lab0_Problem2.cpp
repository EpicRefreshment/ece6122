/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 08/20/2025
Date Last Modified: 08/20/2025

Description:

Lab 0 Problem 2

This file finds the longest sum of consecutive prime numbers that results
in a prime number below a user-defined threshold.
This is based on Project Euler Problem 50.
The program will output the longest sum and the resulting prime number.
The threshold is provided via user input and is expected to be a positive integer
less than 2^32. An input of 0 will terminate the program.
Invalid input will result in an error message and a prompt for new input.

*/
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
using namespace std;

bool isPrime(int number, const vector<int>& primes)
{
    return true;
}

vector<int> sieveOfEratosthenes(int limit)
{
    vector<int> primes;
    return primes;
}

/*
Naive implementation of finding all primes below a threshold.
// Returns a vector of all primes below the threshold.
*/
vector<int> findPrimesKnuth(int threshold)
{
    // Initialize primes vector.
    vector<int> primes;

    cout << "Finding primes below " << threshold << "." << endl;

    // P1
    primes.insert(primes.end(), {2, 3}); // This should always start with 2 and 3
    for (int num = 5; num < threshold; num += 2) // skip even numbers.
    {
        bool prime = true;
    }
    // P3

    return primes;
}
/*
Naive implementation of finding all primes below a threshold.
// Returns a vector of all primes below the threshold.
*/
vector<int> findPrimes(int threshold)
{
    // Initialize primes vector.
    vector<int> primes;

    cout << "Finding primes below " << threshold << "." << endl;

    primes.insert(primes.end(), 2); // This should always start with 2.
    for (int num = 3; num < threshold; num += 2) // skip even numbers.
    {
        bool prime = true; // flag for prime status.
        for (int div = 2; div * div <= num; div++) // check divisibility up to sqrt(num).
        {
            if (num % div == 0)
            {
                prime = false; // num is divisible by div, so it is not prime.
                break; // no need to check further.
            }
        }
        if (prime)
        {
            primes.insert(primes.end(), num); // add prime to list.
        }
    }
    return primes;
}

bool validateInput(string userInput)
{
    // Check if input is an integer.
    int tempInt; // local variable for testing cast to integer.
    stringstream userInputStream = stringstream(userInput);
    userInputStream >> tempInt;

    // Check if any fail bits are set and if the eof bit is not set.
    // These are indicators that the input was not a valid integer.
    if (userInputStream.eof() && !userInputStream.fail())
    {
        // if input is a valid integer, check if it is positive
        // and less than 2^32
        if (tempInt < 0 || tempInt > 4294967295) // 2^32 = 4,294,967,296
        {
            return false; // Input is not positive or too large.
        }
        else
        {
            return true; // Input is a valid integer.
        }
    }
    else 
    {
        return false; // Input is not an integer.
    }
}

int main()
{
    // Instantiate variables.
    int threshold; // obtained from userInput after validation.
    vector<int> primes; // Vector of longest consecutive primes and their sum.
    bool valid; // Flag for valid input.

    // Begin main loop that asks for user input until user enters 0.
    do {
        // initialize this string to reset any error flags.
        string userInput; // User input as string for validation.

        // Prompt user for input.
        cout << "Please enter a natural number (0 to quit): ";
        getline(cin, userInput);

        // Check for valid input.
        valid = validateInput(userInput);
        if (!valid) 
        {
            cout << "Error! Invalid input!" << endl;
            threshold = -1; // set threshold so loop continues.
        }   
        else 
        {
            stringstream(userInput) >> threshold;

            // Naive method
            auto start = chrono::high_resolution_clock::now();
            primes = findPrimes(threshold);
            auto end = chrono::high_resolution_clock::now();

            double duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            duration *= 1e-9; // convert to seconds.

            cout << "Time taken to find primes with naive method: " << fixed << duration 
                 << " seconds." << endl;

            cout << "Threshold: " << threshold << endl;
            for (int prime : primes)
            {
                cout << prime << " ";
            }
            cout << endl;

            // Donald Knuth method
            start = chrono::high_resolution_clock::now();
            primes = findPrimesKnuth(threshold);
            end = chrono::high_resolution_clock::now();

            duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            duration *= 1e-9; // convert to seconds.

            cout << "Time taken to find primes with Donald Knuth method: " << fixed << duration 
                 << " seconds." << endl;

            cout << "Threshold: " << threshold << endl;
            for (int prime : primes)
            {
                cout << prime << " ";
            }
            cout << endl;
        }

    } while (threshold != 0);

    // User terminated program.
    cout << "Program terminated." << endl;
    cout << "Have a nice day!" << endl;
    return 0;
}