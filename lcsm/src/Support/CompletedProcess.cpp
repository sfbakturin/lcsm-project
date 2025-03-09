#include <lcsm/Support/Subprocesses.h>

#include <stdexcept>
#include <string>
#include <utility>

lcsm::support::CompletedProcess::CompletedProcess(int returncode) : m_returncode(returncode) {}

lcsm::support::CompletedProcess::CompletedProcess(int returncode, std::string &&out, std::string &&err) noexcept :
	m_returncode(returncode), m_stdout(std::move(out)), m_stderr(std::move(err))
{
}

lcsm::support::CompletedProcess::CompletedProcess(int returncode, const char *out, const char *err) :
	m_returncode(returncode), m_stdout(out), m_stderr(err)
{
}

void lcsm::support::CompletedProcess::checkReturnCode() const
{
	if (m_returncode != 0)
	{
		throw std::runtime_error("Process exited with nonzero error code!");
	}
}

int lcsm::support::CompletedProcess::returnCode() const noexcept
{
	return m_returncode;
}

const std::string &lcsm::support::CompletedProcess::stdOut() const noexcept
{
	return m_stdout;
}

const std::string &lcsm::support::CompletedProcess::stdErr() const noexcept
{
	return m_stderr;
}

const char *lcsm::support::CompletedProcess::cOut() const noexcept
{
	return stdOut().c_str();
}

const char *lcsm::support::CompletedProcess::cErr() const noexcept
{
	return stdErr().c_str();
}
