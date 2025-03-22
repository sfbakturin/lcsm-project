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
	m_size(n), m_databits(m_size), m_timestamps(m_size), m_private(privateSize), m_polluted(false)
{
}

lcsm::Context::Context(const lcsm::Context &other) :
	m_size(other.m_size), m_databits(other.m_databits), m_timestamps(other.m_timestamps), m_private(other.m_private),
	m_polluted(other.m_polluted)
{
}

lcsm::Context::Context(lcsm::Context &&other) noexcept :
	m_size(other.m_size), m_databits(std::move(other.m_databits)), m_timestamps(std::move(other.m_timestamps)),
	m_private(std::move(other.m_private)), m_polluted(other.m_polluted)
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
	std::swap(m_size, other.m_size);
	std::swap(m_databits, other.m_databits);
	std::swap(m_timestamps, other.m_timestamps);
	std::swap(m_private, other.m_private);
	std::swap(m_polluted, other.m_polluted);
}

std::size_t lcsm::Context::size() const noexcept
{
	return m_size;
}

std::size_t lcsm::Context::privateSize() const noexcept
{
	return m_private.size();
}

bool lcsm::Context::polluted() const noexcept
{
	return m_polluted;
}

void lcsm::Context::setPolluted(bool polluted) noexcept
{
	m_polluted = polluted;
}

const lcsm::Timestamp &lcsm::Context::lastUpdate(std::size_t i) const
{
	if (i >= m_size)
	{
		throw std::out_of_range("Context: Index of timestamps is out of bound");
	}
	return m_timestamps[i];
}

const lcsm::DataBits &lcsm::Context::getValue(std::size_t i) const
{
	if (i >= m_size)
	{
		throw std::out_of_range("Context: Index out of bound");
	}
	return m_databits[i];
}

const std::vector< lcsm::DataBits > &lcsm::Context::values() const noexcept
{
	return m_databits;
}

const std::vector< lcsm::Timestamp > &lcsm::Context::timestamps() const noexcept
{
	return m_timestamps;
}

bool lcsm::Context::isEqualsValues(const Context &other) const
{
	/* Verify sizes. */
	if (m_size != other.m_size)
	{
		throw std::logic_error("Context: when compare to other context for values, sizes must be exactly right.");
	}

	/* Check equality. If any is not equals, then return false. */
	for (std::size_t i = 0; i < m_size; i++)
	{
		if (m_databits[i] != other.m_databits[i])
		{
			return false;
		}
	}

	/* Otherwise, return true. */
	return true;
}

bool lcsm::Context::operator==(const lcsm::Context &other) const
{
	return m_polluted == other.m_polluted && isEqualsValues(other);
}

bool lcsm::Context::operator!=(const lcsm::Context &other) const
{
	return !(*this == other);
}

void lcsm::Context::updateValue(std::size_t i, const lcsm::DataBits &databits, lcsm::Timestamp timestamp, bool forced)
{
	if (i >= m_size)
	{
		throw std::out_of_range("Context: index out of bound");
	}

	if (forced)
	{
		m_databits[i] = databits;
		m_timestamps[i] = timestamp;
	}
	else
	{
		if (m_databits[i] != databits)
		{
			m_databits[i] = databits;
			m_timestamps[i] = timestamp;
		}
	}
}

void lcsm::Context::updateValue(std::size_t i, lcsm::DataBits &&databits, lcsm::Timestamp timestamp, bool forced)
{
	if (i >= m_size)
	{
		throw std::out_of_range("Context: index out of bound");
	}

	if (forced)
	{
		m_databits[i] = std::move(databits);
		m_timestamps[i] = timestamp;
	}
	else
	{
		if (m_databits[i] != databits)
		{
			m_databits[i] = std::move(databits);
			m_timestamps[i] = timestamp;
		}
	}
}

void lcsm::Context::updateValues(lcsm::Timestamp timestamp, const std::vector< lcsm::DataBits > &databits)
{
	if (m_size != databits.size())
	{
		throw std::logic_error("Context can't apply different number of values.");
	}

	m_databits = databits;

	for (std::size_t i = 0; i < m_size; i++)
	{
		m_timestamps[i] = timestamp;
	}
}

void lcsm::Context::updateValues(lcsm::Timestamp timestamp, std::vector< lcsm::DataBits > &&databits)
{
	if (m_size != databits.size())
	{
		throw std::logic_error("Context can't apply different number of values.");
	}

	m_databits = std::move(databits);

	for (std::size_t i = 0; i < m_size; i++)
	{
		m_timestamps[i] = timestamp;
	}
}

void lcsm::Context::updateValues(lcsm::Timestamp timestamp, std::initializer_list< lcsm::DataBits > databits)
{
	if (m_size != databits.size())
	{
		throw std::logic_error("Context can't apply different number of values.");
	}

	std::size_t i = 0;
	for (const lcsm::DataBits &db : databits)
	{
		m_databits[i++] = db;
	}

	for (i = 0; i < m_size; i++)
	{
		m_timestamps[i] = timestamp;
	}
}

bool lcsm::Context::neverUpdate(std::size_t i) const
{
	return lastUpdate(i).isReset();
}

lcsm::PrivateContext &lcsm::Context::privateContext() noexcept
{
	return m_private;
}

const lcsm::PrivateContext &lcsm::Context::privateContext() const noexcept
{
	return m_private;
}
