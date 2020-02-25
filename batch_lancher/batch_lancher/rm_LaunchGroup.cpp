/*
File: rm_LaunchGroup.hpp
Author: Ryan McNamee
Date Created: Monday, 25, September, 2017
Date Updated: Monday, 9, October, 2017
Purpose: Holds and manipulates an unordered list of processes
*/
#include "rm_LaunchGroup.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace rm {
/// Begin LaunchGroup::Constructors
	LaunchGroup::LaunchGroup() {}

	LaunchGroup::~LaunchGroup() {
		// Clear unordered list
		for (pointer_type proc : procs)
			if (proc != nullptr)
				delete proc;
	}
/// End LaunchGroup::Constructors

/// Begin LaunchGroup::Operations
	// Method: LaunchGroup::Add
	// Input: Pointer to rm::Process
	// Output: Void
	// Purpose: Add process to unordered list
	void LaunchGroup::Add(pointer_type proc) {
		procs.push_back(proc);
	}

	// Method: LaunchGroup::Done
	// Output: If all processes in launchgroup have completed
	// Purpose: Check if all processes have completed
	// DEPRECIATED: More reliable method created
	bool LaunchGroup::Done() {
		bool done = true;
		for (pointer_type proc : procs) {
			// Check if any processes have not completed
			if (proc->Done() == false)
				done = false;
		}

		return done;
	}

	// Method: LaunchGroup::RunAll
	// Purpose: Start all processes synchronously
	void LaunchGroup::RunAll() {
		for (pointer_type proc : procs) {
			proc->Start();
		}
	}

	// Method: LaunchGroup::RunAllAsync
	// Purpose: Start all processes asynchronously
	void LaunchGroup::RunAllAsync() {
		for (pointer_type proc : procs) {
			proc->StartAsync();
			if (proc->DidRun())
				procHandles.push_back(proc->GetHandle());
		}
		WaitForMultipleObjects(procHandles.size(), procHandles.data(), TRUE, INFINITE);
	}

	// Method: LaunchGroup::Print
	// Input: output stream to print to
	// Purpose: Print data from each started process
	void LaunchGroup::Print(std::wostream& outstream) {
		for (pointer_type proc : procs) {
			if(proc->DidRun())
				outstream << *proc;
		}
	}

	// Method: LaunchGroup::PrintErrors
	// Input: output stream to print to
	// Purpose: Print any processes which failed to start or exited with an error
	void LaunchGroup::PrintErrors(std::wostream& outstream) {
		for (pointer_type proc : procs) {
			if (!proc->DidRun())
				outstream << L"ERROR - Application not run: "
				<< proc->GetLaunchGroup() << L": " << proc->GetApp() << L", Parameters: " << proc->GetParams() << std::endl;
			else if (!proc->Succeded())
				outstream << L"ERROR - Application exited with error: "
				<< proc->GetExitCode() << L" App: " << proc->GetLaunchGroup() << L": " << proc->GetApp() << L", Parameters: " << proc->GetParams() << std::endl;
		}
	}

	// Method: LaunchGroup::operator<<
	// Input: output stream, Launchgroup to parse from
	// Purpose: Calls print function
	std::wostream& operator<<(std::wostream& lhs, LaunchGroup& rhs)
	{
		rhs.Print(lhs);

		return lhs;
	}
/// End LaunchGroup::Operations
}