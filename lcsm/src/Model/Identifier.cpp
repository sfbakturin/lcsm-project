#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/Algorithm.hpp>

#include <cstddef>
#include <ostream>
#include <string>
#include <utility>

lcsm::Identifier::Identifier() noexcept : m_id(0) {}

lcsm::Identifier::Identifier(lcsm::Identifier::value_type startId) noexcept : m_id(startId) {}

lcsm::Identifier::Identifier(const Identifier &other) noexcept : m_id(other.m_id) {}

lcsm::Identifier::Identifier(Identifier &&other) noexcept : m_id(other.m_id) {}

lcsm::Identifier &lcsm::Identifier::operator=(const lcsm::Identifier &other) noexcept
{
	return lcsm::support::CopyAssign< lcsm::Identifier >(this, other);
}

lcsm::Identifier &lcsm::Identifier::operator=(lcsm::Identifier &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::Identifier >(this, std::move(other));
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

std::string lcsm::Identifier::toString() const
{
	return std::to_string(m_id);
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

namespace lcsm
{
	std::ostream &operator<<(std::ostream &os, const lcsm::Identifier &id)
	{
		return os << id.m_id;
	}
}	 // namespace lcsm
