#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Value.h>

#include <stdexcept>
#include <utility>

lcsm::Value &lcsm::CGPinInput::read()
{
	return m_value;
}

const lcsm::Value &lcsm::CGPinInput::read() const
{
	return m_value;
}

void lcsm::CGPinInput::write(const lcsm::Value &value)
{
	if (m_value != value)
		throw std::logic_error("Can't write value to only input Pin.");
}

void lcsm::CGPinInput::write(lcsm::Value &&value)
{
	if (m_value != value)
		std::logic_error("Can't write value to only input Pin.");
}

lcsm::CGObjectT lcsm::CGPinInput::T() const noexcept
{
	return lcsm::CGObjectT::OBJ_PIN_INPUT;
}

void lcsm::CGPinInput::externalWrite(const lcsm::Value &value)
{
	m_value = value;
}

void lcsm::CGPinInput::externalWrite(lcsm::Value &&value) noexcept
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
