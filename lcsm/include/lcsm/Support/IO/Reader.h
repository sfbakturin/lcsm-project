#ifndef LCSM_SUPPORT_IO_READER_H
#define LCSM_SUPPORT_IO_READER_H

#include <lcsm/lcsmconfig.h>

namespace lcsm
{
	namespace support
	{
		class LCSM_API Reader
		{
		  public:
			Reader() noexcept = default;
			virtual ~Reader() noexcept = default;

			virtual bool hasNext() = 0;
			virtual char next() = 0;

			static constexpr char EndOfSource = '\0';
		};
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_IO_READER_H */
