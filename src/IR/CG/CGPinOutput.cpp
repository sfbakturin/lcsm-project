#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>

#include <stdexcept>
#include <utility>

sim::Value &sim::CGPinOutput::read()
{
	throw std::logic_error("Can't write value from only output Pin.");
}

const sim::Value &sim::CGPinOutput::read() const
{
	throw std::logic_error("Can't write value from only output Pin.");
}

void sim::CGPinOutput::write(const sim::Value &value)
{
	m_value = value;
}

void sim::CGPinOutput::write(sim::Value &&value)
{
	m_value = std::move(value);
}

sim::CGObjectT sim::CGPinOutput::T() const noexcept
{
	return sim::CGObjectT::OBJ_PIN_OUTPUT;
}

const sim::Value &sim::CGPinOutput::externalRead() const noexcept
{
	return m_value;
}

sim::Value &sim::CGPinOutput::externalRead() noexcept
{
	return m_value;
}

sim::CGWire *sim::CGPinOutput::asWire() noexcept
{
	return nullptr;
}

const sim::CGWire *sim::CGPinOutput::asWire() const noexcept
{
	return nullptr;
}

sim::CGPinInput *sim::CGPinOutput::asPinInput() noexcept
{
	return nullptr;
}

const sim::CGPinInput *sim::CGPinOutput::asPinInput() const noexcept
{
	return nullptr;
}

sim::CGPinOutput *sim::CGPinOutput::asPinOutput() noexcept
{
	return this;
}

const sim::CGPinOutput *sim::CGPinOutput::asPinOutput() const noexcept
{
	return this;
}
