#ifndef LCSM_VERILOG_MODULEDECLARE_LEXER_H
#define LCSM_VERILOG_MODULEDECLARE_LEXER_H

#include <lcsm/Support/IO/Reader.h>
#include <lcsm/Verilog/ModuleDeclare/Token.h>
#include <lcsm/lcsmconfig.h>

#include <deque>
#include <memory>
#include <string>

namespace lcsm
{
	namespace verilog
	{
		class LCSM_API ModuleDeclareLexer
		{
		  public:
			ModuleDeclareLexer(const std::shared_ptr< support::Reader > &source);
			ModuleDeclareLexer(std::shared_ptr< support::Reader > &&source);

			ModuleDeclareLexer(const ModuleDeclareLexer &other) = delete;
			ModuleDeclareLexer(ModuleDeclareLexer &&other) noexcept;

			ModuleDeclareLexer &operator=(const ModuleDeclareLexer &other) = delete;
			ModuleDeclareLexer &operator=(ModuleDeclareLexer &&other) noexcept;

			void swap(ModuleDeclareLexer &other);

			const ModuleDeclareToken &nextToken();
			const ModuleDeclareToken &token() const noexcept;
			void backToken();

		  private:
			std::shared_ptr< support::Reader > m_source;
			ModuleDeclareToken m_token;
			char m_character;
			std::deque< ModuleDeclareToken > m_previousTokens;

		  private:
			void nextChar();
			void skipBlanks();
			void buildString(std::string &builder);
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_MODULEDECLARE_LEXER_H */
