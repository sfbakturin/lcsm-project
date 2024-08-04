#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>
#include <sim/IR/Width.h>

#include <utility>

sim::CGPin::CGPin(const sim::Value &value) : m_value(value) {}

sim::CGPin::CGPin(sim::Value &&value) noexcept : m_value(std::move(value)) {}

sim::Width sim::CGPin::GetWidth() const noexcept
{
	return m_value.GetWidth();
}

bool sim::CGPin::CheckWidth(const sim::Value &value) const noexcept
{
	return value.GetWidth() == GetWidth();
}
