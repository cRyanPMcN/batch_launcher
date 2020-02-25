/*
File: rm_LaunchGroup.hpp
Author: Ryan McNamee
Date Created: Monday, 25, September, 2017
Date Updated: Monday, 9, October, 2017
Purpose: Holds and manipulates an unordered list of processes
*/
#ifndef RM_LAUNCHGROUP_HEADER_GUARD
#define RM_LAUNCHGROUP_HEADER_GUARD
#include "rm_Process.hpp"
#include <list>
#include <vector>
#include <sstream>
#include <string>

namespace rm {
	class LaunchGroup {
	public:		/// Types
		using value_type = Process;
		using pointer_type = Process*;
		using reference_type = Process&;
		using const_reference_type = const reference_type;

		using size_type = size_t;
	public:		/// Variables
		std::list<pointer_type> procs;
		std::vector<Process::process_handle> procHandles;
	public:		/// Methods
		/// Constructors
		LaunchGroup();

		~LaunchGroup();

		/// Operations
		// Method: LaunchGroup::Add
		// Input: Pointer to rm::Process
		// Output: Void
		// Purpose: Add process to unordered list
		void Add(pointer_type proc);

		// Method: LaunchGroup::Done
		// Output: If all processes in launchgroup have completed
		// Purpose: Check if all processes have completed
		// DEPRECIATED: More reliable method created
		bool Done();

		// Method: LaunchGroup::RunAll
		// Purpose: Start all processes synchronously
		void RunAll();

		// Method: LaunchGroup::RunAllAsync
		// Purpose: Start all processes asynchronously
		void RunAllAsync();

		// Method: LaunchGroup::Print
		// Input: output stream to print to
		// Purpose: Print data from each started process
		void Print(std::wostream& outstream);

		// Method: LaunchGroup::PrintErrors
		// Input: output stream to print to
		// Purpose: Print any processes which failed to start or exited with an error
		void PrintErrors(std::wostream& outstream);
	};

	/// Operators
	// Method: LaunchGroup::operator<<
	// Input: output stream, Launchgroup to parse from
	// Purpose: Calls print function
	std::wostream& operator<<(std::wostream& lhs, LaunchGroup& rhs);
}

#endif // !RM_LAUNCHGROUP_HEADER_GUARD