#include <lcsm/Model/File/Kind.h>
#include <lcsm/Model/File/Token.h>
#include <lcsm/Support/Algorithm.hpp>

#include <cstdint>
#include <string>
#include <utility>

lcsm::model::LCSMFileToken::LCSMFileToken() : m_kind(lcsm::model::LCSMFileKind::UnknownKind), m_i(0) {}

lcsm::model::LCSMFileToken::LCSMFileToken(lcsm::model::LCSMFileKind kind) : m_kind(kind), m_i(0) {}

lcsm::model::LCSMFileToken::LCSMFileToken(unsigned long long i) :
	m_kind(lcsm::model::LCSMFileKind::IntegerToken), m_i(i)
{
}

lcsm::model::LCSMFileToken::LCSMFileToken(const std::string &s, bool isIdentifier) :
	m_kind(isIdentifier ? lcsm::model::LCSMFileKind::IdentifierToken : lcsm::model::LCSMFileKind::StringToken), m_i(0), m_s(s)
{
}

lcsm::model::LCSMFileToken::LCSMFileToken(std::string &&s, bool isIdentifier) noexcept :
	m_kind(isIdentifier ? lcsm::model::LCSMFileKind::IdentifierToken : lcsm::model::LCSMFileKind::StringToken), m_i(0),
	m_s(std::move(s))
{
}

lcsm::model::LCSMFileToken::LCSMFileToken(const lcsm::model::LCSMFileToken &other) :
	m_kind(other.m_kind), m_i(other.m_i), m_s(other.m_s)
{
}

lcsm::model::LCSMFileToken::LCSMFileToken(lcsm::model::LCSMFileToken &&other) noexcept :
	m_kind(other.m_kind), m_i(other.m_i), m_s(std::move(other.m_s))
{
}

lcsm::model::LCSMFileToken &lcsm::model::LCSMFileToken::operator=(const lcsm::model::LCSMFileToken &other)
{
	return lcsm::support::CopyAssign< lcsm::model::LCSMFileToken >(this, other);
}

lcsm::model::LCSMFileToken &lcsm::model::LCSMFileToken::operator=(lcsm::model::LCSMFileToken &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::model::LCSMFileToken >(this, std::move(other));
}

bool lcsm::model::LCSMFileToken::operator==(lcsm::model::LCSMFileKind kind) const noexcept
{
	return m_kind == kind;
}

bool lcsm::model::LCSMFileToken::operator!=(lcsm::model::LCSMFileKind kind) const noexcept
{
	return m_kind != kind;
}

void lcsm::model::LCSMFileToken::swap(lcsm::model::LCSMFileToken &other) noexcept
{
	std::swap(m_kind, other.m_kind);
	std::swap(m_i, other.m_i);
	std::swap(m_s, other.m_s);
}

lcsm::model::LCSMFileKind lcsm::model::LCSMFileToken::kind() noexcept
{
	return m_kind;
}

void lcsm::model::LCSMFileToken::setToken(lcsm::model::LCSMFileKind kind) noexcept
{
	m_kind = kind;
	m_i = 0;
	m_s.clear();
}

void lcsm::model::LCSMFileToken::setToken(unsigned long long i) noexcept
{
	m_kind = lcsm::model::LCSMFileKind::IntegerToken;
	m_i = i;
	m_s.clear();
}

void lcsm::model::LCSMFileToken::setToken(const std::string &s, bool isIdentifier)
{
	m_kind = (isIdentifier ? lcsm::model::LCSMFileKind::IdentifierToken : lcsm::model::LCSMFileKind::StringToken);
	m_i = 0;
	m_s = s;
}

void lcsm::model::LCSMFileToken::setToken(std::string &&s, bool isIdentifier) noexcept
{
	m_kind = (isIdentifier ? lcsm::model::LCSMFileKind::IdentifierToken : lcsm::model::LCSMFileKind::StringToken);
	m_i = 0;
	m_s = std::move(s);
}

void lcsm::model::LCSMFileToken::setEof() noexcept
{
	setToken(lcsm::model::LCSMFileKind::EndOfFileToken);
}

bool lcsm::model::LCSMFileToken::isKeyword() const noexcept
{
	const std::int8_t tok = static_cast< std::int8_t >(m_kind);
	return lcsm::model::KindKeywordStart <= tok && tok <= lcsm::model::KindKeywordEnd;
}

bool lcsm::model::LCSMFileToken::isInteger() const noexcept
{
	return m_kind == lcsm::model::LCSMFileKind::IntegerToken;
}

bool lcsm::model::LCSMFileToken::isIdentifier() const noexcept
{
	return m_kind == lcsm::model::LCSMFileKind::IdentifierToken;
}

bool lcsm::model::LCSMFileToken::isString() const noexcept
{
	return m_kind == lcsm::model::LCSMFileKind::StringToken;
}

bool lcsm::model::LCSMFileToken::isEof() const noexcept
{
	return m_kind == lcsm::model::LCSMFileKind::EndOfFileToken;
}

unsigned long long lcsm::model::LCSMFileToken::asInteger() const noexcept
{
	return m_i;
}

const std::string &lcsm::model::LCSMFileToken::asIdentifier() const noexcept
{
	return m_s;
}

const std::string &lcsm::model::LCSMFileToken::asString() const noexcept
{
	return m_s;
}
