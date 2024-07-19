#include <sim/Model/Circuit/Transistor.h>

#include <utility>

sim::Transistor::Transistor() noexcept : m_type(sim::TransistorType::P) {}
sim::Transistor::Transistor(TransistorType type) noexcept : m_type(type) {}

sim::Transistor::Transistor(const Transistor &other) noexcept : m_type(other.m_type) {}
sim::Transistor::Transistor(Transistor &&other) noexcept : m_type(other.m_type) {}

sim::Transistor &sim::Transistor::operator=(const sim::Transistor &other) noexcept
{
	if (this != &other)
		sim::Transistor(other).Swap(*this);
	return *this;
}
sim::Transistor &sim::Transistor::operator=(sim::Transistor &&other)
{
	if (this != &other)
		sim::Transistor(std::move(other)).Swap(*this);
	return *this;
}

void sim::Transistor::Swap(sim::Transistor &other) noexcept
{
	std::swap(m_type, other.m_type);
}
