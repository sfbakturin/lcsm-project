#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

#include <utility>

lcsm::CGPin::CGPin(const lcsm::DataBits &value) : m_value(value) {}

lcsm::CGPin::CGPin(lcsm::DataBits &&value) noexcept : m_value(std::move(value)) {}

lcsm::model::Width lcsm::CGPin::width() const
{
	return m_value.width();
}

bool lcsm::CGPin::checkWidth(const lcsm::DataBits &value) const
{
	return value.width() == width();
}
