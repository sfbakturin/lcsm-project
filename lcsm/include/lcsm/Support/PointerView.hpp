#ifndef LCSM_SUPPORT_POINTERVIEW_HPP
#define LCSM_SUPPORT_POINTERVIEW_HPP

#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/lcsmconfig.h>

#include <cstdint>
#include <memory>
#include <utility>

namespace lcsm
{
	namespace support
	{
		template< typename T >
		class LCSM_API PointerView
		{
		  public:
			using value_type = T;
			using shared_value_type = std::shared_ptr< value_type >;
			using reference = value_type &;
			using const_reference = const value_type &;
			using pointer = value_type *;
			using const_pointer = const value_type *;

			PointerView() noexcept : m_ptr(nullptr) {}
			PointerView(value_type *ptr) noexcept : m_ptr(ptr) {}
			PointerView(value_type &ref) noexcept : m_ptr(std::addressof(ref)) {}
			PointerView(shared_value_type &ptr) noexcept : m_ptr(ptr.get()) {}

			PointerView(const PointerView &other) noexcept : m_ptr(other.m_ptr) {}
			PointerView(PointerView &&other) noexcept : m_ptr(other.m_ptr) { other.m_ptr = nullptr; }

			PointerView &operator=(const PointerView &other) noexcept
			{
				return CopyAssign< PointerView< T > >(this, other);
			}
			PointerView &operator=(PointerView &&other) noexcept
			{
				return MoveAssign< PointerView< T > >(this, std::move(other));
			}

			bool operator==(const PointerView &other) const noexcept { return m_ptr == other.m_ptr; }
			bool operator!=(const PointerView &other) const noexcept { return m_ptr != other.m_ptr; }

			bool operator==(const_pointer other) const noexcept { return m_ptr == other; }
			bool operator!=(const_pointer other) const noexcept { return m_ptr != other; }

			pointer operator->() noexcept { return ptr(); }
			const_pointer operator->() const noexcept { return cptr(); }

			reference operator*() & noexcept { return ref(); }
			const_reference operator*() const & noexcept { return cref(); }

			void swap(PointerView &other) noexcept { std::swap(m_ptr, other.m_ptr); }

			bool hasValue() const noexcept { return m_ptr != nullptr; }
			explicit operator bool() const noexcept { return hasValue(); }

			pointer ptr() noexcept { return m_ptr; }
			const_pointer cptr() const noexcept { return m_ptr; }

			reference ref() noexcept { return *m_ptr; }
			const_reference cref() const noexcept { return *m_ptr; }

			pointer get() noexcept { return ptr(); }
			const_pointer get() const noexcept { return cptr(); }

			template< typename R >
			const R *getCast() const noexcept
			{
				return static_cast< const R * >(m_ptr);
			}

			template< typename R >
			R *getCast() noexcept
			{
				return static_cast< R * >(m_ptr);
			}

			void reset() noexcept { m_ptr = nullptr; }

			std::size_t hashCode() const noexcept
			{
				std::uintptr_t i = reinterpret_cast< std::uintptr_t >(m_ptr);
				return i;
			}

			static PointerView< T > fromSharedPtr(const std::shared_ptr< T > &ptr) noexcept { return { ptr.get() }; }

			template< typename R >
			PointerView< R > staticCast() const noexcept
			{
				return { static_cast< R * >(m_ptr) };
			}

		  private:
			pointer m_ptr;
		};
	}	 // namespace support
}	 // namespace lcsm

namespace std
{
	template< typename T >
	struct hash< lcsm::support::PointerView< T > >
	{
		LCSM_API std::size_t operator()(const lcsm::support::PointerView< T > &object) const
		{
			return object.hashCode();
		}
	};
}	 // namespace std

#endif /* LCSM_SUPPORT_POINTERVIEW_HPP */
