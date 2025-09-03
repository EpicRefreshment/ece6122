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
#include <algorithm>
#include <chrono>
using namespace std;

/*
This function generates a list of prime numbers below the given threshold
This is a modified version of the Sieve of Eratosthenes algorithm that runs
in O(n) time complexity referenced from the GeeksOnGeeks website. 
Returns a vector of all primes below the threshold.
*/
vector<int> findPrimes(int threshold)
{
    vector<int> primes; // Vector to store prime numbers.
    vector<bool> isPrime(threshold, true); // Boolean vector to track if number is prime. Optimizes space.
    vector<int> spf(threshold, 0); // Smallest prime factor for each number.

    isPrime[0] = isPrime[1] = false; // 0 and 1 are not prime numbers.

    //Iterate through all numbers from 2 to threshold.
    //If the number is marked as prime, add it to the list of primes
    for (int num = 2; num < threshold; num++)
    {
        if (isPrime[num])
        {
            primes.push_back(num); // num is prime, add to list.
            spf[num] = num; // smallest prime factor of a prime is itself.
        }
        // Inner loop mark multiples of each prime number as non-prime.
        // The loop sets the smallest prime factor for each number so that each
        // number is only marked once.
        for (int primeFactor = 0; primeFactor < primes.size() &&
             num * primes[primeFactor] < threshold && primes[primeFactor] <= spf[num];
             primeFactor++)
        {
            isPrime[num * primes[primeFactor]] = false;

            // Add smallest prime factor to SPF
            spf[num * primes[primeFactor]] = primes[primeFactor];
        }
    }

    return primes;
}

vector<int> findPrimeSum(int threshold, vector<int> primes)
{
    vector<int> primeSum; // Vector to store the longest sum of consecutive primes and the resulting prime.
    int sum = 0; // Temporary variable to store sum of consecutive primes.
    int initialSum = 0; // Initial sum of consecutive primes.
    int initialPrime = 0; // Starting index of the consecutive primes to mark start of subset of primes.
    int endPrime = 0; // Ending index of the consecutive primes to mark end of subset of primes.

    while (initialSum < threshold)
    {
        initialSum += primes[endPrime];
        endPrime++;
    }
    
    for (int num = endPrime - 1; num > 0; num--)
    {
        initialSum -= primes[num];
        sum = initialSum;

        initialPrime = 0;
        while (sum < threshold)
        {
            // check if sum is prime
            if (find(primes.begin(), primes.end(), sum) != primes.end())
            {
                // Get iterators for subset of primes that meet the problem criteria
                auto first = primes.begin() + initialPrime;
                auto last = primes.begin() + num;
                // Initialize vector to store the longest sum of consecutive primes and the resulting prime
                // with subset of primes vector.
                primeSum.assign(first, last);
                primeSum.push_back(sum); // Add resulting prime to end of vector.
                return primeSum; // Return the vector.
            }
            sum += primes[num + initialPrime] - primes[initialPrime];
            initialPrime++;
        }
    }
    return primeSum; // Return empty vector if no sum found.
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

void displayResult(vector<int> primeSum)
{
    cout << "The answer is " << primeSum.back();
    primeSum.pop_back(); // Remove the resulting prime from the vector.
    cout << " with " << primeSum.size() << " terms: ";
    for (int prime : primeSum)
    {
        cout << prime << " ";
        if (prime != primeSum.back())
        {
            cout << "+ ";
        }
    }
    cout << endl;
}

int main()
{
    // Instantiate variables.
    int threshold; // obtained from userInput after validation.
    vector<int> primes; // Vector to store prime numbers.
    vector<int> primeSum; // Vector to store the longest sum of consecutive primes and the resulting prime.
    bool valid; // Flag for valid input.
    double duration; // Time in seconds
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();

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

            // Sieve method
            start = chrono::high_resolution_clock::now();
            primes = findPrimes(threshold);
            end = chrono::high_resolution_clock::now();

            duration = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            duration *= 1e-9; // convert to seconds.

            cout << "Time taken to find primes with sieve method: " << fixed << duration 
                 << " seconds." << endl;

            cout << "Threshold: " << threshold << endl;
            primeSum = findPrimeSum(threshold, primes);
            if (!primeSum.empty())
            {
                displayResult(primeSum);
            }
            else
            {
                cout << "No sum of consecutive primes found that results in a prime below the threshold." << endl;
            }
            cout << endl;
        }

    } while (threshold != 0);

    // User terminated program.
    cout << "Program terminated." << endl;
    cout << "Have a nice day!" << endl;
    return 0;
}