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
			PointerView() noexcept;
			PointerView(T *ptr) noexcept;
			PointerView(T &ref) noexcept;
			PointerView(std::shared_ptr< T > &ptr) noexcept;

			PointerView(const PointerView &other) noexcept;
			PointerView(PointerView &&other) noexcept;

			PointerView &operator=(const PointerView &other) noexcept;
			PointerView &operator=(PointerView &&other) noexcept;

			const T *operator->() const noexcept;
			T *operator->() noexcept;

			const T &operator*() const & noexcept;
			T &operator*() & noexcept;

			void swap(PointerView &other) noexcept;

			bool hasValue() const noexcept;
			explicit operator bool() const noexcept;

			T *Ptr() noexcept;
			const T *Ptr() const noexcept;

			T &Val() noexcept;
			const T &value() const noexcept;

		  private:
			T *m_ptr;
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
const T *sim::support::PointerView< T >::operator->() const noexcept
{
	return Ptr();
}

template< typename T >
T *sim::support::PointerView< T >::operator->() noexcept
{
	return Ptr();
}

template< typename T >
const T &sim::support::PointerView< T >::operator*() const & noexcept
{
	return value();
}

template< typename T >
T &sim::support::PointerView< T >::operator*() & noexcept
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
const T *sim::support::PointerView< T >::Ptr() const noexcept
{
	return m_ptr;
}

template< typename T >
T *sim::support::PointerView< T >::Ptr() noexcept
{
	return m_ptr;
}

template< typename T >
const T &sim::support::PointerView< T >::value() const noexcept
{
	return *m_ptr;
}

template< typename T >
T &sim::support::PointerView< T >::Val() noexcept
{
	return *m_ptr;
}

#endif /* SIM_SUPPORT_POINTERVIEW_HPP */
