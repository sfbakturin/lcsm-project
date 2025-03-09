#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Support/Parser/FileSource.h>

#include <fstream>
#include <ios>
#include <string>
#include <utility>

lcsm::support::FileSource::FileSource(const char *filename) :
	m_source(filename), m_character(lcsm::support::CharSource::EndOfSource)
{
}

lcsm::support::FileSource::FileSource(const std::string &filename) :
	m_source(filename), m_character(lcsm::support::CharSource::EndOfSource)
{
}

lcsm::support::FileSource::FileSource(FileSource &&other) :
	m_source(std::move(other.m_source)), m_character(lcsm::support::CharSource::EndOfSource)
{
}

lcsm::support::FileSource &lcsm::support::FileSource::operator=(lcsm::support::FileSource &&other)
{
	return lcsm::support::MoveAssign< lcsm::support::FileSource >(this, std::move(other));
}

void lcsm::support::FileSource::swap(lcsm::support::FileSource &other)
{
	std::swap(m_source, other.m_source);
}

bool lcsm::support::FileSource::hasNext()
{
	// Check if source is open.
	if (!m_source.is_open())
	{
		return false;
	}

	// Read to character, if Ok, then m_character is filled and we can use it.
	if (m_source >> std::noskipws >> m_character)
	{
		return true;
	}

	return false;
}

char lcsm::support::FileSource::next()
{
	return (hasNext() ? m_character : lcsm::support::CharSource::EndOfSource);
}
