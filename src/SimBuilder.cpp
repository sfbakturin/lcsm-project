#include <sim/SimBuilder.h>
#include <sim/SimContext.h>

#include <utility>

sim::SimBuilder::SimBuilder(sim::SimContext &context) noexcept : m_context(context) {}

sim::SimBuilder::SimBuilder(const sim::SimBuilder &other) noexcept : m_context(other.m_context) {}
sim::SimBuilder::SimBuilder(sim::SimBuilder &&other) noexcept : m_context(other.m_context) {}

sim::SimBuilder &sim::SimBuilder::operator=(const sim::SimBuilder &other) noexcept
{
	if (this != &other)
		sim::SimBuilder(other).Swap(*this);
	return *this;
}
sim::SimBuilder &sim::SimBuilder::operator=(sim::SimBuilder &&other) noexcept
{
	if (this != &other)
		sim::SimBuilder(std::move(other)).Swap(*this);
	return *this;
}

void sim::SimBuilder::Swap(sim::SimBuilder &other) noexcept
{
	std::swap(m_context, other.m_context);
}
