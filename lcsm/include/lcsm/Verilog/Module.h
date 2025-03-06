#ifndef LCSM_VERILOG_MODULE_H
#define LCSM_VERILOG_MODULE_H

#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Verilog/Port.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

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
			std::string m_identifier;
			std::vector< Port > m_ports;

		  private:
			Module() noexcept = default;

			static Module parse(const std::shared_ptr< support::CharSource > &source);
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_MODULE_H */
