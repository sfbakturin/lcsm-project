#ifndef LCSM_SUPPORT_ALGORITHM_HPP
#define LCSM_SUPPORT_ALGORITHM_HPP

#include <lcsm/Support/Meta.hpp>

#include <algorithm>
#include <memory>
#include <utility>

namespace lcsm
{
	namespace support
	{
		template< typename Class >
		inline Class &CopyAssign(Class *that, const Class &other) noexcept(IsNothrowCopyAssign< Class >())
		{
			if (that != std::addressof(other))
				Class(other).swap(*that);
			return *that;
		}

		template< typename Class >
		inline Class &MoveAssign(Class *that, Class &&other) noexcept(IsNothrowMoveAssign< Class >())
		{
			if (that != std::addressof(other))
				Class(std::move(other)).swap(*that);
			return *that;
		}
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_ALGORITHM_HPP */
