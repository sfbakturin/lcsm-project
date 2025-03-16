#ifndef LCSM_SUPPORT_IO_CSTRINGREADER_H
#define LCSM_SUPPORT_IO_CSTRINGREADER_H

#include <lcsm/Support/IO/Reader.h>

#include <cstddef>

namespace lcsm
{
	namespace support
	{
		class CStringReader : public Reader
		{
		  public:
			CStringReader(const char *source) noexcept;

			CStringReader(const CStringReader &other) noexcept;
			CStringReader(CStringReader &&other) noexcept;

			CStringReader &operator=(const CStringReader &other) noexcept;
			CStringReader &operator=(CStringReader &&other) noexcept;

			void swap(CStringReader &other) noexcept;

			virtual bool hasNext() override final;
			virtual char next() override final;

		  private:
			const char *m_source;
			std::size_t m_index;
			std::size_t m_length;
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_IO_CSTRINGREADER_H */
