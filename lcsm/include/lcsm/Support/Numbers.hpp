#ifndef LCSM_SUPPORT_NUMBERS_HPP
#define LCSM_SUPPORT_NUMBERS_HPP

#include <type_traits>

#include <cstdint>

namespace lcsm
{
	namespace support
	{
		template< typename Integer, std::enable_if_t< std::is_integral< Integer >::value || std::is_convertible< Integer, std::int32_t >::value, bool > = true >
		int compare(Integer L, Integer R) noexcept
		{
			if (L < R)
				return 1;
			else if (L > R)
				return -1;
			else
				return 0;
		}
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_NUMBERS_HPP */
