#include <lcsm/Support/Subprocesses.h>

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>
#include <windows.h>
#include <winnt.h>

static constexpr std::size_t BUFFER_LENGTH = 4096;
static constexpr std::size_t BUFFER_SIZE = BUFFER_LENGTH * sizeof(char);
static constexpr DWORD SEC_TO_MS = 100;
static constexpr DWORD MILLISECONDS_TO_WAIT = 120 * SEC_TO_MS;

lcsm::support::CompletedProcess lcsm::support::subprocessRun(char *cmdline)
{
	// Security attributes for child process.
	SECURITY_ATTRIBUTES securityAttributes;
	ZeroMemory(&securityAttributes, sizeof(securityAttributes));
	securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributes.bInheritHandle = TRUE;
	securityAttributes.lpSecurityDescriptor = NULL;

	// Pipes for reading from stdout and stderr.
	HANDLE childPipeStdoutRead;
	HANDLE childPipeStdoutWrite;
	HANDLE childPipeStderrRead;
	HANDLE childPipeStderrWrite;

	if (CreatePipe(&childPipeStdoutRead, &childPipeStdoutWrite, &securityAttributes, 0) == FALSE)
	{
		throw std::runtime_error("Bad pipe creation!");
	}
	if (CreatePipe(&childPipeStderrRead, &childPipeStderrWrite, &securityAttributes, 0) == FALSE)
	{
		throw std::runtime_error("Bad pipe creation!");
	}

	if (SetHandleInformation(childPipeStdoutRead, HANDLE_FLAG_INHERIT, 0) == FALSE)
	{
		CloseHandle(childPipeStdoutWrite);
		CloseHandle(childPipeStderrWrite);
		throw std::runtime_error("Bad setting handle creation!");
	}

	if (SetHandleInformation(childPipeStderrRead, HANDLE_FLAG_INHERIT, 0) == FALSE)
	{
		CloseHandle(childPipeStdoutWrite);
		CloseHandle(childPipeStderrWrite);
		throw std::runtime_error("Bad setting handle creation!");
	}

	// Startup information with redirecting stdout and stderr.
	STARTUPINFO startUpInfo;
	ZeroMemory(&startUpInfo, sizeof(STARTUPINFO));
	startUpInfo.cb = sizeof(STARTUPINFO);
	startUpInfo.hStdError = childPipeStderrWrite;
	startUpInfo.hStdOutput = childPipeStdoutWrite;
	startUpInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Process information.
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

	// Create process, if not successful - raise the runtime error.
	if (CreateProcessA(NULL, static_cast< LPSTR >(cmdline), NULL, NULL, TRUE, 0, NULL, NULL, &startUpInfo, &processInfo) == FALSE)
	{
		CloseHandle(childPipeStdoutWrite);
		CloseHandle(childPipeStderrWrite);
		throw std::runtime_error("Bad setting handle creation!");
	}

	// Close for writing to stdout/stderr.
	CloseHandle(childPipeStdoutWrite);
	CloseHandle(childPipeStderrWrite);

	// Get stdout/stderr.
	std::string stdOut;
	std::string stdErr;
	char buffer[BUFFER_LENGTH + 1] = { 0 };
	DWORD numberOfBytesRead;

	while (ReadFile(childPipeStdoutRead, buffer, BUFFER_SIZE, &numberOfBytesRead, NULL) && numberOfBytesRead != 0)
	{
		buffer[numberOfBytesRead] = '\0';
		stdOut += buffer;
	}

	while (ReadFile(childPipeStderrRead, buffer, BUFFER_SIZE, &numberOfBytesRead, NULL) && numberOfBytesRead != 0)
	{
		buffer[numberOfBytesRead] = '\0';
		stdErr += buffer;
	}

	// Close for reading from stdout/stderr.
	CloseHandle(childPipeStdoutRead);
	CloseHandle(childPipeStderrRead);

	// Get returncode.
	DWORD returncode;
	DWORD waitedStatus = WaitForSingleObject(processInfo.hProcess, MILLISECONDS_TO_WAIT);
	if (waitedStatus == WAIT_TIMEOUT || waitedStatus == WAIT_FAILED)
	{
		throw std::runtime_error("Child process in infinity loop!");
	}
	if (GetExitCodeProcess(processInfo.hProcess, &returncode) == FALSE)
	{
		throw std::runtime_error("Child process doesn't return code!");
	}

	return { static_cast< int >(returncode), std::move(stdOut), std::move(stdErr) };
}
