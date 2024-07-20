#ifndef SIM_SUPPORT_REF_HPP
#define SIM_SUPPORT_REF_HPP

#include <memory>
#include <utility>

namespace sim
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
}	 // namespace sim

template< typename T >
sim::Ref< T >::Ref() noexcept : m_ptr(nullptr)
{
}

template< typename T >
sim::Ref< T >::Ref(T *ptr) noexcept : m_ptr(ptr)
{
}

template< typename T >
sim::Ref< T >::Ref(T &ref) noexcept : m_ptr(std::addressof(ref))
{
}

template< typename T >
sim::Ref< T >::Ref(const Ref< T > &other) noexcept : m_ptr(other.m_ptr)
{
}

template< typename T >
sim::Ref< T >::Ref(Ref &&other) noexcept : m_ptr(other.m_ptr)
{
	other.m_ptr = nullptr;
}

template< typename T >
sim::Ref< T > &sim::Ref< T >::operator=(const sim::Ref< T > &other) noexcept
{
	if (this != &other)
		sim::Ref< T >(other).Swap(*this);
	return *this;
}

template< typename T >
sim::Ref< T > &sim::Ref< T >::operator=(sim::Ref< T > &&other) noexcept
{
	if (this != &other)
		sim::Ref< T >(std::move(other)).Swap(*this);
	return *this;
}

template< typename T >
void sim::Ref< T >::Swap(sim::Ref< T > &other) noexcept
{
	std::swap(m_ptr, other.m_ptr);
}

template< typename T >
bool sim::Ref< T >::HasValue() const noexcept
{
	return m_ptr != nullptr;
}

template< typename T >
sim::Ref< T >::operator bool() const noexcept
{
	return HasValue();
}

template< typename T >
const T *sim::Ref< T >::Ptr() const noexcept
{
	return m_ptr;
}

template< typename T >
T *sim::Ref< T >::Ptr() noexcept
{
	return m_ptr;
}

template< typename T >
const T &sim::Ref< T >::Val() const noexcept
{
	return *m_ptr;
}

template< typename T >
T &sim::Ref< T >::Val() noexcept
{
	return *m_ptr;
}

#endif /* SIM_SUPPORT_REF_HPP */
