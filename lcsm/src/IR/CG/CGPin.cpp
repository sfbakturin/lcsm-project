#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Value.h>
#include <lcsm/IR/Width.h>

#include <utility>

lcsm::CGPin::CGPin(const lcsm::Value &value) : m_value(value) {}

lcsm::CGPin::CGPin(lcsm::Value &&value) noexcept : m_value(std::move(value)) {}

lcsm::Width lcsm::CGPin::width() const
{
	return m_value.width();
}

bool lcsm::CGPin::checkWidth(const lcsm::Value &value) const
{
	return value.width() == width();
}
