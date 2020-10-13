/*
File: LaunchTimes_main.cpp
Author: Ryan McNamee
Date Created: Tuesday, 19, September, 2017
Date Updated: Monday, 9, October, 2017
Purpose: Start the program
*/
#include "rm_Launcher.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

using namespace std;

int main(int argc, char* argv[]) {
	// Print program info
	cout << endl << "launchtimes.exe, by Ryan McNamee, 2017" << endl << endl;

	// Only allow 2 arguments
	if (argc != 2) {
		// Complain
		cerr << "Error: Invalid parameter list" << endl
			<< "Usage: launchtime.exe [text file]" << endl;
		// Abandon
		return EXIT_FAILURE;
	}

	// Create File
	wifstream file(argv[1]);
	// Check if file could be opened
	if (!file) {
		// Complain
		cerr << "Error: File could not be opened" << endl;
		// Abandon
		return EXIT_FAILURE;
	}

	cout << "Parsing File: " << endl;
	// Create launcher and parse file
	rm::Launcher launcher;
	try {
		file >> launcher;
	}
	catch (exception e) {		// No exception should be thrown but code may change later
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}
	// File no longer needed
	file.close();

	// Run processes and output data
	cout << endl << "Running Applications: " << endl;
	launcher.RunAll();
	cout << endl << "Printing Data: " << endl;
	wcout << launcher;

	cout << endl << "Progrma End." << endl;
	// Everything worked
	return EXIT_SUCCESS;
}