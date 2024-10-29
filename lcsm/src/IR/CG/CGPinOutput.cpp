#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Value.h>

#include <stdexcept>
#include <utility>

lcsm::Value &lcsm::CGPinOutput::read()
{
	throw std::logic_error("Can't write value from only output Pin.");
}

const lcsm::Value &lcsm::CGPinOutput::read() const
{
	throw std::logic_error("Can't write value from only output Pin.");
}

void lcsm::CGPinOutput::write(const lcsm::Value &value)
{
	m_value = value;
}

void lcsm::CGPinOutput::write(lcsm::Value &&value)
{
	m_value = std::move(value);
}

lcsm::CGObjectT lcsm::CGPinOutput::T() const noexcept
{
	return lcsm::CGObjectT::OBJ_PIN_OUTPUT;
}

const lcsm::Value &lcsm::CGPinOutput::externalRead() const noexcept
{
	return m_value;
}

lcsm::Value &lcsm::CGPinOutput::externalRead() noexcept
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
