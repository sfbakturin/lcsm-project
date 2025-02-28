#include <initializer_list>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/Algorithm.hpp>

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::Context::Context() : lcsm::Context(1, 0) {}

lcsm::Context::Context(std::size_t n, std::size_t privateSize) :
	m_databits(n), m_private(privateSize), m_updating(false)
{
}

lcsm::Context::Context(const lcsm::Context &other) :
	m_databits(other.m_databits), m_timestamp(other.m_timestamp), m_private(other.m_private), m_updating(other.m_updating)
{
}

lcsm::Context::Context(lcsm::Context &&other) noexcept :
	m_databits(std::move(other.m_databits)), m_timestamp(std::move(other.m_timestamp)),
	m_private(std::move(other.m_private)), m_updating(other.m_updating)
{
}

lcsm::Context &lcsm::Context::operator=(const lcsm::Context &other)
{
	return lcsm::support::CopyAssign< lcsm::Context >(this, other);
}

lcsm::Context &lcsm::Context::operator=(lcsm::Context &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::Context >(this, std::move(other));
}

void lcsm::Context::swap(lcsm::Context &other) noexcept
{
	std::swap(m_databits, other.m_databits);
	std::swap(m_timestamp, other.m_timestamp);
	std::swap(m_private, other.m_private);
	std::swap(m_updating, other.m_updating);
}

std::size_t lcsm::Context::size() const noexcept
{
	return m_databits.size();
}

const lcsm::Timestamp &lcsm::Context::lastUpdate() const noexcept
{
	return m_timestamp;
}

const lcsm::DataBits &lcsm::Context::getValue(std::size_t i) const
{
	if (i >= m_databits.size())
		throw std::out_of_range("Context: Index out of bound");
	return m_databits[i];
}

const std::vector< lcsm::DataBits > &lcsm::Context::values() const noexcept
{
	return m_databits;
}

bool lcsm::Context::isEqualsValues(const Context &other) const
{
	/* Verify sizes. */
	const std::size_t s = size();
	if (s != other.size())
		throw std::logic_error("Context: when compare to other context for values, sizes must be exactly right.");

	/* Check equality. If any is not equals, then return false. */
	for (std::size_t i = 0; i < s; i++)
		if (m_databits[i] != other.m_databits[i])
			return false;

	/* Otherwise, return true. */
	return true;
}

void lcsm::Context::beginUpdate(const lcsm::Timestamp &timestamp)
{
	if (m_updating)
		throw std::logic_error("Context is already updating.");
	m_timestamp = timestamp;
	m_updating = true;
}

void lcsm::Context::beginUpdate(lcsm::Timestamp &&timestamp)
{
	if (m_updating)
		throw std::logic_error("Context is already updating.");
	m_timestamp = std::move(timestamp);
	m_updating = true;
}

void lcsm::Context::updateValue(std::size_t i, const lcsm::DataBits &databits)
{
	if (!m_updating)
		throw std::logic_error("Context is locked for updating.");
	if (i >= m_databits.size())
		throw std::out_of_range("Context: Index out of bound");
	m_databits[i] = databits;
}

void lcsm::Context::updateValue(std::size_t i, lcsm::DataBits &&databits)
{
	if (!m_updating)
		throw std::logic_error("Context is locked for updating.");
	if (i >= m_databits.size())
		throw std::out_of_range("Context: Index out of bound");
	m_databits[i] = std::move(databits);
}

void lcsm::Context::endUpdate()
{
	if (!m_updating)
		throw std::logic_error("Context is not updating right now.");
	m_updating = false;
}

void lcsm::Context::updateValues(const lcsm::Timestamp &timestamp, const std::vector< lcsm::DataBits > &databits)
{
	if (m_updating)
		throw std::logic_error("Context is already updating.");
	if (databits.size() != m_databits.size())
		throw std::logic_error("Context can't apply different number of values.");
	m_timestamp = timestamp;
	m_databits = databits;
}

void lcsm::Context::updateValues(const lcsm::Timestamp &timestamp, std::vector< lcsm::DataBits > &&databits)
{
	if (m_updating)
		throw std::logic_error("Context is already updating.");
	if (databits.size() != m_databits.size())
		throw std::logic_error("Context can't apply different number of values.");
	m_timestamp = timestamp;
	m_databits = std::move(databits);
}

void lcsm::Context::updateValues(lcsm::Timestamp &&timestamp, const std::vector< lcsm::DataBits > &databits)
{
	if (m_updating)
		throw std::logic_error("Context is already updating.");
	if (databits.size() != m_databits.size())
		throw std::logic_error("Context can't apply different number of values.");
	m_timestamp = std::move(timestamp);
	m_databits = databits;
}

void lcsm::Context::updateValues(lcsm::Timestamp &&timestamp, std::vector< lcsm::DataBits > &&databits)
{
	if (m_updating)
		throw std::logic_error("Context is already updating.");
	if (databits.size() != m_databits.size())
		throw std::logic_error("Context can't apply different number of values.");
	m_timestamp = std::move(timestamp);
	m_databits = std::move(databits);
}

void lcsm::Context::updateValues(const lcsm::Timestamp &timestamp, std::initializer_list< lcsm::DataBits > databits)
{
	if (m_updating)
		throw std::logic_error("Context is already updating.");
	if (databits.size() != m_databits.size())
		throw std::logic_error("Context can't apply different number of values.");
	m_timestamp = timestamp;
	m_databits = databits;
}

void lcsm::Context::updateValues(lcsm::Timestamp &&timestamp, std::initializer_list< lcsm::DataBits > databits)
{
	if (m_updating)
		throw std::logic_error("Context is already updating.");
	if (databits.size() != m_databits.size())
		throw std::logic_error("Context can't apply different number of values.");
	m_timestamp = std::move(timestamp);
	m_databits = databits;
}

bool lcsm::Context::neverUpdate() const noexcept
{
	return m_timestamp.isReset();
}

lcsm::PrivateContext &lcsm::Context::privateContext() noexcept
{
	return m_private;
}

const lcsm::PrivateContext &lcsm::Context::privateContext() const noexcept
{
	return m_private;
}
