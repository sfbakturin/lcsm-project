#include <initializer_list>
#include <lcsm/Support/IO/Writer.h>

#include <cstddef>
#include <string>

void lcsm::support::Writer::write(char c, std::size_t repeat)
{
	for (std::size_t i = 0; i < repeat; i++)
	{
		write(c);
	}
}

void lcsm::support::Writer::writeln(char c, std::size_t repeat)
{
	write(c, repeat);
	writeln('\n');
}

void lcsm::support::Writer::writeln(char c)
{
	write(c);
	write('\n');
}

void lcsm::support::Writer::writeln(const std::string &str)
{
	write(str);
	write('\n');
}

void lcsm::support::Writer::writeEscaped(const std::string &str)
{
	write('\"');
	for (char c : str)
	{
		switch (c)
		{
		case '\n':
		{
			write("\\n");
			break;
		}
		case '\r':
		{
			write("\\r");
			break;
		}
		case '\t':
		{
			write("\\t");
			break;
		}
		case '\\':
		{
			write("\\\\");
			break;
		}
		default:
		{
			write(c);
			break;
		}
		}
	}
	write('\"');
}

void lcsm::support::Writer::writelnEscaped(const std::string &str)
{
	writeEscaped(str);
	write('\n');
}

void lcsm::support::Writer::writeln(const char *str)
{
	write(str);
	write('\n');
}

void lcsm::support::Writer::write(std::initializer_list< const char * > strs)
{
	for (const char *str : strs)
	{
		write(str);
	}
}

void lcsm::support::Writer::writeln(std::initializer_list< const char * > strs)
{
	write(strs);
	write('\n');
}

void lcsm::support::Writer::write(std::initializer_list< const char * > strs, char sep)
{
	bool needsSep = false;
	for (const char *str : strs)
	{
		if (needsSep)
		{
			write(sep);
		}
		write(str);
		needsSep = true;
	}
}

void lcsm::support::Writer::writeln(std::initializer_list< const char * > strs, char sep)
{
	write(strs, sep);
	write('\n');
}
