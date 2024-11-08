#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>

#include <stdexcept>
#include <utility>

lcsm::DataBits &lcsm::CGPinOutput::read()
{
	// TODO: Maybe, it should be as real inout element.
	throw std::logic_error("Can't write value from only output Pin.");
}

const lcsm::DataBits &lcsm::CGPinOutput::read() const
{
	// TODO: Maybe, it should be as real inout element.
	throw std::logic_error("Can't write value from only output Pin.");
}

void lcsm::CGPinOutput::write(const lcsm::DataBits &value)
{
	m_value = value;
}

void lcsm::CGPinOutput::write(lcsm::DataBits &&value)
{
	m_value = std::move(value);
}

lcsm::CGObjectType lcsm::CGPinOutput::objectType() const noexcept
{
	return lcsm::CGObjectType::PinOutput;
}

const lcsm::DataBits &lcsm::CGPinOutput::externalRead() const noexcept
{
	return m_value;
}

lcsm::DataBits &lcsm::CGPinOutput::externalRead() noexcept
{
	return m_value;
}

lcsm::CGPinOutput *lcsm::CGPinOutput::asPinOutput() noexcept
{
	return this;
}

const lcsm::CGPinOutput *lcsm::CGPinOutput::asPinOutput() const noexcept
{
	return this;
}
