#ifndef LCSM_SUPPORT_META_HPP
#define LCSM_SUPPORT_META_HPP

#include <type_traits>

namespace lcsm
{
	namespace support
	{
		template< typename Class >
		constexpr inline bool IsNothrowCopyAssign() noexcept
		{
			return std::is_nothrow_copy_assignable< Class >::value && std::is_nothrow_copy_constructible< Class >::value;
		}

		template< typename Class >
		constexpr inline bool IsNothrowMoveAssign() noexcept
		{
			return std::is_nothrow_move_assignable< Class >::value && std::is_nothrow_move_constructible< Class >::value;
		}

		namespace details
		{
			template< typename T, typename = void >
			struct IsSwappable
			{
				static constexpr bool value = false;
			};

			template< typename T >
			struct IsSwappable< T, decltype(void(std::declval< T & >().swap(std::declval< const typename T::value_type & >()))) >
			{
				static constexpr bool value = true;
			};
		}	 // namespace details

		template< typename Class >
		constexpr inline bool IsSwappable() noexcept
		{
			return details::IsSwappable< Class >::value;
		}
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_META_HPP */
