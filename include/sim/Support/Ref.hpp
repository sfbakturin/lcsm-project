#ifndef SIM_SUPPORT_REF_HPP
#define SIM_SUPPORT_REF_HPP

#include <memory>
#include <utility>

namespace sim
{
	namespace support
	{
		template< typename T >
		class Ref
		{
		  public:
			Ref() noexcept;
			Ref(T *ptr) noexcept;
			Ref(T &ref) noexcept;

			Ref(const Ref &other) noexcept;
			Ref(Ref &&other) noexcept;

			Ref &operator=(const Ref &other) noexcept;
			Ref &operator=(Ref &&other) noexcept;

			void Swap(Ref &other) noexcept;

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
sim::support::Ref< T >::Ref() noexcept : m_ptr(nullptr)
{
}

template< typename T >
sim::support::Ref< T >::Ref(T *ptr) noexcept : m_ptr(ptr)
{
}

template< typename T >
sim::support::Ref< T >::Ref(T &ref) noexcept : m_ptr(std::addressof(ref))
{
}

template< typename T >
sim::support::Ref< T >::Ref(const Ref< T > &other) noexcept : m_ptr(other.m_ptr)
{
}

template< typename T >
sim::support::Ref< T >::Ref(Ref &&other) noexcept : m_ptr(other.m_ptr)
{
	other.m_ptr = nullptr;
}

template< typename T >
sim::support::Ref< T > &sim::support::Ref< T >::operator=(const sim::support::Ref< T > &other) noexcept
{
	if (this != &other)
		sim::support::Ref< T >(other).Swap(*this);
	return *this;
}

template< typename T >
sim::support::Ref< T > &sim::support::Ref< T >::operator=(sim::support::Ref< T > &&other) noexcept
{
	if (this != &other)
		sim::support::Ref< T >(std::move(other)).Swap(*this);
	return *this;
}

template< typename T >
void sim::support::Ref< T >::Swap(sim::support::Ref< T > &other) noexcept
{
	std::swap(m_ptr, other.m_ptr);
}

template< typename T >
bool sim::support::Ref< T >::HasValue() const noexcept
{
	return m_ptr != nullptr;
}

template< typename T >
sim::support::Ref< T >::operator bool() const noexcept
{
	return HasValue();
}

template< typename T >
const T *sim::support::Ref< T >::Ptr() const noexcept
{
	return m_ptr;
}

template< typename T >
T *sim::support::Ref< T >::Ptr() noexcept
{
	return m_ptr;
}

template< typename T >
const T &sim::support::Ref< T >::Val() const noexcept
{
	return *m_ptr;
}

template< typename T >
T &sim::support::Ref< T >::Val() noexcept
{
	return *m_ptr;
}

#endif /* SIM_SUPPORT_REF_HPP */
