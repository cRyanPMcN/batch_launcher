/*
File: rm_LaunchGroup.hpp
Author: Ryan McNamee
Date Created: Friday, 22, September, 2017
Date Updated: Monday, 9, October, 2017
Purpose: Wraps C Functions for Windows processes
*/
#ifndef RM_PROCESS_HPP_HEADER_GUARD
#define RM_PROCESS_HPP_HEADER_GUARD
#include <string>
#include <Windows.h>

namespace rm {
	// Constant for maximum length of a command in the windows commandline
	static const unsigned long CP_MAX_COMMANDLINE = 32768;

	// Wraps SYSTEMTIMEs for process
	struct ProcessTime {
		SYSTEMTIME creationTime, exitTime, kernalTime, userTime;
	};

	class Process {
	public:		/// Types
		using process_handle = HANDLE;
		using size_type = size_t;
		using system_time = SYSTEMTIME;
		using proc_info = PROCESS_INFORMATION;
		using start_info = STARTUPINFO;
		using exit_code = DWORD;
		using process_time = ProcessTime;
	private:	/// Variables
		bool started = false;
		/// Process creation data
		size_type launchgroup;
		std::wstring command;
		std::wstring app, params;

		/// Process start data
		proc_info process;
		start_info sinfo;

		/// Process finish data
		exit_code exitCode;
		process_time procTime;
	public:		/// Methods
		/// Constructors
		Process(size_type lgroup, std::wstring const& app, std::wstring const& params);

		~Process();

		/// Operations
		// Method: Process::Start
		// Purpose: Starts the process and waits for it to complete
		void Start();

		// Method: Process::StartAsync
		// Purpose: Starts the process without waiting for it to complete
		void StartAsync();

		// Method: Process::Stop
		// Purpose: Immediately terminates the process with exit code of EXIT_FAILURE
		void Stop();

		// Method: Process::Done
		// Output: If process has finished
		// Purpose: Check if the process is alive
		// WARNING: Will never return true if process exits with code 259
		// DEPRECIATED: More reliable method created
		bool Done();

		// Method: Process::DidRun
		// Output: Whether the Process was successfully started
		inline bool DidRun() {
			return started;
		}

		// Method: Process::Succeded
		// Output: Whether the process exited with the exit code 0
		inline bool Succeded() {
			PrepareExitCode();
			return exitCode == EXIT_SUCCESS;
		}

		// Method: Process::GetCommand
		// Output: The command that was used to start the process
		inline std::wstring const& GetCommand() {
			return command;
		}

		// Method: Process::GetLaunchGroup
		// Output: The launchgroup the process should be in
		inline size_type GetLaunchGroup() {
			return launchgroup;
		}

		// Method: Process::GetApp
		// Output: Application that Process may attempt to start
		inline std::wstring const& GetApp() {
			return app;
		}
		
		// Method: Process::GetParams
		// Output: Parameters of application that Process may attempt to start
		inline std::wstring const& GetParams() {
			return params;
		}

		// Method: Process::GetExitCode
		// Output: ExitCode of completed process
		inline exit_code GetExitCode() {
			PrepareExitCode();
			return exitCode;
		}

		// Method: Process::GetHandle
		// Output: Handle to running process
		inline process_handle GetHandle() {
			return process.hProcess;
		}
		// Method: Process::GetProcessTime
		// Output: All available timing information for the Process
		inline process_time GetProcessTime() {
			PrepareProcessTimes();
			return procTime;
		}

	protected:
		// Method: Process::BasicStart
		// Purpose: Wraps C Function CreateProcess() to start the process
		void BasicStart();

		// Method: PrepareExitCode
		// Purpose: Loads the ExitCode for the process into Process::exitCode
		inline void PrepareExitCode() {
			GetExitCodeProcess(process.hProcess, &exitCode);
		}

		// Method: Process::PrepareProcessTimes
		// Purpose: Loads creationTime, exitTime, kernalTime, userTime for process into Process::procTime
		inline void PrepareProcessTimes() {
			FILETIME cTime, eTime, kTime, uTime;
			GetProcessTimes(process.hProcess, &cTime, &eTime, &kTime, &uTime);

			FileTimeToSystemTime(&cTime, &procTime.creationTime);
			FileTimeToSystemTime(&eTime, &procTime.exitTime);
			FileTimeToSystemTime(&kTime, &procTime.kernalTime);
			FileTimeToSystemTime(&uTime, &procTime.userTime);
		}
	};

	/// Operators
	// Method: Process::operator<<
	// Input: output stream, Process to parse from
	// Output output stream
	// Purpose: Write information about process to an output stream
	std::wostream& operator <<(std::wostream& lhs, Process& rhs);
}

#endif