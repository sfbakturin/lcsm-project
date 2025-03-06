#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/Parser/CFileSource.h>
#include <lcsm/Support/Parser/CharSource.h>

#include <cstdio>
#include <memory>
#include <utility>

lcsm::support::CFileSource::CFileSource(const char *filename) noexcept : m_source(std::fopen(filename, "r")) {}

lcsm::support::CFileSource::CFileSource(std::FILE *source) noexcept : m_source(source) {}

lcsm::support::CFileSource::CFileSource(lcsm::support::CFileSource &&other) noexcept : m_source(other.m_source)
{
	other.m_source = nullptr;
}

lcsm::support::CFileSource &lcsm::support::CFileSource::operator=(lcsm::support::CFileSource &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::support::CFileSource >(this, std::move(other));
}

void lcsm::support::CFileSource::swap(lcsm::support::CFileSource &other)
{
	std::swap(m_source, other.m_source);
}

bool lcsm::support::CFileSource::hasNext()
{
	// TODO: Create buffer reader, when this calls:
	//       1. Shrink buffer.
	//       2. if shrinked buffer is not empty, then return true.
	//       3. Otherwise, read to buffer and got to 1.
	return true;
}

char lcsm::support::CFileSource::next()
{
	// TODO: See CFileSource::hasNext.

	// If source is NULL, then return EOF.
	if (!hasNext())
	{
		return lcsm::support::CharSource::EndOfSource;
	}

	// Read character.
	char ch = lcsm::support::CharSource::EndOfSource;
	std::fscanf(m_source, "%c", std::addressof(ch));
	return ch;
}
