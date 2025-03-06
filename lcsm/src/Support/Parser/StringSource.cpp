#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Support/Parser/StringSource.h>

#include <string>
#include <utility>

lcsm::support::StringSource::StringSource(const std::string &source) : m_source(source), m_index(0) {}

lcsm::support::StringSource::StringSource(std::string &&source) noexcept : m_source(std::move(source)), m_index(0) {}

lcsm::support::StringSource::StringSource(const lcsm::support::StringSource &other) :
	m_source(other.m_source), m_index(other.m_index)
{
}

lcsm::support::StringSource::StringSource(lcsm::support::StringSource &&other) noexcept :
	m_source(std::move(other.m_source)), m_index(other.m_index)
{
}

lcsm::support::StringSource &lcsm::support::StringSource::operator=(const lcsm::support::StringSource &other)
{
	return lcsm::support::CopyAssign< lcsm::support::StringSource >(this, other);
}

lcsm::support::StringSource &lcsm::support::StringSource::operator=(lcsm::support::StringSource &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::support::StringSource >(this, std::move(other));
}

void lcsm::support::StringSource::swap(lcsm::support::StringSource &other) noexcept
{
	std::swap(m_source, other.m_source);
	std::swap(m_index, other.m_index);
}

bool lcsm::support::StringSource::hasNext()
{
	return m_index < m_source.size();
}

char lcsm::support::StringSource::next()
{
	return (hasNext() ? m_source[m_index++] : lcsm::support::CharSource::EndOfSource);
}
