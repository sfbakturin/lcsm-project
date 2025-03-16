#ifndef LCSM_SUPPORT_IO_FILEWRITER_H
#define LCSM_SUPPORT_IO_FILEWRITER_H

#include <lcsm/Support/IO/Writer.h>

#include <fstream>
#include <string>

namespace lcsm
{
	namespace support
	{
		class FileWriter : public Writer
		{
		  public:
			FileWriter(const std::string &filename);
			FileWriter(const char *filename);

			virtual void write(char c) override final;
			virtual void write(const std::string &str) override final;
			virtual void write(const char *str) override final;

		  private:
			std::ofstream m_stream;
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_IO_FILEWRITER_H */
