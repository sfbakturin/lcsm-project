#ifndef LCSM_SUPPORT_ALGORITHM_HPP
#define LCSM_SUPPORT_ALGORITHM_HPP

#include <lcsm/Support/Meta.hpp>
#include <lcsm/lcsmconfig.h>

#include <algorithm>
#include <memory>
#include <utility>

namespace lcsm
{
	namespace support
	{
		template< typename Class >
		LCSM_API inline Class &
			CopyAssign(Class *that, const Class &other) noexcept(std::is_nothrow_copy_constructible< Class >::value)
		{
			if (that != std::addressof(other))
				Class(other).swap(*that);
			return *that;
		}

		template< typename Class >
		LCSM_API inline Class &MoveAssign(Class *that, Class &&other) noexcept(std::is_nothrow_move_constructible< Class >::value)
		{
			if (that != std::addressof(other))
				Class(std::move(other)).swap(*that);
			return *that;
		}
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_ALGORITHM_HPP */
