#ifndef LCSM_VERILOG_MODULEDECLARE_PARSER_H
#define LCSM_VERILOG_MODULEDECLARE_PARSER_H

#include <lcsm/Support/IO/Reader.h>
#include <lcsm/Verilog/ModuleDeclare/Context.h>
#include <lcsm/Verilog/ModuleDeclare/Lexer.h>
#include <lcsm/Verilog/Port.h>
#include <lcsm/lcsmconfig.h>

#include <memory>

namespace lcsm
{
	namespace verilog
	{
		class LCSM_API ModuleDeclareParser
		{
		  public:
			ModuleDeclareParser(const std::shared_ptr< support::Reader > &source);
			ModuleDeclareParser(std::shared_ptr< support::Reader > &&source) noexcept;

			ModuleDeclareParser(const ModuleDeclareParser &other) = delete;
			ModuleDeclareParser(ModuleDeclareParser &&other) noexcept;

			ModuleDeclareParser &operator=(const ModuleDeclareParser &other) = delete;
			ModuleDeclareParser &operator=(ModuleDeclareParser &&other) noexcept;

			void swap(ModuleDeclareParser &other);

			void parse(ModuleDeclareContext &context);

		  private:
			ModuleDeclareLexer m_lex;
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_MODULEDECLARE_PARSER_H */
