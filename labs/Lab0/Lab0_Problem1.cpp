/*
Author: Jonathan Wolford
Class: ECE6122Q
Date Created: 08/20/2025
Date Last Modified: 08/29/2025

Description:

Lab 0 Problem 1

This file is a simple C++ program that outputs text to the console that solves Problem 1 of Lab 0 in ECE6122.
This program acts as an example of using insertion stream operators and escape characters
to output formatted text.

*/

#include <iostream>
using namespace std;

/*
Program entrypoint. Simple function that uses insertion stream operators
to display the text required by the assignment.
*/
int main()
{
    // Output via insertion stream operators.
    cout << "My name is: Jonathan Wolford" << endl;
    cout << "This (\") is a double quote." << endl;
    cout << "This (\') is a single quote." << endl;
    cout << "This (\\) is a backslash." << endl;
    cout << "This (/) is a forward slash." << endl;

    // Operation successful. Exit program
    return 0;
}