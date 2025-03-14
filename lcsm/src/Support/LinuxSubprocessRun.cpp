#include <lcsm/Support/Subprocesses.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sched.h>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <utility>

#define PIPER 0
#define PIPEW 1

static constexpr std::size_t BUFFER_LENGTH = 4096;
static constexpr std::size_t BUFFER_SIZE = BUFFER_LENGTH * sizeof(char);

lcsm::support::CompletedProcess lcsm::support::subprocessRun(char *cmdline)
{
	// Create pipes.
	int outpipefd[2];
	int errpipefd[2];

	if (pipe(outpipefd) == -1)
	{
		throw std::runtime_error(strerror(errno));
	}

	if (pipe(errpipefd) == -1)
	{
		close(outpipefd[PIPER]);
		close(outpipefd[PIPEW]);
		throw std::runtime_error(strerror(errno));
	}

	// Fork.
	pid_t childpid = fork();

	if (childpid < 0)
	{
		close(outpipefd[PIPER]);
		close(outpipefd[PIPEW]);
		close(errpipefd[PIPEW]);
		close(errpipefd[PIPEW]);
		throw std::runtime_error(strerror(errno));
	}

	if (childpid == 0)
	{
		// Now as child process.

		// Close all pipes for reading.
		close(outpipefd[PIPER]);
		close(errpipefd[PIPER]);

		// Duplicate from stdout/stderr.
		if (dup2(outpipefd[PIPEW], STDOUT_FILENO) == -1)
		{
			close(outpipefd[PIPEW]);
			close(errpipefd[PIPEW]);
			throw std::runtime_error(strerror(errno));
		}

		if (dup2(errpipefd[PIPEW], STDERR_FILENO) == -1)
		{
			close(outpipefd[PIPEW]);
			close(errpipefd[PIPEW]);
			throw std::runtime_error(strerror(errno));
		}

		// Close pipes.
		close(outpipefd[PIPEW]);
		close(errpipefd[PIPEW]);

		// Execute process.
		std::exit(WEXITSTATUS(std::system(cmdline)));
	}

	// Now as parent process.

	// Close all pipes for writing.
	close(outpipefd[PIPEW]);
	close(errpipefd[PIPEW]);

	// Get stdout/stderr.
	std::string stdOut;
	std::string stdErr;
	char buffer[BUFFER_LENGTH + 1] = { 0 };
	ssize_t numberOfBytesRead;

	while ((numberOfBytesRead = read(outpipefd[PIPER], buffer, BUFFER_SIZE)) > 0)
	{
		buffer[numberOfBytesRead] = '\0';
		stdOut += buffer;
	}

	while ((numberOfBytesRead = read(errpipefd[PIPER], buffer, BUFFER_SIZE)) > 0)
	{
		buffer[numberOfBytesRead] = '\0';
		stdErr += buffer;
	}

	// Close pipes.
	close(outpipefd[PIPER]);
	close(errpipefd[PIPER]);

	// Get returncode.
	int status;
	waitpid(childpid, &status, 0);
	const int returncode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

	return { returncode, std::move(stdOut), std::move(stdErr) };
}
