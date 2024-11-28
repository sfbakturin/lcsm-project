#ifndef LCSM_SUPPORT_POINTERVIEW_HPP
#define LCSM_SUPPORT_POINTERVIEW_HPP

#include <cstdint>
#include <memory>
#include <utility>

namespace lcsm
{
	namespace support
	{
		template< typename T >
		class PointerView
		{
		  public:
			using value_type = T;
			using shared_value_type = std::shared_ptr< value_type >;
			using reference = value_type &;
			using const_reference = const value_type &;
			using pointer = value_type *;
			using const_pointer = const value_type *;

			PointerView() noexcept;
			PointerView(value_type *ptr) noexcept;
			PointerView(value_type &ref) noexcept;
			PointerView(shared_value_type &ptr) noexcept;

			PointerView(const PointerView &other) noexcept;
			PointerView(PointerView &&other) noexcept;

			PointerView &operator=(const PointerView &other) noexcept;
			PointerView &operator=(PointerView &&other) noexcept;

			bool operator==(const PointerView &other) const noexcept;
			bool operator!=(const PointerView &other) const noexcept;

			pointer operator->() noexcept;
			const_pointer operator->() const noexcept;

			reference operator*() & noexcept;
			const_reference operator*() const & noexcept;

			void swap(PointerView &other) noexcept;

			bool hasValue() const noexcept;
			explicit operator bool() const noexcept;

			pointer ptr() noexcept;
			const_pointer cptr() const noexcept;

			reference ref() noexcept;
			const_reference cref() const noexcept;

			std::size_t hashCode() const noexcept;

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
		std::size_t operator()(const lcsm::support::PointerView< T > &object) const { return object.hashCode(); }
	};
}	 // namespace std

template< typename T >
lcsm::support::PointerView< T >::PointerView() noexcept : m_ptr(nullptr)
{
}

template< typename T >
lcsm::support::PointerView< T >::PointerView(T *ptr) noexcept : m_ptr(ptr)
{
}

template< typename T >
lcsm::support::PointerView< T >::PointerView(T &ref) noexcept : m_ptr(std::addressof(ref))
{
}

template< typename T >
lcsm::support::PointerView< T >::PointerView(std::shared_ptr< T > &ptr) noexcept : m_ptr(ptr.get())
{
}

template< typename T >
lcsm::support::PointerView< T >::PointerView(const PointerView< T > &other) noexcept : m_ptr(other.m_ptr)
{
}

template< typename T >
lcsm::support::PointerView< T >::PointerView(PointerView &&other) noexcept : m_ptr(other.m_ptr)
{
	other.m_ptr = nullptr;
}

template< typename T >
lcsm::support::PointerView< T > &lcsm::support::PointerView< T >::operator=(const lcsm::support::PointerView< T > &other) noexcept
{
	if (this != &other)
		lcsm::support::PointerView< T >(other).swap(*this);
	return *this;
}

template< typename T >
lcsm::support::PointerView< T > &lcsm::support::PointerView< T >::operator=(lcsm::support::PointerView< T > &&other) noexcept
{
	if (this != &other)
		lcsm::support::PointerView< T >(std::move(other)).swap(*this);
	return *this;
}

template< typename T >
bool lcsm::support::PointerView< T >::operator==(const PointerView< T > &other) const noexcept
{
	return m_ptr == other.m_ptr;
}

template< typename T >
bool lcsm::support::PointerView< T >::operator!=(const PointerView< T > &other) const noexcept
{
	return m_ptr != other.m_ptr;
}

template< typename T >
T *lcsm::support::PointerView< T >::operator->() noexcept
{
	return ptr();
}

template< typename T >
const T *lcsm::support::PointerView< T >::operator->() const noexcept
{
	return cptr();
}

template< typename T >
T &lcsm::support::PointerView< T >::operator*() & noexcept
{
	return this->value();
}

template< typename T >
const T &lcsm::support::PointerView< T >::operator*() const & noexcept
{
	return this->value();
}

template< typename T >
void lcsm::support::PointerView< T >::swap(lcsm::support::PointerView< T > &other) noexcept
{
	std::swap(m_ptr, other.m_ptr);
}

template< typename T >
bool lcsm::support::PointerView< T >::hasValue() const noexcept
{
	return m_ptr != nullptr;
}

template< typename T >
lcsm::support::PointerView< T >::operator bool() const noexcept
{
	return hasValue();
}

template< typename T >
T *lcsm::support::PointerView< T >::ptr() noexcept
{
	return m_ptr;
}

template< typename T >
const T *lcsm::support::PointerView< T >::cptr() const noexcept
{
	return m_ptr;
}

template< typename T >
T &lcsm::support::PointerView< T >::ref() noexcept
{
	return *m_ptr;
}

template< typename T >
const T &lcsm::support::PointerView< T >::cref() const noexcept
{
	return *m_ptr;
}

template< typename T >
std::size_t lcsm::support::PointerView< T >::hashCode() const noexcept
{
	std::uintptr_t i = reinterpret_cast< std::uintptr_t >(m_ptr);
	return i;
}

template< typename T >
bool operator==(const lcsm::support::PointerView< T > &left, T *right) noexcept
{
	return left.cptr() == right;
}

template< typename T >
bool operator==(const lcsm::support::PointerView< T > &left, const T *right) noexcept
{
	return left.cptr() == right;
}

template< typename T >
bool operator==(T *left, const lcsm::support::PointerView< T > &right) noexcept
{
	return left == right.cptr();
}

template< typename T >
bool operator==(const T *left, const lcsm::support::PointerView< T > &right) noexcept
{
	return left == right.cptr();
}

#endif /* LCSM_SUPPORT_POINTERVIEW_HPP */
