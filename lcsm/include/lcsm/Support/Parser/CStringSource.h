#ifndef LCSM_SUPPORT_PARSER_CSTRINGSOURCE_H
#define LCSM_SUPPORT_PARSER_CSTRINGSOURCE_H

#include <lcsm/Support/Parser/CharSource.h>

#include <cstddef>

namespace lcsm
{
	namespace support
	{
		class CStringSource : public CharSource
		{
		  public:
			CStringSource(const char *source) noexcept;

			CStringSource(const CStringSource &other) noexcept;
			CStringSource(CStringSource &&other) noexcept;

			CStringSource &operator=(const CStringSource &other) noexcept;
			CStringSource &operator=(CStringSource &&other) noexcept;

			void swap(CStringSource &other) noexcept;

			virtual bool hasNext() override final;
			virtual char next() override final;

		  private:
			const char *m_source;
			std::size_t m_index;
			std::size_t m_length;
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_PARSER_CSTRINGSOURCE_H */
