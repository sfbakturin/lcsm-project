#ifndef LCSM_SUPPORT_ARRAY_HPP
#define LCSM_SUPPORT_ARRAY_HPP

#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/lcsmconfig.h>
#include <type_traits>

#include <array>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace lcsm
{

	namespace support
	{
		template< typename T, std::size_t S >
		class LCSM_API StaticArray
		{
			static_assert(std::is_default_constructible< T >::value, "StaticArray::T should be default constructible");

			static constexpr bool NOTHROW_DEFAULT_CONSTRUCTIBLE = std::is_nothrow_default_constructible< T >::value;
			static constexpr bool NOTHROW_COPY_CONSTRUCTIBLE = std::is_nothrow_copy_constructible< T >::value;
			static constexpr bool NOTHROW_COPY_ASSIGNABLE = std::is_nothrow_copy_assignable< T >::value;
			static constexpr bool NOTHROW_MOVE_CONSTRUCTIBLE = std::is_nothrow_move_constructible< T >::value;
			static constexpr bool NOTHROW_MOVE_ASSIGNABLE = std::is_nothrow_move_assignable< T >::value;

		  public:
			using value_type = T;
			using reference = value_type &;
			using const_reference = const value_type &;
			using pointer = value_type *;
			using const_pointer = const value_type *;
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			StaticArray() noexcept(NOTHROW_DEFAULT_CONSTRUCTIBLE) : m_data{} {}

			StaticArray(const T &initial) noexcept(NOTHROW_COPY_ASSIGNABLE)
			{
				for (size_type i = 0; i < S; i++)
				{
					m_data[i] = initial;
				}
			}

			StaticArray(T &&initial) noexcept(NOTHROW_COPY_ASSIGNABLE && NOTHROW_MOVE_ASSIGNABLE)
			{
				for (size_type i = 0; i < S - 1; i++)
				{
					m_data[i] = initial;
				}
				m_data[S - 1] = std::move(initial);
			}

			StaticArray(const StaticArray &other) noexcept(NOTHROW_COPY_ASSIGNABLE)
			{
				for (size_type i = 0; i < S; i++)
				{
					m_data[i] = other[i];
				}
			}

			StaticArray(StaticArray &&other) noexcept(NOTHROW_MOVE_ASSIGNABLE)
			{
				for (size_type i = 0; i < S; i++)
				{
					m_data[i] = std::move(other[i]);
				}
			}

			StaticArray(const std::array< T, S > &array) noexcept(NOTHROW_COPY_ASSIGNABLE)
			{
				for (size_type i = 0; i < S; i++)
				{
					m_data[i] = array[i];
				}
			}

			StaticArray(std::array< T, S > &&array) noexcept(NOTHROW_MOVE_ASSIGNABLE)
			{
				for (size_type i = 0; i < S; i++)
				{
					m_data[i] = std::move(array[i]);
				}
			}

			StaticArray &operator=(const StaticArray &other) noexcept(NOTHROW_COPY_CONSTRUCTIBLE)
			{
				return CopyAssign< StaticArray< T, S > >(this, other);
			}

			StaticArray &operator=(StaticArray &&other) noexcept(NOTHROW_MOVE_CONSTRUCTIBLE)
			{
				return MoveAssign< StaticArray< T, S > >(this, std::move(other));
			}

			void swap(StaticArray &other) noexcept
			{
				for (size_type i = 0; i < S; i++)
				{
					std::swap(m_data[i], other[i]);
				}
			}

			size_type size() const noexcept { return S; }

			reference operator[](size_type pos)
			{
				if (S > pos)
				{
					return m_data[pos];
				}
				throw std::out_of_range("Out of reference");
			}

			const_reference operator[](size_type pos) const
			{
				if (S > pos)
				{
					return m_data[pos];
				}
				throw std::out_of_range("Out of reference");
			}

		  private:
			T m_data[S];
		};
	}	 // namespace support
}	 // namespace lcsm

// template< typename T, std::size_t S >
// lcsm::support::StaticArray< T, S >::StaticArray() noexcept(NOTHROW_DEFAULT_CONSTRUCTIBLE) : m_data{}
// {
// }

// template< typename T, std::size_t S >
// lcsm::support::StaticArray< T, S >::StaticArray(const StaticArray &other) noexcept(NOTHROW_COPY_ASSIGNABLE)
// {
// 	using size_type = lcsm::support::StaticArray< T, S >::size_type;
// 	for (size_type i = 0; i < S; i++)
// 		m_data[i] = other[i];
// }

// template< typename T, std::size_t S >
// lcsm::support::StaticArray< T, S >::StaticArray(StaticArray &&other) noexcept(NOTHROW_MOVE_ASSIGNABLE)
// {
// 	using size_type = lcsm::support::StaticArray< T, S >::size_type;
// 	for (size_type i = 0; i < S; i++)
// 		m_data[i] = std::move(other[i]);
// }

// template< typename T, std::size_t S >
// lcsm::support::StaticArray< T, S >::StaticArray(const T &initial) noexcept(NOTHROW_COPY_ASSIGNABLE)
// {
// 	using size_type = lcsm::support::StaticArray< T, S >::size_type;
// 	for (size_type i = 0; i < S; i++)
// 		m_data[i] = initial;
// }

// template< typename T, std::size_t S >
// lcsm::support::StaticArray< T, S >::StaticArray(T &&initial) noexcept(NOTHROW_COPY_ASSIGNABLE &&
// NOTHROW_MOVE_ASSIGNABLE)
// {
// 	using size_type = lcsm::support::StaticArray< T, S >::size_type;
// 	for (size_type i = 0; i < S - 1; i++)
// 		m_data[i] = initial;
// 	m_data[S - 1] = std::move(initial);
// }

// template< typename T, std::size_t S >
// lcsm::support::StaticArray< T, S >::StaticArray(const std::array< T, S > &array) noexcept(NOTHROW_COPY_ASSIGNABLE)
// {
// 	using size_type = lcsm::support::StaticArray< T, S >::size_type;
// 	for (size_type i = 0; i < S; i++)
// 		m_data[i] = array[i];
// }

// template< typename T, std::size_t S >
// lcsm::support::StaticArray< T, S >::StaticArray(std::array< T, S > &&array) noexcept(NOTHROW_MOVE_ASSIGNABLE)
// {
// 	using size_type = lcsm::support::StaticArray< T, S >::size_type;
// 	for (size_type i = 0; i < S; i++)
// 		m_data[i] = std::move(array[i]);
// }

// template< typename T, std::size_t S >
// lcsm::support::StaticArray< T, S > &
// 	lcsm::support::StaticArray< T, S >::operator=(const StaticArray &other) noexcept(NOTHROW_COPY_CONSTRUCTIBLE)
// {
// 	if (this != std::addressof(other))
// 		lcsm::support::StaticArray< T, S >(other).swap(*this);
// 	return *this;
// }

// template< typename T, std::size_t S >
// lcsm::support::StaticArray< T, S > &
// 	lcsm::support::StaticArray< T, S >::operator=(StaticArray &&other) noexcept(NOTHROW_MOVE_CONSTRUCTIBLE)
// {
// 	if (this != std::addressof(other))
// 		lcsm::support::StaticArray< T, S >(std::move(other)).swap(*this);
// 	return *this;
// }

// template< typename T, std::size_t S >
// void lcsm::support::StaticArray< T, S >::swap(StaticArray &other) noexcept
// {
// 	for (std::size_t i = 0; i < S; i++)
// 		std::swap(m_data[i], other[i]);
// }

// template< typename T, std::size_t S >
// std::size_t lcsm::support::StaticArray< T, S >::size() const noexcept
// {
// 	return S;
// }

// template< typename T, std::size_t S >
// typename lcsm::support::StaticArray< T, S >::reference
// 	lcsm::support::StaticArray< T, S >::operator[](lcsm::support::StaticArray< T, S >::size_type pos)
// {
// 	if (S > pos)
// 		return m_data[pos];
// 	throw std::out_of_range("Out of reference");
// }

// template< typename T, std::size_t S >
// typename lcsm::support::StaticArray< T, S >::const_reference
// 	lcsm::support::StaticArray< T, S >::operator[](lcsm::support::StaticArray< T, S >::size_type pos) const
// {
// 	if (S > pos)
// 		return m_data[pos];
// 	throw std::out_of_range("Out of reference");
// }

#endif /* LCSM_SUPPORT_ARRAY_HPP */
