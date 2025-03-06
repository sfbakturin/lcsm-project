#ifndef LCSM_SUPPORT_PARSER_FILESOURCE_H
#define LCSM_SUPPORT_PARSER_FILESOURCE_H

#include <lcsm/Support/Parser/CharSource.h>

#include <fstream>
#include <string>

namespace lcsm
{
	namespace support
	{
		class FileSource : public CharSource
		{
		  public:
			FileSource(const char *filename);
			FileSource(const std::string &filename);
			FileSource(std::ifstream &&source);

			FileSource(const FileSource &other) = delete;
			FileSource(FileSource &&other);

			FileSource &operator=(const FileSource &other) = delete;
			FileSource &operator=(FileSource &&other);

			void swap(FileSource &other);

			virtual bool hasNext() override final;
			virtual char next() override final;

		  private:
			std::ifstream m_source;
			char m_character;
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_PARSER_FILESOURCE_H */
