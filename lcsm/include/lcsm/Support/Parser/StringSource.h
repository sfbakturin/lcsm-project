#ifndef LCSM_SUPPORT_PARSER_STRINGSOURCE_H
#define LCSM_SUPPORT_PARSER_STRINGSOURCE_H

#include <lcsm/Support/Parser/CharSource.h>

#include <cstddef>
#include <string>

namespace lcsm
{
	namespace support
	{
		class StringSource : public CharSource
		{
		  public:
			StringSource(const std::string &source);
			StringSource(std::string &&source) noexcept;

			StringSource(const StringSource &other);
			StringSource(StringSource &&other) noexcept;

			StringSource &operator=(const StringSource &other);
			StringSource &operator=(StringSource &&other) noexcept;

			void swap(StringSource &other) noexcept;

			virtual bool hasNext() override final;
			virtual char next() override final;

		  private:
			std::string m_source;
			std::size_t m_index;
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_PARSER_STRINGSOURCE_H */
