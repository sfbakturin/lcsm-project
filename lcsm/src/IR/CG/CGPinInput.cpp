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

void lcsm::CGPinInput::write(const lcsm::DataBits &)
{
	// Scheduler must prevent attempts to write to the input Pin.
	throw std::logic_error("Attempt to write to the input Pin element.");
}

void lcsm::CGPinInput::write(lcsm::DataBits &&)
{
	// Scheduler must prevent attempts to write to the input Pin.
	throw std::logic_error("Attempt to write to the input Pin element.");
}

lcsm::CGObjectType lcsm::CGPinInput::objectType() const noexcept
{
	return lcsm::CGObjectType::PinInput;
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
