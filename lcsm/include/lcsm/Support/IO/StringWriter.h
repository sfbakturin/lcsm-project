#ifndef LCSM_SUPPORT_IO_STRINGWRITER_H
#define LCSM_SUPPORT_IO_STRINGWRITER_H

#include <lcsm/Support/IO/Writer.h>

#include <string>

namespace lcsm
{
	namespace support
	{
		class StringWriter : public Writer
		{
		  public:
			StringWriter() = default;

			virtual void write(char c) override final;
			virtual void write(const std::string &str) override final;
			virtual void write(const char *str) override final;

			const std::string &toString() const noexcept;

		  private:
			std::string m_builder;
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_IO_STRINGWRITER_H */
