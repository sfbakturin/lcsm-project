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

			PointerView(const PointerView &other) noexcept;
			PointerView(PointerView &&other) noexcept;

			PointerView &operator=(const PointerView &other) noexcept;
			PointerView &operator=(PointerView &&other) noexcept;

			void Swap(PointerView &other) noexcept;

			bool HasValue() const noexcept;
			explicit operator bool() const noexcept;

			const T *Ptr() const noexcept;
			T *Ptr() noexcept;

			const T &Val() const noexcept;
			T &Val() noexcept;

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
sim::support::PointerView< T >::PointerView(T &ref) noexcept : m_ptr(std::addressof(ref))
{
}

template< typename T >
sim::support::PointerView< T >::PointerView(const PointerView< T > &other) noexcept : m_ptr(other.m_ptr)
{
}

template< typename T >
sim::support::PointerView< T >::PointerView(PointerView &&other) noexcept : m_ptr(other.m_ptr)
{
	other.m_ptr = nullptr;
}

template< typename T >
sim::support::PointerView< T > &sim::support::PointerView< T >::operator=(const sim::support::PointerView< T > &other) noexcept
{
	if (this != &other)
		sim::support::PointerView< T >(other).Swap(*this);
	return *this;
}

template< typename T >
sim::support::PointerView< T > &sim::support::PointerView< T >::operator=(sim::support::PointerView< T > &&other) noexcept
{
	if (this != &other)
		sim::support::PointerView< T >(std::move(other)).Swap(*this);
	return *this;
}

template< typename T >
void sim::support::PointerView< T >::Swap(sim::support::PointerView< T > &other) noexcept
{
	std::swap(m_ptr, other.m_ptr);
}

template< typename T >
bool sim::support::PointerView< T >::HasValue() const noexcept
{
	return m_ptr != nullptr;
}

template< typename T >
sim::support::PointerView< T >::operator bool() const noexcept
{
	return HasValue();
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
const T &sim::support::PointerView< T >::Val() const noexcept
{
	return *m_ptr;
}

template< typename T >
T &sim::support::PointerView< T >::Val() noexcept
{
	return *m_ptr;
}

#endif /* SIM_SUPPORT_POINTERVIEW_HPP */
