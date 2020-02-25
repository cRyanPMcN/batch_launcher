/*
File: rm_Launcher.hpp
Author: Ryan McNamee
Date Created: Friday, 29, September, 2017
Date Updated: Monday, 9, October, 2017
Purpose: Parse input file, generate sorted launchgroups and processes
		Output basic information about run
*/
#ifndef RM_LAUNCHER_HEADER_GUARD
#define RM_LAUNCHER_HEADER_GUARD
#include "rm_LaunchGroup.hpp"
#include <set>
#include <fstream>
#include <map>

namespace rm {
	class Launcher {
	public:		/// Types
		using key_type = size_t;
		using value_type = rm::LaunchGroup;
		using pointer_type = value_type*;
		using container_type = std::map<key_type, value_type>;
		using pair_type = std::pair<key_type, value_type::pointer_type>;

		using size_type = size_t;
	private:	/// Variables
		container_type lgroups;
	public:		/// Methods
		/// Constructors
		Launcher();

		~Launcher();

		/// Operations
		// Method: rm::Launcher::Add
		// Purpose: Handles the sorting of processes into launchgroups
		void Add(value_type::pointer_type const& proc);

		// Method: rm::Launcher::RunAll
		// Purpose: Calls launchgroups to launch processes
		void RunAll();

		// Method: rm::Launcher::PrintData
		// Purpose: Calls all launchgroups to print process data
		// Reason: Did not want to give access to data structure
		void PrintData(std::wostream& outstream);
	};

	/// Operators
	// Method: operator>>
	// Input: input file, Launcher to parse into
	// Output: input file
	// Purpose: Parses through input file for launchgroup and application information
	//			generates process object, sorting of processes is handled elsewhere
	std::wifstream& operator>>(std::wifstream& lhs, Launcher& rhs);
	// Method: operator>>
	// Input: output stream, Launcher to parse from
	// Output: output stream
	// Purpose: Prints Table Headers and error data
	std::wostream& operator<<(std::wostream& lhs, Launcher& rhs);
}

#endif