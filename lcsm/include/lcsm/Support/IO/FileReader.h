#ifndef LCSM_SUPPORT_IO_FILEREADER_H
#define LCSM_SUPPORT_IO_FILEREADER_H

#include <lcsm/Support/IO/Reader.h>
#include <lcsm/lcsmconfig.h>

#include <fstream>
#include <string>

namespace lcsm
{
	namespace support
	{
		class LCSM_API FileReader : public Reader
		{
		  public:
			FileReader(const char *filename);
			FileReader(const std::string &filename);

			FileReader(const FileReader &other) = delete;
			FileReader(FileReader &&other);

			FileReader &operator=(const FileReader &other) = delete;
			FileReader &operator=(FileReader &&other);

			void swap(FileReader &other);

			virtual bool hasNext() override final;
			virtual char next() override final;

		  private:
			std::ifstream m_source;
			char m_character;
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_IO_FILEREADER_H */
