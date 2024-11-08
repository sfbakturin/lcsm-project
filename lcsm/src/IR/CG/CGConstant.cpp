#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

#include <stdexcept>

lcsm::DataBits &lcsm::CGConstant::read()
{
	return m_value;
}

const lcsm::DataBits &lcsm::CGConstant::read() const
{
	return m_value;
}

void lcsm::CGConstant::write(const lcsm::DataBits &)
{
	// Scheduler must prevent attempts to write to the Constant.
	throw std::logic_error("Attempt to write to the Constant element.");
}

void lcsm::CGConstant::write(lcsm::DataBits &&)
{
	// Scheduler must prevent attempts to write to the Constant.
	throw std::logic_error("Attempt to write to the Constant element.");
}

lcsm::model::Width lcsm::CGConstant::width() const
{
	return m_value.width();
}

bool lcsm::CGConstant::checkWidth(const lcsm::DataBits &value) const
{
	return value.width() == width();
}

lcsm::CGObjectType lcsm::CGConstant::objectType() const noexcept
{
	return lcsm::CGObjectType::Constant;
}

lcsm::CGConstant *lcsm::CGConstant::asConstant() noexcept
{
	return this;
}

const lcsm::CGConstant *lcsm::CGConstant::asConstant() const noexcept
{
	return this;
}

void lcsm::CGConstant::emplaceDataBits(lcsm::model::Width, std::uint64_t)
{
	// TODO: Do we really need this function?
}

void lcsm::CGConstant::setDataBits(const lcsm::DataBits &value)
{
	m_value = value;
}

void lcsm::CGConstant::setDataBits(lcsm::DataBits &&value) noexcept
{
	m_value = std::move(value);
}
