#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>

#include <stdexcept>
#include <utility>

sim::Value &sim::CGPinInput::read()
{
	return m_value;
}

const sim::Value &sim::CGPinInput::read() const
{
	return m_value;
}

void sim::CGPinInput::write(const sim::Value &value)
{
	if (m_value != value)
		throw std::logic_error("Can't write value to only input Pin.");
}

void sim::CGPinInput::write(sim::Value &&value)
{
	if (m_value != value)
		std::logic_error("Can't write value to only input Pin.");
}

sim::CGObjectT sim::CGPinInput::T() const noexcept
{
	return sim::CGObjectT::OBJ_PIN_INPUT;
}

void sim::CGPinInput::externalWrite(const sim::Value &value)
{
	m_value = value;
}

void sim::CGPinInput::externalWrite(sim::Value &&value) noexcept
{
	m_value = std::move(value);
}

sim::CGWire *sim::CGPinInput::asWire() noexcept
{
	return nullptr;
}

const sim::CGWire *sim::CGPinInput::asWire() const noexcept
{
	return nullptr;
}

sim::CGPinInput *sim::CGPinInput::asPinInput() noexcept
{
	return this;
}

const sim::CGPinInput *sim::CGPinInput::asPinInput() const noexcept
{
	return this;
}

sim::CGPinOutput *sim::CGPinInput::asPinOutput() noexcept
{
	return nullptr;
}

const sim::CGPinOutput *sim::CGPinInput::asPinOutput() const noexcept
{
	return nullptr;
}
