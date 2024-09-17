#ifndef SIM_SUPPORT_POINTERVIEW_HPP
#define SIM_SUPPORT_POINTERVIEW_HPP

#include <memory>
#include <utility>

namespace sim
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

		  private:
			pointer m_ptr;
		};
	}	 // namespace support
}	 // namespace sim

template< typename T >
sim::support::PointerView< T >::PointerView() noexcept : m_ptr(nullptr)
{
}

template< typename T >
sim::support::PointerView< T >::PointerView(T *ptr) noexcept : m_ptr(ptr)
{
}

template< typename T >
sim::support::PointerView< T >::PointerView(T &ref) noexcept :
	m_ptr(std::addressof(ref))
{
}

template< typename T >
sim::support::PointerView< T >::PointerView(std::shared_ptr< T > &ptr) noexcept :
	m_ptr(ptr.get())
{
}

template< typename T >
sim::support::PointerView< T >::PointerView(const PointerView< T > &other) noexcept
	: m_ptr(other.m_ptr)
{
}

template< typename T >
sim::support::PointerView< T >::PointerView(PointerView &&other) noexcept :
	m_ptr(other.m_ptr)
{
	other.m_ptr = nullptr;
}

template< typename T >
sim::support::PointerView< T > &
	sim::support::PointerView< T >::operator=(const sim::support::PointerView< T > &other) noexcept
{
	if (this != &other)
		sim::support::PointerView< T >(other).swap(*this);
	return *this;
}

template< typename T >
sim::support::PointerView< T > &
	sim::support::PointerView< T >::operator=(sim::support::PointerView< T > &&other) noexcept
{
	if (this != &other)
		sim::support::PointerView< T >(std::move(other)).swap(*this);
	return *this;
}

template< typename T >
T *sim::support::PointerView< T >::operator->() noexcept
{
	return ptr();
}

template< typename T >
const T *sim::support::PointerView< T >::operator->() const noexcept
{
	return cptr();
}

template< typename T >
T &sim::support::PointerView< T >::operator*() & noexcept
{
	return value();
}

template< typename T >
const T &sim::support::PointerView< T >::operator*() const & noexcept
{
	return value();
}

template< typename T >
void sim::support::PointerView< T >::swap(sim::support::PointerView< T > &other) noexcept
{
	std::swap(m_ptr, other.m_ptr);
}

template< typename T >
bool sim::support::PointerView< T >::hasValue() const noexcept
{
	return m_ptr != nullptr;
}

template< typename T >
sim::support::PointerView< T >::operator bool() const noexcept
{
	return hasValue();
}

template< typename T >
T *sim::support::PointerView< T >::ptr() noexcept
{
	return m_ptr;
}

template< typename T >
const T *sim::support::PointerView< T >::cptr() const noexcept
{
	return m_ptr;
}

template< typename T >
T &sim::support::PointerView< T >::ref() noexcept
{
	return *m_ptr;
}

template< typename T >
const T &sim::support::PointerView< T >::cref() const noexcept
{
	return *m_ptr;
}

template< typename T >
bool operator==(const sim::support::PointerView< T > &left, T *right) noexcept
{
	return left.cptr() == right;
}

template< typename T >
bool operator==(const sim::support::PointerView< T > &left, const T *right) noexcept
{
	return left.cptr() == right;
}

template< typename T >
bool operator==(T *left, const sim::support::PointerView< T > &right) noexcept
{
	return left == right.cptr();
}

template< typename T >
bool operator==(const T *left, const sim::support::PointerView< T > &right) noexcept
{
	return left == right.cptr();
}

#endif /* SIM_SUPPORT_POINTERVIEW_HPP */
