/*
File: rm_Launcher.cpp
Author: Ryan McNamee
Date Created: Friday, 29, September, 2017
Date Updated: Monday, 9, October, 2017
Purpose: Parse input file, generate sorted launchgroups and processes
		Output basic information about run
*/
#include "rm_Launcher.hpp"
#include "rm_lib.hpp"
#include <iostream>
#include <iomanip>
#include <string>

namespace rm {
/// Begin Launcher::Constructors
	Launcher::Launcher() {}

	Launcher::~Launcher() {}
/// End Launcher::Constructors

/// Begin Launcher::Operations
	// Method: rm::Launcher::Add
	// Purpose: Handles the sorting of processes into launchgroups
	void rm::Launcher::Add(value_type::pointer_type const& proc) {
		lgroups[proc->GetLaunchGroup()].Add(proc);
	}

	// Method: rm::Launcher::RunAll
	// Purpose: Calls launchgroups to launch processes
	void rm::Launcher::RunAll() {
		for (container_type::value_type& lg : lgroups) {
			lg.second.RunAllAsync();
		}
	}

	// Method: rm::Launcher::PrintData
	// Purpose: Calls all launchgroups to print process data
	// Reason: Did not want to give access to data structure
	void rm::Launcher::PrintData(std::wostream& outstream) {
		for (container_type::value_type& lg : lgroups) {
			outstream << lg.second;
		}
		outstream << std::endl;
		for (container_type::value_type& lg : lgroups) {
			lg.second.PrintErrors(outstream);
		}
	}
/// End Launcher::Operations

/// Begin Launcher::Operators
	// Method: operator>>
	// Input: input file, Launcher to parse into
	// Output: input file
	// Purpose: Parses through input file for launchgroup and application information
	//			generates process object, sorting of processes is handled elsewhere
	std::wifstream& rm::operator>>(std::wifstream& lhs, rm::Launcher& rhs) {
		size_t currLine = 0;
		while (!lhs.eof()) {
			++currLine;

			std::wstring launchgroup;
			std::wstring app;
			std::wstring params;

			// Parse Line
			// Skip line if there is only a launchgroup
			if (!GetLine(lhs, launchgroup, L',')) {
				std::cerr << "Error at line: " << currLine << ". No application found." << std::endl;
				continue;
			}
			// Check if parameters exist
			if (GetLine(lhs, app, L',')) {
				GetLine(lhs, params);
			}

			// Skip line if there is no launchgroup
			if (launchgroup.size() == 0) {
				std::cerr << "Error at line: " << currLine << ". No launchgroup found." << std::endl;
				continue;
			}

			// Skip line if launchgroup is not a number
			rm::Process::size_type launchnumber;
			try {
				launchnumber = std::stoul(launchgroup);
			}
			catch (std::invalid_argument e) {
				std::cerr << "Error at line: " << currLine << ". Non-integer value in launchgroup." << std::endl;
				continue;
			}

			// trim start of app string
			// all other invalid symbols are expected to be handled by user
			while (app[0] == ' ') {
				app.erase(0, 1);
			}

			// Launcher handles process order
			rhs.Add(new rm::Process(launchnumber, app, params));
		}

		return lhs;
	}

	// Method: operator>>
	// Input: output stream, Launcher to parse from
	// Output: output stream
	// Purpose: Prints Table Headers and error data
	std::wostream& rm::operator<<(std::wostream& lhs, rm::Launcher& rhs) {
		// Print table headers
		lhs << std::setw(3) << std::right
			<< L"LG" << L" |"
			<< std::setw(12) << std::right
			<< L"KernalTime" << L" |"
			<< std::setw(12) << std::right
			<< L"UserTime" << L" |"
			<< std::setw(5) << std::right
			<< L"Exit" << L" |"
			<< std::left
			<< L" Application" << std::endl;

		// Call launchgroups to print process info
		rhs.PrintData(lhs);

		// Return ostream
		return lhs;
	}
/// End Launcher::Operators
}