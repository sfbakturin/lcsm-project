#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Verilog/ModuleDeclare/Lexer.h>
#include <lcsm/Verilog/ModuleDeclare/Token.h>

#include <cctype>
#include <memory>
#include <string>
#include <utility>

lcsm::verilog::ModuleDeclareLexer::ModuleDeclareLexer(const std::shared_ptr< lcsm::support::CharSource > &source) :
	m_source(source), m_character(lcsm::support::CharSource::EndOfSource)
{
	nextChar();
}

lcsm::verilog::ModuleDeclareLexer::ModuleDeclareLexer(std::shared_ptr< lcsm::support::CharSource > &&source) :
	m_source(std::move(source)), m_character(lcsm::support::CharSource::EndOfSource)
{
	nextChar();
}

lcsm::verilog::ModuleDeclareLexer::ModuleDeclareLexer(lcsm::verilog::ModuleDeclareLexer &&other) noexcept :
	m_source(std::move(other.m_source)), m_token(std::move(other.m_token)), m_character(other.m_character),
	m_previousTokens(std::move(other.m_previousTokens))
{
}

lcsm::verilog::ModuleDeclareLexer &lcsm::verilog::ModuleDeclareLexer::operator=(lcsm::verilog::ModuleDeclareLexer &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::verilog::ModuleDeclareLexer >(this, std::move(other));
}

void lcsm::verilog::ModuleDeclareLexer::swap(lcsm::verilog::ModuleDeclareLexer &other)
{
	std::swap(m_source, other.m_source);
	std::swap(m_token, other.m_token);
	std::swap(m_character, other.m_character);
	std::swap(m_previousTokens, other.m_previousTokens);
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

static inline bool isBlank(char C) noexcept
{
	return std::isspace(C);
}

static inline bool isValid(char C) noexcept
{
	return std::isalnum(C) || C == '_';
}

static void buildIntegerOrIdentifier(std::string &builder, lcsm::verilog::ModuleDeclareToken &token)
{
	// Check, if built string is actually integer.
	// Otherwise, it's identifier.
	if (IsNumber(builder))
	{
		const int i = std::stoi(builder);
		token.setToken(i);
	}
	else
	{
		token.setToken(std::move(builder));
	}
}

const lcsm::verilog::ModuleDeclareToken &lcsm::verilog::ModuleDeclareLexer::nextToken()
{
	// Take previous token, if there is one.
	if (!m_previousTokens.empty())
	{
		m_token = m_previousTokens.front();
		m_previousTokens.pop_front();
		return m_token;
	}

	// Builder for long strings.
	std::string builder1;

	// Skip all blank chars.
	skipBlanks();

	// Check, if this character is just single kind.
	lcsm::verilog::ModuleDeclareKind kind1 = lcsm::verilog::IsKeyword(m_character);
	if (kind1 >= 0)
	{
		m_token.setToken(kind1);
		nextChar();
		goto l_finish;
	}

	// Check, if EOF.
	if (m_character == lcsm::support::CharSource::EndOfSource)
	{
		m_token.setEof();
		goto l_finish;
	}

	// Continue parsing to blank character (non valid).
	buildString(builder1);

	// Check, if this built string is start of some keyword.
	if (lcsm::verilog::IsStartOfKeyword(builder1))
	{
		// Token for future.
		lcsm::verilog::ModuleDeclareToken nextToken;

		// Builder for second part of keyword.
		std::string builder2;

		// Skip all blank chars..
		skipBlanks();

		// Check, if this character is just single kind, then, this is operator and then builder2 - is separated token.
		lcsm::verilog::ModuleDeclareKind kind2 = lcsm::verilog::IsKeyword(m_character);
		if (kind2 >= 0)
		{
			nextToken.setToken(kind2);
			nextChar();
			// Save this token to previous for next iteration for parser.
			m_previousTokens.push_back(std::move(nextToken));
			// Return to parsing builder1.
			goto l_continue;
		}

		// Check, if EOF.
		if (m_character == lcsm::support::CharSource::EndOfSource)
		{
			nextToken.setEof();
			// Save this token to previous for next iteration for parser.
			m_previousTokens.push_back(std::move(nextToken));
			// Return to parsing builder1.
			goto l_continue;
		}

		// Continue parsing to blank character (non valid).
		buildString(builder2);

		// Finalize.
		const std::string built = builder1 + ' ' + builder2;

		// Check, if keyword. Otherwise, place this token as previous.
		kind2 = lcsm::verilog::IsKeyword(built);
		if (kind2 >= 0)
		{
			m_token.setToken(kind2);
			goto l_finish;
		}
		else
		{
			// Check, if new built string is keyword.
			// Otherwise, build as integer or identifier.
			const lcsm::verilog::ModuleDeclareKind newKind = lcsm::verilog::IsKeyword(builder2);
			if (newKind >= 0)
			{
				nextToken.setToken(newKind);
			}
			else
			{
				// Build as integer or identifier.
				buildIntegerOrIdentifier(builder2, nextToken);
			}

			// Save this token to previous for next iteration for parser.
			m_previousTokens.push_back(std::move(nextToken));
		}
	}

l_continue:
	// Check, if this built string is keyword.
	kind1 = lcsm::verilog::IsKeyword(builder1);
	if (kind1 >= 0)
	{
		m_token.setToken(kind1);
		goto l_finish;
	}

	// Build as integer or identifier.
	buildIntegerOrIdentifier(builder1, m_token);

l_finish:
	return m_token;
}

const lcsm::verilog::ModuleDeclareToken &lcsm::verilog::ModuleDeclareLexer::token() const noexcept
{
	return m_token;
}

void lcsm::verilog::ModuleDeclareLexer::backToken()
{
	m_previousTokens.push_front(std::move(m_token));
}

void lcsm::verilog::ModuleDeclareLexer::nextChar()
{
	m_character = m_source->next();
}

void lcsm::verilog::ModuleDeclareLexer::skipBlanks()
{
	while (isBlank(m_character))
	{
		nextChar();
	}
}

void lcsm::verilog::ModuleDeclareLexer::buildString(std::string &builder)
{
	while (isValid(m_character))
	{
		builder.push_back(m_character);
		nextChar();
	}
}
