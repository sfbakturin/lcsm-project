#include <lcsm/Component/Identifier.h>

#include <cstddef>
#include <memory>
#include <utility>

lcsm::Identifier::Identifier() noexcept : m_id(0) {}

lcsm::Identifier::Identifier(const Identifier &other) noexcept : m_id(other.m_id) {}

lcsm::Identifier::Identifier(Identifier &&other) noexcept : m_id(other.m_id) {}

lcsm::Identifier &lcsm::Identifier::operator=(const lcsm::Identifier &other) noexcept
{
	if (this != std::addressof(other))
		lcsm::Identifier(other).swap(*this);
	return *this;
}

lcsm::Identifier &lcsm::Identifier::operator=(lcsm::Identifier &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::Identifier(std::move(other)).swap(*this);
	return *this;
}

void lcsm::Identifier::swap(lcsm::Identifier &other) noexcept
{
	std::swap(m_id, other.m_id);
}

lcsm::Identifier lcsm::Identifier::next() const noexcept
{
	return { m_id + 1 };
}

std::size_t lcsm::Identifier::hashCode() const noexcept
{
	return static_cast< std::size_t >(m_id);
}

bool lcsm::Identifier::operator<(const lcsm::Identifier &other) const noexcept
{
	return m_id < other.m_id;
}

bool lcsm::Identifier::operator>(const lcsm::Identifier &other) const noexcept
{
	return m_id > other.m_id;
}

bool lcsm::Identifier::operator==(const lcsm::Identifier &other) const noexcept
{
	return m_id == other.m_id;
}

bool lcsm::Identifier::operator!=(const lcsm::Identifier &other) const noexcept
{
	return m_id != other.m_id;
}

bool lcsm::Identifier::operator<=(const lcsm::Identifier &other) const noexcept
{
	return m_id <= other.m_id;
}

bool lcsm::Identifier::operator>=(const lcsm::Identifier &other) const noexcept
{
	return m_id >= other.m_id;
}

lcsm::Identifier::Identifier(lcsm::Identifier::value_type id) noexcept : m_id(id) {}
