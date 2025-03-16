#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/IO/FileReader.h>
#include <lcsm/Support/IO/Reader.h>

#include <fstream>
#include <ios>
#include <string>
#include <utility>

lcsm::support::FileReader::FileReader(const char *filename) :
	m_source(filename), m_character(lcsm::support::Reader::EndOfSource)
{
}

lcsm::support::FileReader::FileReader(const std::string &filename) :
	m_source(filename), m_character(lcsm::support::Reader::EndOfSource)
{
}

lcsm::support::FileReader::FileReader(lcsm::support::FileReader &&other) :
	m_source(std::move(other.m_source)), m_character(lcsm::support::Reader::EndOfSource)
{
}

lcsm::support::FileReader &lcsm::support::FileReader::operator=(lcsm::support::FileReader &&other)
{
	return lcsm::support::MoveAssign< lcsm::support::FileReader >(this, std::move(other));
}

void lcsm::support::FileReader::swap(lcsm::support::FileReader &other)
{
	std::swap(m_source, other.m_source);
}

bool lcsm::support::FileReader::hasNext()
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

char lcsm::support::FileReader::next()
{
	return (hasNext() ? m_character : lcsm::support::Reader::EndOfSource);
}
