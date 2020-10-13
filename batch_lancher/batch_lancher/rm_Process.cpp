/*
File: rm_LaunchGroup.hpp
Author: Ryan McNamee
Date Created: Friday, 22, September, 2017
Date Updated: Monday, 9, October, 2017
Purpose: Wraps C Functions for Windows processes
*/
#include "rm_Process.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace rm {
/// Begin Process::Constructors
	Process::Process(size_type lgroup, std::wstring const& app, std::wstring const& params)
		: launchgroup(lgroup), app(app), params(params), command(L"\"" + app + L"\" " + params) {
	
	}

	Process::~Process() {
		// Close Process handles
		CloseHandle(process.hThread);
		CloseHandle(process.hProcess);
	}
/// End Process::Constructors

/// Begin Process::Operations
	// Method: Process::BasicStart
	// Purpose: Wraps C Function CreateProcess() to start the process
	void Process::BasicStart() {
		// Initialize information for process
		sinfo = { 0 };
		sinfo.cb = sizeof(sinfo);
		process = { 0 };

		try {
			// Create an empty string to copy command into
			wchar_t* commandline = new wchar_t[CP_MAX_COMMANDLINE];

			// Copy Process::command into empty string
			wcsncpy_s(commandline, CP_MAX_COMMANDLINE, command.c_str(), command.size() + 1);

			// Attempt to start process
			// Log whether the process started
			started = CreateProcess(NULL, commandline, NULL, NULL, false, CREATE_NEW_CONSOLE, NULL, NULL, &sinfo, &process);
			// Destroy no longer empty string
			delete[] commandline;
		}
		catch (std::bad_alloc&) {
			std::wcerr << L"Insufficient memory to launch the application." << std::endl;
			return;
		}
	}

	// Method: Process::Start
	// Purpose: Starts the process and waits for it to complete
	void Process::Start() {
		BasicStart();
		// Wait for process to finish
		if (WAIT_FAILED == WaitForSingleObject(process.hProcess, INFINITE))
			std::cerr << "Failure waiting for proccess to terminate." << std::endl;
	}

	// Method: Process::StartAsync
	// Purpose: Starts the process without waiting for it to complete
	void Process::StartAsync() {
		BasicStart();
	}

	// Method: Process::Stop
	// Purpose: Immediately terminates the process with exit code of EXIT_FAILURE
	void Process::Stop() {
		exitCode = EXIT_FAILURE;
		TerminateProcess(process.hProcess, exitCode);
	}

	// Method: Process::Done
	// Output: If process has finished
	// Purpose: Check if the process is alive
	// WARNING: Will never return true if process exits with code 259
	// DEPRECIATED: More reliable method created
	bool Process::Done() {
		GetExitCodeProcess(process.hProcess, &exitCode);
		return exitCode != STILL_ACTIVE;
	}
/// End Process::Operations

/// Begin Process::Operators
	// Method: Process::operator<<
	// Input: output stream, Process to parse from
	// Output output stream
	// Purpose: Write information about process to an output stream
	std::wostream& operator<<(std::wostream& lhs, Process& rhs) {
		// Get and seperate kernalTime and usertime
		rm::ProcessTime procTime = rhs.GetProcessTime();
		Process::system_time kTime = procTime.kernalTime;
		Process::system_time uTime = procTime.userTime;

		// Write kernalTime to a string
		std::wstringstream oss;
		oss << kTime.wHour << ":" << kTime.wMinute << ":" << kTime.wSecond << "." << kTime.wMilliseconds;
		std::wstring kernalTime = oss.str();
		// Clear stringstream
		oss.clear();
		// Write userTime to a string
		oss << uTime.wHour << ":" << uTime.wMinute << ":" << uTime.wSecond << "." << uTime.wMilliseconds;
		std::wstring userTime = oss.str();

		// Write process info to output stream
		lhs << std::setw(3) << std::right
			<< rhs.GetLaunchGroup() << L" |"
			<< std::setw(12) << std::right
			<< kernalTime << L" |"
			<< std::setw(12) << std::right
			<< userTime << L" |"
			<< std::setw(5) << std::right
			<< rhs.GetExitCode() << L" | "
			<< std::left
			<< rhs.GetApp() << std::endl
			<< L"Parameters: " << rhs.GetParams() << std::endl;

		// Return output stream
		return lhs;
	}
/// End Process::Operators
}