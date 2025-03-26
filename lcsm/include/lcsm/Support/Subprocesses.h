#ifndef LCSM_SUPPORT_SUBPROCESS_H
#define LCSM_SUPPORT_SUBPROCESS_H

#include <initializer_list>
#include <lcsm/lcsmconfig.h>

#include <string>
#include <vector>

namespace lcsm
{
	namespace support
	{
		class LCSM_API CompletedProcess
		{
		  public:
			CompletedProcess(int returncode);
			CompletedProcess(int returncode, std::string &&out, std::string &&err) noexcept;
			CompletedProcess(int returncode, const char *out, const char *err);

			void checkReturnCode() const;

			int returnCode() const noexcept;
			const std::string &stdOut() const noexcept;
			const std::string &stdErr() const noexcept;
			const char *cOut() const noexcept;
			const char *cErr() const noexcept;

		  private:
			int m_returncode;
			std::string m_stdout;
			std::string m_stderr;
		};

		LCSM_API CompletedProcess subprocessRun(char *cmdline);
		LCSM_API CompletedProcess subprocessRun(const char *exec, const std::vector< std::string > &args);
		LCSM_API CompletedProcess subprocessRun(const char *exec, std::initializer_list< const char * > args);
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_SUBPROCESS_H */
