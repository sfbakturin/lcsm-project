#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>

#include <stdexcept>
#include <utility>

lcsm::DataBits &lcsm::CGPinInput::read()
{
	return m_value;
}

const lcsm::DataBits &lcsm::CGPinInput::read() const
{
	return m_value;
}

void lcsm::CGPinInput::write(const lcsm::DataBits &value)
{
	// FIXME: if bit of value is not equals m_value AND signal strength of value is more, then it's error.
	// if (m_value != value)
	// throw std::logic_error("Can't write value to only input Pin.");
}

void lcsm::CGPinInput::write(lcsm::DataBits &&value)
{
	// FIXME: if bit of value is not equals m_value AND signal strength of value is more, then it's error.
	// if (m_value != value)
	// std::logic_error("Can't write value to only input Pin.");
}

lcsm::CGObjectT lcsm::CGPinInput::T() const noexcept
{
	return lcsm::CGObjectT::OBJ_PIN_INPUT;
}

void lcsm::CGPinInput::externalWrite(const lcsm::DataBits &value)
{
	m_value = value;
}

void lcsm::CGPinInput::externalWrite(lcsm::DataBits &&value) noexcept
{
	m_value = std::move(value);
}

lcsm::CGPinInput *lcsm::CGPinInput::asPinInput() noexcept
{
	return this;
}

const lcsm::CGPinInput *lcsm::CGPinInput::asPinInput() const noexcept
{
	return this;
}
