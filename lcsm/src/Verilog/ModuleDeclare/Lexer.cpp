#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Verilog/ModuleDeclare/Lexer.h>
#include <lcsm/Verilog/ModuleDeclare/Token.h>

#include <cctype>
#include <fstream>
#include <ios>
#include <string>
#include <utility>

lcsm::verilog::ModuleDeclareLexer::ModuleDeclareLexer(std::ifstream &&stream) : m_stream(std::move(stream))
{
	nextChar();
}

lcsm::verilog::ModuleDeclareLexer::ModuleDeclareLexer(lcsm::verilog::ModuleDeclareLexer &&other) :
	m_stream(std::move(other.m_stream)), m_token(std::move(other.m_token)), m_character(other.m_character),
	m_previousToken(std::move(other.m_previousToken))
{
}

lcsm::verilog::ModuleDeclareLexer &lcsm::verilog::ModuleDeclareLexer::operator=(lcsm::verilog::ModuleDeclareLexer &&other)
{
	return lcsm::support::MoveAssign< lcsm::verilog::ModuleDeclareLexer >(this, std::move(other));
}

void lcsm::verilog::ModuleDeclareLexer::swap(lcsm::verilog::ModuleDeclareLexer &other)
{
	std::swap(m_stream, other.m_stream);
	std::swap(m_token, other.m_token);
	std::swap(m_character, other.m_character);
	std::swap(m_previousToken, other.m_previousToken);
}

static bool IsNumber(const std::string &s) noexcept
{
	for (char c : s)
	{
		if (!('0' <= c && c <= '9'))
		{
			return false;
		}
	}
	return true;
}

const lcsm::verilog::ModuleDeclareToken &lcsm::verilog::ModuleDeclareLexer::nextToken()
{
	// Builder for long strings.
	std::string builder;

	// Skip all blank chars.
	while (isBlank(m_character))
	{
		nextChar();
	}

	// Check, if this character is just single kind.
	lcsm::verilog::ModuleDeclareKind kind = lcsm::verilog::IsKeyword(m_character);
	if (kind >= 0)
	{
		m_token.setToken(kind);
		nextChar();
		goto l_finish;
	}

	// Continue parsing to blank character (non valid).
	while (isValid(m_character))
	{
		builder.push_back(m_character);
		nextChar();
	}

	// Check, if this built string is keyword.
	kind = lcsm::verilog::IsKeyword(builder);
	if (kind >= 0)
	{
		m_token.setToken(kind);
		goto l_finish;
	}

	// Check, if built string is actually integer.
	// Otherwise, it's identifier.
	if (IsNumber(builder))
	{
		const int i = std::stoi(builder);
		m_token.setToken(i);
	}
	else
	{
		m_token.setToken(std::move(builder));
	}

l_finish:
	return m_token;
}

const lcsm::verilog::ModuleDeclareToken &lcsm::verilog::ModuleDeclareLexer::token() const noexcept
{
	return m_token;
}

void lcsm::verilog::ModuleDeclareLexer::returnToken() {}

void lcsm::verilog::ModuleDeclareLexer::nextChar()
{
	m_stream >> std::noskipws >> m_character;
}

bool lcsm::verilog::ModuleDeclareLexer::isBlank(char C) noexcept
{
	return std::isspace(C);
}

bool lcsm::verilog::ModuleDeclareLexer::isValid(char C) noexcept
{
	return std::isalnum(C) || C == '_';
}
