#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/IO/CStringReader.h>
#include <lcsm/Support/IO/Reader.h>

#include <cstring>
#include <utility>

lcsm::support::CStringReader::CStringReader(const char *source) noexcept : m_source(source), m_index(0), m_length(0)
{
	if (m_source != nullptr)
	{
		m_length = std::strlen(m_source);
	}
}

lcsm::support::CStringReader::CStringReader(const CStringReader &other) noexcept :
	m_source(other.m_source), m_index(other.m_index), m_length(other.m_length)
{
}

lcsm::support::CStringReader::CStringReader(CStringReader &&other) noexcept :
	m_source(other.m_source), m_index(other.m_index), m_length(other.m_length)
{
	other.m_source = nullptr;
	other.m_index = 0;
	other.m_length = 0;
}

lcsm::support::CStringReader &lcsm::support::CStringReader::operator=(const lcsm::support::CStringReader &other) noexcept
{
	return lcsm::support::CopyAssign< lcsm::support::CStringReader >(this, other);
}

lcsm::support::CStringReader &lcsm::support::CStringReader::operator=(lcsm::support::CStringReader &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::support::CStringReader >(this, std::move(other));
}

void lcsm::support::CStringReader::swap(lcsm::support::CStringReader &other) noexcept
{
	std::swap(m_source, other.m_source);
	std::swap(m_index, other.m_index);
	std::swap(m_length, other.m_length);
}

bool lcsm::support::CStringReader::hasNext()
{
	return m_source != nullptr && m_index < m_length;
}

char lcsm::support::CStringReader::next()
{
	return (hasNext() ? m_source[m_index++] : lcsm::support::Reader::EndOfSource);
}
