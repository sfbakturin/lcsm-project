#include <sim/Component/Identifier.h>

#include <cstddef>
#include <memory>
#include <utility>

sim::Identifier::Identifier() noexcept : m_id(0) {}

sim::Identifier::Identifier(const Identifier &other) noexcept : m_id(other.m_id)
{
}

sim::Identifier::Identifier(Identifier &&other) noexcept : m_id(other.m_id) {}

sim::Identifier &sim::Identifier::operator=(const sim::Identifier &other) noexcept
{
	if (this != std::addressof(other))
		sim::Identifier(other).swap(*this);
	return *this;
}

sim::Identifier &sim::Identifier::operator=(sim::Identifier &&other) noexcept
{
	if (this != std::addressof(other))
		sim::Identifier(std::move(other)).swap(*this);
	return *this;
}

void sim::Identifier::swap(sim::Identifier &other) noexcept
{
	std::swap(m_id, other.m_id);
}

sim::Identifier sim::Identifier::next() const noexcept
{
	return { m_id + 1 };
}

std::size_t sim::Identifier::hashCode() const noexcept
{
	return static_cast< std::size_t >(m_id);
}

bool sim::Identifier::operator<(const sim::Identifier &other) const noexcept
{
	return m_id < other.m_id;
}

bool sim::Identifier::operator>(const sim::Identifier &other) const noexcept
{
	return m_id > other.m_id;
}

bool sim::Identifier::operator==(const sim::Identifier &other) const noexcept
{
	return m_id == other.m_id;
}

bool sim::Identifier::operator!=(const sim::Identifier &other) const noexcept
{
	return m_id != other.m_id;
}

bool sim::Identifier::operator<=(const sim::Identifier &other) const noexcept
{
	return m_id <= other.m_id;
}

bool sim::Identifier::operator>=(const sim::Identifier &other) const noexcept
{
	return m_id >= other.m_id;
}

sim::Identifier::Identifier(sim::Identifier::value_type id) noexcept : m_id(id)
{
}
