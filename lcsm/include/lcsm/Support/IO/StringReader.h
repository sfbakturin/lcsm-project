#ifndef LCSM_SUPPORT_IO_STRINGREADER_H
#define LCSM_SUPPORT_IO_STRINGREADER_H

#include <lcsm/Support/IO/Reader.h>
#include <lcsm/lcsmconfig.h>

#include <cstddef>
#include <string>

namespace lcsm
{
	namespace support
	{
		class LCSM_API StringReader : public Reader
		{
		  public:
			StringReader(const std::string &source);
			StringReader(std::string &&source) noexcept;

			StringReader(const StringReader &other);
			StringReader(StringReader &&other) noexcept;

			StringReader &operator=(const StringReader &other);
			StringReader &operator=(StringReader &&other) noexcept;

			void swap(StringReader &other) noexcept;

			virtual bool hasNext() override final;
			virtual char next() override final;

		  private:
			std::string m_source;
			std::size_t m_index;
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_IO_STRINGREADER_H */
