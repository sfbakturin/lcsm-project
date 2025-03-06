#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/Parser/CStringSource.h>
#include <lcsm/Support/Parser/CharSource.h>

#include <cstring>
#include <utility>

lcsm::support::CStringSource::CStringSource(const char *source) noexcept : m_source(source), m_index(0), m_length(0)
{
	if (m_source != nullptr)
	{
		m_length = std::strlen(m_source);
	}
}

lcsm::support::CStringSource::CStringSource(const CStringSource &other) noexcept :
	m_source(other.m_source), m_index(other.m_index), m_length(other.m_length)
{
}

lcsm::support::CStringSource::CStringSource(CStringSource &&other) noexcept :
	m_source(other.m_source), m_index(other.m_index), m_length(other.m_length)
{
	other.m_source = nullptr;
	other.m_index = 0;
	other.m_length = 0;
}

lcsm::support::CStringSource &lcsm::support::CStringSource::operator=(const lcsm::support::CStringSource &other) noexcept
{
	return lcsm::support::CopyAssign< lcsm::support::CStringSource >(this, other);
}

lcsm::support::CStringSource &lcsm::support::CStringSource::operator=(lcsm::support::CStringSource &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::support::CStringSource >(this, std::move(other));
}

void lcsm::support::CStringSource::swap(lcsm::support::CStringSource &other) noexcept
{
	std::swap(m_source, other.m_source);
	std::swap(m_index, other.m_index);
	std::swap(m_length, other.m_length);
}

bool lcsm::support::CStringSource::hasNext()
{
	return m_source != nullptr && m_index < m_length;
}

char lcsm::support::CStringSource::next()
{
	return (hasNext() ? m_source[m_index++] : lcsm::support::CharSource::EndOfSource);
}
