#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Timestamp.h>

#include <memory>
#include <utility>

lcsm::Context::Context(const lcsm::Timestamp &timestamp) noexcept : m_timestamp(timestamp) {}

lcsm::Context::Context(lcsm::Timestamp &&timestamp) noexcept : m_timestamp(std::move(timestamp)) {}

lcsm::Context::Context(const lcsm::Timestamp &timestamp, const lcsm::DataBits &databits) noexcept :
	m_timestamp(timestamp), m_databits(databits)
{
}

lcsm::Context::Context(lcsm::Timestamp &&timestamp, const lcsm::DataBits &databits) noexcept :
	m_timestamp(std::move(timestamp)), m_databits(databits)
{
}

lcsm::Context::Context(const lcsm::Timestamp &timestamp, lcsm::DataBits &&databits) noexcept :
	m_timestamp(timestamp), m_databits(std::move(databits))
{
}

lcsm::Context::Context(lcsm::Timestamp &&timestamp, lcsm::DataBits &&databits) noexcept :
	m_timestamp(std::move(timestamp)), m_databits(std::move(databits))
{
}

lcsm::Context::Context(const lcsm::Context &other) noexcept :
	m_timestamp(other.m_timestamp), m_databits(other.m_databits)
{
}

lcsm::Context::Context(lcsm::Context &&other) noexcept :
	m_timestamp(std::move(other.m_timestamp)), m_databits(std::move(other.m_databits))
{
}

lcsm::Context &lcsm::Context::operator=(const lcsm::Context &other) noexcept
{
	if (this != std::addressof(other))
		lcsm::Context(other).swap(*this);
	return *this;
}

lcsm::Context &lcsm::Context::operator=(lcsm::Context &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::Context(std::move(other)).swap(*this);
	return *this;
}

void lcsm::Context::swap(lcsm::Context &other) noexcept
{
	std::swap(m_timestamp, other.m_timestamp);
	std::swap(m_databits, other.m_databits);
}

const lcsm::Timestamp &lcsm::Context::lastUpdate() const noexcept
{
	return m_timestamp;
}

const lcsm::DataBits &lcsm::Context::getValue() const noexcept
{
	return m_databits;
}

void lcsm::Context::updateValue(const lcsm::Timestamp &timestamp, const lcsm::DataBits &databits) noexcept
{
	m_timestamp = timestamp;
	m_databits = databits;
}

void lcsm::Context::updateValue(lcsm::Timestamp &&timestamp, const lcsm::DataBits &databits) noexcept
{
	m_timestamp = std::move(timestamp);
	m_databits = databits;
}

void lcsm::Context::updateValue(const lcsm::Timestamp &timestamp, lcsm::DataBits &&databits) noexcept
{
	m_timestamp = timestamp;
	m_databits = std::move(databits);
}

void lcsm::Context::updateValue(lcsm::Timestamp &&timestamp, lcsm::DataBits &&databits) noexcept
{
	m_timestamp = std::move(timestamp);
	m_databits = std::move(databits);
}

bool lcsm::Context::neverUpdate() const noexcept
{
	return m_timestamp.isReset();
}
