#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/IO/Reader.h>
#include <lcsm/Support/IO/StringReader.h>

#include <string>
#include <utility>

lcsm::support::StringReader::StringReader(const std::string &source) : m_source(source), m_index(0) {}

lcsm::support::StringReader::StringReader(std::string &&source) noexcept : m_source(std::move(source)), m_index(0) {}

lcsm::support::StringReader::StringReader(const lcsm::support::StringReader &other) :
	m_source(other.m_source), m_index(other.m_index)
{
}

lcsm::support::StringReader::StringReader(lcsm::support::StringReader &&other) noexcept :
	m_source(std::move(other.m_source)), m_index(other.m_index)
{
}

lcsm::support::StringReader &lcsm::support::StringReader::operator=(const lcsm::support::StringReader &other)
{
	return lcsm::support::CopyAssign< lcsm::support::StringReader >(this, other);
}

lcsm::support::StringReader &lcsm::support::StringReader::operator=(lcsm::support::StringReader &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::support::StringReader >(this, std::move(other));
}

void lcsm::support::StringReader::swap(lcsm::support::StringReader &other) noexcept
{
	std::swap(m_source, other.m_source);
	std::swap(m_index, other.m_index);
}

bool lcsm::support::StringReader::hasNext()
{
	return m_index < m_source.size();
}

char lcsm::support::StringReader::next()
{
	return (hasNext() ? m_source[m_index++] : lcsm::support::Reader::EndOfSource);
}
