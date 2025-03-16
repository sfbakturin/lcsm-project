#include <lcsm/Support/IO/StringWriter.h>
#include <lcsm/Support/IO/Writer.h>

#include <string>

void lcsm::support::StringWriter::write(char c)
{
	m_builder.push_back(c);
}

void lcsm::support::StringWriter::write(const std::string &str)
{
	m_builder.append(str);
}

void lcsm::support::StringWriter::write(const char *str)
{
	m_builder.append(str);
}

const std::string &lcsm::support::StringWriter::toString() const noexcept
{
	return m_builder;
}
