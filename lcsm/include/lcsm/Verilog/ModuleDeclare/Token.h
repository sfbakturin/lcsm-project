#ifndef LCSM_VERILOG_MODULEDECLARE_TOKEN_H
#define LCSM_VERILOG_MODULEDECLARE_TOKEN_H

#include <lcsm/lcsmconfig.h>

#include <cstdint>
#include <string>

namespace lcsm
{
	namespace verilog
	{
		enum ModuleDeclareKind : std::int8_t
		{
			UnknownKind = -1,
			KW_EQUAL,
			KW_COLON,
			KW_COMMA,
			KW_SEMI,
			KW_LPAREN,
			KW_RPAREN,
			KW_LCURLY,
			KW_RCURLY,
			KW_LSQR,
			KW_RSQR,
			KW_MODULE,
			KW_INPUT,
			KW_INOUT,
			KW_OUTPUT,
			KW_SIGNED,
			KW_UNSIGNED,
			KW_SUPPLY0,
			KW_SUPPLY1,
			KW_TRI,
			KW_TRI0,
			KW_TRI1,
			KW_TRIAND,
			KW_TRIOR,
			KW_UWIRE,
			KW_WIRE,
			KW_WAND,
			KW_WOR,
			KW_BIT,
			KW_LOGIC,
			KW_REG,
			KW_INTEGER,
			KW_TIME,
			TK_INTEGER,
			TK_IDENTIFIER,
			TK_EOF
		};

		static constexpr std::int8_t KindKeywordStart = ModuleDeclareKind::KW_EQUAL;
		static constexpr std::int8_t KindKeywordEnd = ModuleDeclareKind::KW_TIME;

		LCSM_API const char *KindToKeyword(ModuleDeclareKind kind) noexcept;
		LCSM_API ModuleDeclareKind IsKeyword(const char *s) noexcept;
		LCSM_API ModuleDeclareKind IsKeyword(const std::string &s) noexcept;
		LCSM_API ModuleDeclareKind IsKeyword(char c) noexcept;
		LCSM_API bool IsStartOfKeyword(const char *s) noexcept;
		LCSM_API bool IsStartOfKeyword(const std::string &s) noexcept;

		class LCSM_API ModuleDeclareToken
		{
		  public:
			ModuleDeclareToken();
			ModuleDeclareToken(ModuleDeclareKind kind);
			ModuleDeclareToken(int i);
			ModuleDeclareToken(const std::string &s);
			ModuleDeclareToken(std::string &&s) noexcept;

			ModuleDeclareToken(const ModuleDeclareToken &other);
			ModuleDeclareToken(ModuleDeclareToken &&other) noexcept;

			ModuleDeclareToken &operator=(const ModuleDeclareToken &other);
			ModuleDeclareToken &operator=(ModuleDeclareToken &&other) noexcept;

			bool operator==(ModuleDeclareKind kind) const noexcept;
			bool operator!=(ModuleDeclareKind kind) const noexcept;

			void swap(ModuleDeclareToken &other) noexcept;

			ModuleDeclareKind kind() const noexcept;

			void setToken(ModuleDeclareKind kind) noexcept;
			void setToken(int i) noexcept;
			void setToken(const std::string &s);
			void setToken(std::string &&s) noexcept;

			void setEof() noexcept;

			bool isKeyword() const noexcept;
			bool isInteger() const noexcept;
			bool isIdentifier() const noexcept;
			bool isEof() const noexcept;

			int asInteger() const noexcept;
			const std::string &asIdentifier() const noexcept;

		  private:
			ModuleDeclareKind m_kind;
			int m_i;
			std::string m_s;
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_MODULEDECLARE_TOKEN_H */
