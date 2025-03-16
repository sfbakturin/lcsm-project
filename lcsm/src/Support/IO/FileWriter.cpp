#include <lcsm/Support/IO/FileWriter.h>

#include <fstream>
#include <string>

lcsm::support::FileWriter::FileWriter(const std::string &filename) : m_stream(filename) {}

lcsm::support::FileWriter::FileWriter(const char *filename) : m_stream(filename) {}

void lcsm::support::FileWriter::write(char c)
{
	m_stream << c;
}

void lcsm::support::FileWriter::write(const std::string &str)
{
	m_stream << str;
}

void lcsm::support::FileWriter::write(const char *str)
{
	m_stream << str;
}
