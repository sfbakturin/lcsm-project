#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/Algorithm.hpp>

#include <cstddef>
#include <utility>

lcsm::Timestamp::Timestamp() noexcept : m_tick(0) {}

lcsm::Timestamp::Timestamp(const lcsm::Timestamp &other) noexcept : m_tick(other.m_tick) {}

lcsm::Timestamp::Timestamp(lcsm::Timestamp &&other) noexcept : m_tick(other.m_tick) {}

lcsm::Timestamp &lcsm::Timestamp::operator=(const lcsm::Timestamp &other) noexcept
{
	return lcsm::support::CopyAssign< lcsm::Timestamp >(this, other);
}

lcsm::Timestamp &lcsm::Timestamp::operator=(lcsm::Timestamp &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::Timestamp >(this, std::move(other));
}

void lcsm::Timestamp::swap(lcsm::Timestamp &other) noexcept
{
	std::swap(m_tick, other.m_tick);
}

lcsm::Timestamp lcsm::Timestamp::next() const noexcept
{
	lcsm::Timestamp next = *this;
	next.m_tick++;
	return next;
}

bool lcsm::Timestamp::isReset() const noexcept
{
	return m_tick == 0;
}

bool lcsm::Timestamp::operator<(const lcsm::Timestamp &other) const noexcept
{
	/* Compare simulator ticks. */
	return m_tick < other.m_tick;
}

bool lcsm::Timestamp::operator<=(const lcsm::Timestamp &other) const noexcept
{
	return *this < other || *this == other;
}

bool lcsm::Timestamp::operator==(const lcsm::Timestamp &other) const noexcept
{
	/* Compare simulator ticks. */
	return m_tick == other.m_tick;
}

bool lcsm::Timestamp::operator!=(const lcsm::Timestamp &other) const noexcept
{
	return !(*this == other);
}

bool lcsm::Timestamp::operator>(const lcsm::Timestamp &other) const noexcept
{
	return !(*this < other) && *this != other;
}

bool lcsm::Timestamp::operator>=(const lcsm::Timestamp &other) const noexcept
{
	return !(*this < other);
}

lcsm::Timestamp lcsm::Timestamp::operator+(const lcsm::Timestamp &other) const noexcept
{
	lcsm::Timestamp sum = *this;
	sum.m_tick += other.m_tick;
	return sum;
}

lcsm::Timestamp &lcsm::Timestamp::operator+=(const lcsm::Timestamp &other) noexcept
{
	*this = *this + other;
	return *this;
}

lcsm::Timestamp &lcsm::Timestamp::operator++() noexcept
{
	m_tick++;
	return *this;
}

lcsm::Timestamp lcsm::Timestamp::operator++(int) noexcept
{
	lcsm::Timestamp temp = *this;
	++*this;
	return temp;
}

std::size_t lcsm::Timestamp::hashCode() const noexcept
{
	return static_cast< std::size_t >(m_tick);
}
