#ifndef LCSM_VERILOG_MODULEDECLARE_PARSER_H
#define LCSM_VERILOG_MODULEDECLARE_PARSER_H

#include <lcsm/Verilog/ModuleDeclare/Lexer.h>

#include <fstream>

namespace lcsm
{
	namespace verilog
	{
		class ModuleDeclareParser
		{
		  public:
			ModuleDeclareParser(const char *filename);
			ModuleDeclareParser(std::ifstream &&stream);

			ModuleDeclareParser(const ModuleDeclareParser &other) = delete;
			ModuleDeclareParser(ModuleDeclareParser &&other);

			ModuleDeclareParser &operator=(const ModuleDeclareParser &other) = delete;
			ModuleDeclareParser &operator=(ModuleDeclareParser &&other);

			void swap(ModuleDeclareParser &other);

			void parse();

		  private:
			ModuleDeclareLexer m_lex;
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_MODULEDECLARE_PARSER_H */
