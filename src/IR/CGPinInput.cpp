#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>

#include <stdexcept>
#include <utility>

sim::Value &sim::CGPinInput::Read()
{
	return m_value;
}

const sim::Value &sim::CGPinInput::Read() const
{
	return m_value;
}

void sim::CGPinInput::Write(const sim::Value &value)
{
	if (m_value != value)
		throw std::logic_error("Can't write value to only input Pin.");
}

void sim::CGPinInput::Write(sim::Value &&value)
{
	if (m_value != value)
		std::logic_error("Can't write value to only input Pin.");
}

sim::CGObjectT sim::CGPinInput::T() const noexcept
{
	return sim::CGObjectT::OBJ_PIN_INPUT;
}

void sim::CGPinInput::ExternalWrite(const sim::Value &value)
{
	m_value = value;
}

void sim::CGPinInput::ExternalWrite(sim::Value &&value) noexcept
{
	m_value = std::move(value);
}

const sim::CGWire *sim::CGPinInput::AsWire() const noexcept
{
	return nullptr;
}

sim::CGWire *sim::CGPinInput::AsWire() noexcept
{
	return nullptr;
}

const sim::CGPinInput *sim::CGPinInput::AsPinInput() const noexcept
{
	return this;
}

sim::CGPinInput *sim::CGPinInput::AsPinInput() noexcept
{
	return this;
}

const sim::CGPinOutput *sim::CGPinInput::AsPinOutput() const noexcept
{
	return nullptr;
}

sim::CGPinOutput *sim::CGPinInput::AsPinOutput() noexcept
{
	return nullptr;
}
