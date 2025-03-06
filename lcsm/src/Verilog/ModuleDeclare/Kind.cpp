#include <lcsm/Verilog/ModuleDeclare/Token.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

const char *lcsm::verilog::KindToKeyword(lcsm::verilog::ModuleDeclareKind kind) noexcept
{
	switch (kind)
	{
	case lcsm::verilog::ModuleDeclareKind::KW_EQUAL:
	{
		return "=";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_COLON:
	{
		return ":";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_COMMA:
	{
		return ",";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_SEMI:
	{
		return ";";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_LPAREN:
	{
		return "(";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_RPAREN:
	{
		return ")";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_LCURLY:
	{
		return "{";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_RCURLY:
	{
		return "}";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_LSQR:
	{
		return "[";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_RSQR:
	{
		return "]";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_MODULE:
	{
		return "module";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_INOUT:
	{
		return "inout";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_INPUT:
	{
		return "input";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT:
	{
		return "output";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT_REG:
	{
		return "output reg";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_SIGNED:
	{
		return "signed";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_SUPPLY0:
	{
		return "supply0";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_SUPPLY1:
	{
		return "supply1";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TRI:
	{
		return "tri";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TRI0:
	{
		return "tri0";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TRI1:
	{
		return "tri1";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TRIAND:
	{
		return "triand";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TRIOR:
	{
		return "trior";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_UWIRE:
	{
		return "uwire";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_WIRE:
	{
		return "wire";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_WAND:
	{
		return "wand";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_WOR:
	{
		return "wor";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_INTEGER:
	{
		return "integer";
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TIME:
	{
		return "time";
	}
	default:
	{
		return nullptr;
	}
	}
	return nullptr;
}

lcsm::verilog::ModuleDeclareKind lcsm::verilog::IsKeyword(const char *s) noexcept
{
	for (std::int8_t it = lcsm::verilog::KindKeywordStart; it <= lcsm::verilog::KindKeywordEnd; it++)
	{
		const lcsm::verilog::ModuleDeclareKind kind = static_cast< lcsm::verilog::ModuleDeclareKind >(it);
		const char *keyword = KindToKeyword(kind);
		if (std::strcmp(keyword, s) == 0)
		{
			return kind;
		}
	}
	return lcsm::verilog::ModuleDeclareKind::UnknownKind;
}

lcsm::verilog::ModuleDeclareKind lcsm::verilog::IsKeyword(const std::string &s) noexcept
{
	return lcsm::verilog::IsKeyword(s.c_str());
}

lcsm::verilog::ModuleDeclareKind lcsm::verilog::IsKeyword(char c) noexcept
{
	switch (c)
	{
	case '=':
		return lcsm::verilog::ModuleDeclareKind::KW_EQUAL;
	case ':':
		return lcsm::verilog::ModuleDeclareKind::KW_COLON;
	case ',':
		return lcsm::verilog::ModuleDeclareKind::KW_COMMA;
	case ';':
		return lcsm::verilog::ModuleDeclareKind::KW_SEMI;
	case '(':
		return lcsm::verilog::ModuleDeclareKind::KW_LPAREN;
	case ')':
		return lcsm::verilog::ModuleDeclareKind::KW_RPAREN;
	case '{':
		return lcsm::verilog::ModuleDeclareKind::KW_LCURLY;
	case '}':
		return lcsm::verilog::ModuleDeclareKind::KW_RCURLY;
	case '[':
		return lcsm::verilog::ModuleDeclareKind::KW_LSQR;
	case ']':
		return lcsm::verilog::ModuleDeclareKind::KW_RSQR;
	default:
		return lcsm::verilog::ModuleDeclareKind::UnknownKind;
	}
	return lcsm::verilog::ModuleDeclareKind::UnknownKind;
}

bool lcsm::verilog::IsStartOfKeyword(const char *s) noexcept
{
	const std::size_t len = std::strlen(s);
	for (std::int8_t it = lcsm::verilog::KindKeywordStart; it <= lcsm::verilog::KindKeywordEnd; it++)
	{
		const lcsm::verilog::ModuleDeclareKind kind = static_cast< lcsm::verilog::ModuleDeclareKind >(it);
		const char *keyword = KindToKeyword(kind);
		const std::size_t lenKeyword = std::strlen(keyword);
		if (len != lenKeyword && std::strncmp(keyword, s, len) == 0)
		{
			return true;
		}
	}
	return false;
}

bool lcsm::verilog::IsStartOfKeyword(const std::string &s) noexcept
{
	return lcsm::verilog::IsStartOfKeyword(s.c_str());
}
