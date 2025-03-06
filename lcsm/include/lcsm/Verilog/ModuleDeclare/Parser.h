#ifndef LCSM_VERILOG_MODULEDECLARE_PARSER_H
#define LCSM_VERILOG_MODULEDECLARE_PARSER_H

#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Verilog/ModuleDeclare/Context.h>
#include <lcsm/Verilog/ModuleDeclare/Lexer.h>
#include <lcsm/Verilog/Port.h>

#include <memory>

namespace lcsm
{
	namespace verilog
	{
		class ModuleDeclareParser
		{
		  public:
			ModuleDeclareParser(const std::shared_ptr< support::CharSource > &source);
			ModuleDeclareParser(std::shared_ptr< support::CharSource > &&source);

			ModuleDeclareParser(const ModuleDeclareParser &other) = delete;
			ModuleDeclareParser(ModuleDeclareParser &&other);

			ModuleDeclareParser &operator=(const ModuleDeclareParser &other) = delete;
			ModuleDeclareParser &operator=(ModuleDeclareParser &&other);

			void swap(ModuleDeclareParser &other);

			void parse(ModuleDeclareContext &context);

		  private:
			ModuleDeclareLexer m_lex;
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_MODULEDECLARE_PARSER_H */
