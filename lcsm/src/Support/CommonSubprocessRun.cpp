#include <initializer_list>
#include <lcsm/Support/Subprocesses.h>

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

lcsm::support::CompletedProcess lcsm::support::subprocessRun(const char *exec, const std::vector< std::string > &args)
{
	// Determine exec's length and the othe's args.
	const std::size_t exeLength = std::strlen(exec);
	std::size_t n = exeLength;
	for (const std::string &arg : args)
	{
		n += arg.size() + 1;
	}

	// Copy to C-string for subprocessRun(char *).
	char *cmdline = new char[n + 1];
	cmdline[n] = '\0';

	std::strcpy(cmdline, exec);
	std::size_t offset = exeLength;

	for (const std::string &arg : args)
	{
		cmdline[offset] = ' ';
		offset++;
		std::strcpy(cmdline + offset, arg.c_str());
		offset += arg.size();
	}

	// Run subprocess.
	try
	{
		const lcsm::support::CompletedProcess completed = lcsm::support::subprocessRun(cmdline);
		delete[] cmdline;
		return completed;
	} catch (...)
	{
		delete[] cmdline;
		throw;
	}
}

lcsm::support::CompletedProcess lcsm::support::subprocessRun(const char *exec, std::initializer_list< const char * > args)
{
	// Determine exe's length and the othe's args.
	const std::size_t execLength = std::strlen(exec);
	std::size_t n = execLength;
	for (const char *arg : args)
	{
		n += std::strlen(arg) + 1;
	}

	// Copy to C-string for subprocessRun(char *).
	char *cmdline = new char[n + 1];
	cmdline[n] = '\0';

	std::strcpy(cmdline, exec);
	std::size_t offset = execLength;

	for (const char *arg : args)
	{
		cmdline[offset] = ' ';
		offset++;
		std::strcpy(cmdline + offset, arg);
		offset += std::strlen(arg);
	}

	// Run subprocess.
	try
	{
		const lcsm::support::CompletedProcess completed = lcsm::support::subprocessRun(cmdline);
		delete[] cmdline;
		return completed;
	} catch (...)
	{
		delete[] cmdline;
		throw;
	}
}
