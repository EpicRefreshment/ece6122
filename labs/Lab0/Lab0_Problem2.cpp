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
vector<int> findPrimes(int threshold)
{
    vector<int> primes(2);
    cout << "Finding primes below " << threshold << " ." << endl;
    return primes;
}

bool validateInput(string userInput)
{
    // Check if input is an integer.
    int tempInt; // local variable for testing cast to integer.
    if (stringstream(userInput) >> tempInt)
    {
        cout << tempInt << endl;
        // if input is a valid integer, check if it is positive
        // and less than 2^32
        if (tempInt < 0 || tempInt >= 4,294,967,296) // 2^32 = 4,294,967,296
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
            cout << "Threshold: " << threshold << endl;
        }

    } while (threshold != 0);

    // User terminated program.
    cout << "Program terminated." << endl;
    cout << "Have a nice day!" << endl;
    return 0;
}