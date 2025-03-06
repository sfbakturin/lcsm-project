#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Verilog/ModuleDeclare/Token.h>

#include <cstdint>
#include <string>
#include <utility>

lcsm::verilog::ModuleDeclareToken::ModuleDeclareToken() : m_kind(lcsm::verilog::ModuleDeclareKind::UnknownKind), m_i(0)
{
}

lcsm::verilog::ModuleDeclareToken::ModuleDeclareToken(lcsm::verilog::ModuleDeclareKind kind) : m_kind(kind), m_i(0) {}

lcsm::verilog::ModuleDeclareToken::ModuleDeclareToken(int i) :
	m_kind(lcsm::verilog::ModuleDeclareKind::TK_INTEGER), m_i(i)
{
}

lcsm::verilog::ModuleDeclareToken::ModuleDeclareToken(const std::string &s) :
	m_kind(lcsm::verilog::ModuleDeclareKind::TK_IDENTIFIER), m_s(s)
{
}

lcsm::verilog::ModuleDeclareToken::ModuleDeclareToken(std::string &&s) noexcept :
	m_kind(lcsm::verilog::ModuleDeclareKind::TK_IDENTIFIER), m_s(std::move(s))
{
}

lcsm::verilog::ModuleDeclareToken::ModuleDeclareToken(const lcsm::verilog::ModuleDeclareToken &other) :
	m_kind(other.m_kind), m_i(other.m_i), m_s(other.m_s)
{
}

lcsm::verilog::ModuleDeclareToken::ModuleDeclareToken(lcsm::verilog::ModuleDeclareToken &&other) noexcept :
	m_kind(other.m_kind), m_i(other.m_i), m_s(std::move(other.m_s))
{
}

lcsm::verilog::ModuleDeclareToken &lcsm::verilog::ModuleDeclareToken::operator=(const lcsm::verilog::ModuleDeclareToken &other)
{
	return lcsm::support::CopyAssign< lcsm::verilog::ModuleDeclareToken >(this, other);
}

lcsm::verilog::ModuleDeclareToken &lcsm::verilog::ModuleDeclareToken::operator=(lcsm::verilog::ModuleDeclareToken &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::verilog::ModuleDeclareToken >(this, std::move(other));
}

void lcsm::verilog::ModuleDeclareToken::swap(lcsm::verilog::ModuleDeclareToken &other) noexcept
{
	std::swap(m_kind, other.m_kind);
	std::swap(m_i, other.m_i);
	std::swap(m_s, other.m_s);
}

lcsm::verilog::ModuleDeclareKind lcsm::verilog::ModuleDeclareToken::kind() const noexcept
{
	return m_kind;
}

void lcsm::verilog::ModuleDeclareToken::setToken(lcsm::verilog::ModuleDeclareKind kind) noexcept
{
	m_kind = kind;
	m_i = 0;
	m_s.clear();
}

void lcsm::verilog::ModuleDeclareToken::setToken(int i) noexcept
{
	m_kind = lcsm::verilog::ModuleDeclareKind::TK_INTEGER;
	m_i = i;
	m_s.clear();
}

void lcsm::verilog::ModuleDeclareToken::setToken(const std::string &s)
{
	m_kind = lcsm::verilog::ModuleDeclareKind::TK_IDENTIFIER;
	m_i = 0;
	m_s = s;
}

void lcsm::verilog::ModuleDeclareToken::setToken(std::string &&s) noexcept
{
	m_kind = lcsm::verilog::ModuleDeclareKind::TK_IDENTIFIER;
	m_i = 0;
	m_s = std::move(s);
}

void lcsm::verilog::ModuleDeclareToken::setEof() noexcept
{
	m_kind = lcsm::verilog::ModuleDeclareKind::TK_EOF;
	m_i = 0;
	m_s.clear();
}

bool lcsm::verilog::ModuleDeclareToken::isKeyword() const noexcept
{
	const std::int8_t kind = static_cast< std::int8_t >(m_kind);
	return lcsm::verilog::KindKeywordStart <= kind && kind <= lcsm::verilog::KindKeywordEnd;
}

bool lcsm::verilog::ModuleDeclareToken::isInteger() const noexcept
{
	return m_kind == lcsm::verilog::ModuleDeclareKind::TK_INTEGER;
}

bool lcsm::verilog::ModuleDeclareToken::isIdentifier() const noexcept
{
	return m_kind == lcsm::verilog::ModuleDeclareKind::TK_IDENTIFIER;
}

bool lcsm::verilog::ModuleDeclareToken::isEof() const noexcept
{
	return m_kind == lcsm::verilog::ModuleDeclareKind::TK_EOF;
}

int lcsm::verilog::ModuleDeclareToken::asInteger() const noexcept
{
	return m_i;
}

const std::string &lcsm::verilog::ModuleDeclareToken::asIdentifier() const noexcept
{
	return m_s;
}
