#ifndef LCSM_VERILOG_MODULEDECLARE_LEXER_H
#define LCSM_VERILOG_MODULEDECLARE_LEXER_H

#include <lcsm/Verilog/ModuleDeclare/Token.h>

#include <fstream>
#include <vector>

namespace lcsm
{
	namespace verilog
	{
		class ModuleDeclareLexer
		{
		  public:
			ModuleDeclareLexer(std::ifstream &&stream);

			ModuleDeclareLexer(const ModuleDeclareLexer &other) = delete;
			ModuleDeclareLexer(ModuleDeclareLexer &&other);

			ModuleDeclareLexer &operator=(const ModuleDeclareLexer &other) = delete;
			ModuleDeclareLexer &operator=(ModuleDeclareLexer &&other);

			void swap(ModuleDeclareLexer &other);

			const ModuleDeclareToken &nextToken();
			const ModuleDeclareToken &token() const noexcept;

			void returnToken();

		  private:
			std::ifstream m_stream;
			ModuleDeclareToken m_token;
			char m_character;
			std::vector< ModuleDeclareToken > m_previousToken;

		  private:
			void nextChar();
			static bool isBlank(char C) noexcept;
			static bool isValid(char C) noexcept;
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_MODULEDECLARE_LEXER_H */
