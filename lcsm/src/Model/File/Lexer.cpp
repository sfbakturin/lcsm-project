#include <lcsm/Model/File/Kind.h>
#include <lcsm/Model/File/Lexer.h>
#include <lcsm/Model/File/Token.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/IO/Reader.h>

#include <deque>
#include <memory>
#include <string>
#include <utility>

lcsm::model::LCSMFileLexer::LCSMFileLexer(const std::shared_ptr< lcsm::support::Reader > &source) : m_source(source)
{
	nextChar();
}

lcsm::model::LCSMFileLexer::LCSMFileLexer(std::shared_ptr< lcsm::support::Reader > &&source) :
	m_source(std::move(source))
{
	nextChar();
}

lcsm::model::LCSMFileLexer::LCSMFileLexer(lcsm::model::LCSMFileLexer &&other) noexcept :
	m_source(std::move(other.m_source)), m_token(std::move(other.m_token)), m_character(other.m_character),
	m_previousTokens(std::move(other.m_previousTokens))
{
}

lcsm::model::LCSMFileLexer &lcsm::model::LCSMFileLexer::operator=(lcsm::model::LCSMFileLexer &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::model::LCSMFileLexer >(this, std::move(other));
}

void lcsm::model::LCSMFileLexer::swap(lcsm::model::LCSMFileLexer &other)
{
	std::swap(m_source, other.m_source);
	std::swap(m_token, other.m_token);
	std::swap(m_character, other.m_character);
	std::swap(m_previousTokens, other.m_previousTokens);
}

// static bool IsNumber(const std::string &s) noexcept
// {
// 	for (char c : s)
// 	{
// 		if (!('0' <= c && c <= '9'))
// 		{
// 			return false;
// 		}
// 	}
// 	return true;
// }

static inline bool isBlank(char C) noexcept
{
	return std::isspace(C);
}

static inline bool isValid(char C) noexcept
{
	return std::isalnum(C) || C == '_';
}

// static void buildIntegerOrIdentifier(std::string &builder, lcsm::model::LCSMFileToken &token)
// {
// 	// Check, if built string is actually integer.
// 	// Otherwise, it's identifier.
// 	if (IsNumber(builder))
// 	{
// 		const int i = std::stoi(builder);
// 		token.setToken(i);
// 	}
// 	else
// 	{
// 		token.setToken(std::move(builder));
// 	}
// }

const lcsm::model::LCSMFileToken &lcsm::model::LCSMFileLexer::nextToken()
{
	// Take previous token, if there is one.
	if (!m_previousTokens.empty())
	{
		m_token = m_previousTokens.front();
		m_previousTokens.pop_front();
		return m_token;
	}

	// Builder for long strings.
	std::string builder;

	// Skip all blank chars.
	skipBlanks();

	// Check, if this character is just single kind.
	lcsm::model::LCSMFileKind kind = lcsm::model::IsKeyword(m_character);
	if (kind >= 0)
	{
		m_token.setToken(kind);
		nextChar();
		goto l_finish;
	}

	// Check, if EOF.
	if (m_character == lcsm::support::Reader::EndOfSource)
	{
		m_token.setEof();
		goto l_finish;
	}

	// Continue parsing to blank character (non valid).
	buildString(builder);

	// Check, if built string is keyword.
	kind = lcsm::model::IsKeyword(builder);
	if (kind >= 0)
	{
		m_token.setToken(kind);
		goto l_finish;
	}

	// Otherwise, build this string as integer or identifier.
	// buildIntegerOrIdentifier(builder, m_token);

l_finish:
	return m_token;
}

const lcsm::model::LCSMFileToken &lcsm::model::LCSMFileLexer::token() const noexcept
{
	return m_token;
}

void lcsm::model::LCSMFileLexer::backToken()
{
	m_previousTokens.push_front(std::move(m_token));
}

void lcsm::model::LCSMFileLexer::nextChar()
{
	m_character = m_source->next();
}

void lcsm::model::LCSMFileLexer::skipBlanks()
{
	while (isBlank(m_character))
	{
		nextChar();
	}
}

void lcsm::model::LCSMFileLexer::buildString(std::string &builder)
{
	while (isValid(m_character))
	{
		builder.push_back(m_character);
		nextChar();
	}
}
