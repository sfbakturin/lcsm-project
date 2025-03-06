#ifndef LCSM_SUPPORT_PARSER_CFILESOURCE_H
#define LCSM_SUPPORT_PARSER_CFILESOURCE_H

#include <lcsm/Support/Parser/CharSource.h>

#include <cstdio>

namespace lcsm
{
	namespace support
	{
		class CFileSource : public CharSource
		{
		  public:
			CFileSource(const char *filename) noexcept;
			CFileSource(std::FILE *source) noexcept;

			CFileSource(const CFileSource &other) = delete;
			CFileSource(CFileSource &&other) noexcept;

			CFileSource &operator=(const CFileSource &other) = delete;
			CFileSource &operator=(CFileSource &&other) noexcept;

			void swap(CFileSource &other);

			virtual bool hasNext() override final;
			virtual char next() override final;

		  private:
			std::FILE *m_source;
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_PARSER_CFILESOURCE_H */
