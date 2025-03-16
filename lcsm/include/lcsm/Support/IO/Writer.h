#ifndef LCSM_SUPPORT_IO_WRITER_H
#define LCSM_SUPPORT_IO_WRITER_H

#include <initializer_list>

#include <cstddef>
#include <string>

namespace lcsm
{
	namespace support
	{
		class Writer
		{
		  public:
			Writer() noexcept = default;
			virtual ~Writer() noexcept = default;

			virtual void write(char c) = 0;
			virtual void writeln(char c);

			virtual void write(char c, std::size_t repeat);
			virtual void writeln(char c, std::size_t repeat);

			virtual void write(const std::string &str) = 0;
			virtual void writeln(const std::string &str);
			virtual void writeEscaped(const std::string &str);
			virtual void writelnEscaped(const std::string &str);

			virtual void write(const char *str) = 0;
			virtual void writeln(const char *str);

			virtual void write(std::initializer_list< const char * > strs);
			virtual void writeln(std::initializer_list< const char * > strs);
			virtual void write(std::initializer_list< const char * > strs, char sep);
			virtual void writeln(std::initializer_list< const char * > strs, char sep);
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_IO_WRITER_H */
