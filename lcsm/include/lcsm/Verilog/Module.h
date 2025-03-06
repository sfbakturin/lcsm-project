#ifndef LCSM_VERILOG_MODULE_H
#define LCSM_VERILOG_MODULE_H

#include <cstdio>
#include <fstream>
#include <string>

namespace lcsm
{
	namespace verilog
	{
		class Module
		{
		  public:
			static Module fromFile(std::FILE *stream);
			static Module fromFile(const char *filename);
			static Module fromFile(std::ifstream &&stream);

			static Module fromString(const char *string);
			static Module fromString(const std::string &string);
			static Module fromString(std::string &&string);

		  private:
			Module() noexcept = default;
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_MODULE_H */
