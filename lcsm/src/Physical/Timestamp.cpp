#include <lcsm/LCSM.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/Numbers.hpp>

#include <memory>
#include <utility>

const lcsm::Timestamp lcsm::Timestamp::RESET;

lcsm::Timestamp::Timestamp() noexcept :
	m_tickFast(0), m_tickStatic(0), m_tickComposite(0), m_tickDynamic(0), m_tickUser(0)
{
}

lcsm::Timestamp::Timestamp(const lcsm::Timestamp &other) noexcept :
	m_tickFast(other.m_tickFast), m_tickStatic(other.m_tickStatic), m_tickComposite(other.m_tickComposite),
	m_tickDynamic(other.m_tickDynamic), m_tickUser(other.m_tickUser)
{
}

lcsm::Timestamp::Timestamp(lcsm::timer_t tickFast, lcsm::timer_t tickStatic, lcsm::timer_t tickComposite, lcsm::timer_t tickDynamic, lcsm::timer_t tickUser) noexcept
	:
	m_tickFast(tickFast), m_tickStatic(tickStatic), m_tickComposite(tickComposite), m_tickDynamic(tickDynamic), m_tickUser(tickUser)
{
}

lcsm::Timestamp::Timestamp(lcsm::Timestamp &&other) noexcept :
	m_tickFast(other.m_tickFast), m_tickStatic(other.m_tickStatic), m_tickComposite(other.m_tickComposite),
	m_tickDynamic(other.m_tickDynamic), m_tickUser(other.m_tickUser)
{
}

lcsm::Timestamp &lcsm::Timestamp::operator=(const lcsm::Timestamp &other) noexcept
{
	if (this != std::addressof(other))
		lcsm::Timestamp(other).swap(*this);
	return *this;
}

lcsm::Timestamp &lcsm::Timestamp::operator=(lcsm::Timestamp &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::Timestamp(std::move(other)).swap(*this);
	return *this;
}

void lcsm::Timestamp::swap(lcsm::Timestamp &other) noexcept
{
	std::swap(m_tickFast, other.m_tickFast);
	std::swap(m_tickStatic, other.m_tickStatic);
	std::swap(m_tickComposite, other.m_tickComposite);
	std::swap(m_tickDynamic, other.m_tickDynamic);
	std::swap(m_tickUser, other.m_tickUser);
}

bool lcsm::Timestamp::isReset() const noexcept
{
	return *this == lcsm::Timestamp::RESET;
}

lcsm::Timestamp lcsm::Timestamp::tickFast() const noexcept
{
	return { static_cast< lcsm::timer_t >(m_tickFast + TICK), m_tickStatic, m_tickComposite, m_tickDynamic, m_tickUser };
}

lcsm::Timestamp lcsm::Timestamp::tickStatic() const noexcept
{
	return { m_tickFast, static_cast< lcsm::timer_t >(m_tickStatic + TICK), m_tickComposite, m_tickDynamic, m_tickUser };
}

lcsm::Timestamp lcsm::Timestamp::tickComposite() const noexcept
{
	return { m_tickFast, m_tickStatic, static_cast< lcsm::timer_t >(m_tickComposite + TICK), m_tickDynamic, m_tickUser };
}

lcsm::Timestamp lcsm::Timestamp::tickDynamic() const noexcept
{
	return { m_tickFast, m_tickStatic, m_tickComposite, static_cast< lcsm::timer_t >(m_tickDynamic + TICK), m_tickUser };
}

lcsm::Timestamp lcsm::Timestamp::tickUser() const noexcept
{
	return { m_tickFast, m_tickStatic, m_tickComposite, m_tickDynamic, static_cast< lcsm::timer_t >(m_tickUser + TICK) };
}

lcsm::Timestamp lcsm::Timestamp::normalizeToDynamic() const noexcept
{
	return { 0, 0, 0, m_tickDynamic, m_tickUser };
}

lcsm::Timestamp lcsm::Timestamp::normalizeToComposite() const noexcept
{
	return { 0, 0, m_tickComposite, m_tickDynamic, m_tickUser };
}

lcsm::Timestamp lcsm::Timestamp::normalizeToStatic() const noexcept
{
	return { 0, m_tickStatic, m_tickComposite, m_tickDynamic, m_tickUser };
}

lcsm::Timestamp lcsm::Timestamp::operator+(const lcsm::Timestamp &other) const noexcept
{
	const lcsm::timer_t tickFast = m_tickFast + other.m_tickFast;
	const lcsm::timer_t tickStatic = m_tickStatic + other.m_tickStatic;
	const lcsm::timer_t tickComposite = m_tickComposite + other.m_tickComposite;
	const lcsm::timer_t tickDynamic = m_tickDynamic + other.m_tickDynamic;
	const lcsm::timer_t tickUser = m_tickUser + other.m_tickUser;
	return { tickFast, tickStatic, tickComposite, tickDynamic, tickUser };
}

lcsm::Timestamp lcsm::Timestamp::operator-(const lcsm::Timestamp &other) const noexcept
{
	const lcsm::timer_t tickFast = other.m_tickFast > m_tickFast ? 0 : m_tickFast - other.m_tickFast;
	const lcsm::timer_t tickStatic = other.m_tickStatic > m_tickStatic ? 0 : m_tickStatic - other.m_tickStatic;
	const lcsm::timer_t tickComposite = other.m_tickComposite > m_tickComposite ? 0 : m_tickComposite - other.m_tickComposite;
	const lcsm::timer_t tickDynamic = other.m_tickDynamic > m_tickDynamic ? 0 : m_tickDynamic - other.m_tickDynamic;
	const lcsm::timer_t tickUser = other.m_tickUser > m_tickUser ? 0 : m_tickUser - other.m_tickUser;
	return { tickFast, tickStatic, tickComposite, tickDynamic, tickUser };
}

lcsm::Timestamp &lcsm::Timestamp::operator+=(const lcsm::Timestamp &other) noexcept
{
	*this = *this + other;
	return *this;
}

lcsm::Timestamp &lcsm::Timestamp::operator-=(const lcsm::Timestamp &other) noexcept
{
	*this = *this - other;
	return *this;
}

bool lcsm::Timestamp::operator<(const lcsm::Timestamp &other) const noexcept
{
	const int cmpUser = lcsm::support::compare(m_tickUser, other.m_tickUser);
	const int cmpDynamic = lcsm::support::compare(m_tickDynamic, other.m_tickDynamic);
	const int cmpComposite = lcsm::support::compare(m_tickComposite, other.m_tickComposite);
	const int cmpStatic = lcsm::support::compare(m_tickStatic, other.m_tickStatic);
	const int cmpFast = lcsm::support::compare(m_tickFast, other.m_tickFast);

	if (cmpUser != 0)
		return cmpUser == 1;
	else if (cmpDynamic != 0)
		return cmpDynamic == 1;
	else if (cmpComposite != 0)
		return cmpComposite == 1;
	else if (cmpStatic != 0)
		return cmpStatic == 1;
	else if (cmpFast != 0)
		return cmpFast == 1;

	return false;
}

bool lcsm::Timestamp::operator<=(const lcsm::Timestamp &other) const noexcept
{
	return *this < other || *this == other;
}

bool lcsm::Timestamp::operator==(const lcsm::Timestamp &other) const noexcept
{
	return m_tickUser == other.m_tickUser && m_tickDynamic == other.m_tickDynamic &&
		   m_tickComposite == other.m_tickComposite && m_tickStatic == other.m_tickStatic && m_tickFast == other.m_tickFast;
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
