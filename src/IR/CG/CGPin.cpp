#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>
#include <sim/IR/Width.h>

#include <utility>

sim::CGPin::CGPin(const sim::Value &value) : m_value(value) {}

sim::CGPin::CGPin(sim::Value &&value) noexcept : m_value(std::move(value)) {}

sim::Width sim::CGPin::width() const noexcept
{
	return m_value.width();
}

bool sim::CGPin::checkWidth(const sim::Value &value) const noexcept
{
	return value.width() == width();
}
