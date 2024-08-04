#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>

#include <stdexcept>
#include <utility>

sim::Value &sim::CGPinOutput::Read()
{
	throw std::logic_error("Can't write value from only output Pin.");
}

const sim::Value &sim::CGPinOutput::Read() const
{
	throw std::logic_error("Can't write value from only output Pin.");
}

void sim::CGPinOutput::Write(const sim::Value &value)
{
	m_value = value;
}

void sim::CGPinOutput::Write(sim::Value &&value)
{
	m_value = std::move(value);
}

sim::CGObjectT sim::CGPinOutput::T() const noexcept
{
	return sim::CGObjectT::OBJ_PIN_OUTPUT;
}

const sim::Value &sim::CGPinOutput::ExternalRead() const noexcept
{
	return m_value;
}

sim::Value &sim::CGPinOutput::ExternalRead() noexcept
{
	return m_value;
}

const sim::CGWire *sim::CGPinOutput::AsWire() const noexcept
{
	return nullptr;
}

sim::CGWire *sim::CGPinOutput::AsWire() noexcept
{
	return nullptr;
}

const sim::CGPinInput *sim::CGPinOutput::AsPinInput() const noexcept
{
	return nullptr;
}

sim::CGPinInput *sim::CGPinOutput::AsPinInput() noexcept
{
	return nullptr;
}

const sim::CGPinOutput *sim::CGPinOutput::AsPinOutput() const noexcept
{
	return this;
}

sim::CGPinOutput *sim::CGPinOutput::AsPinOutput() noexcept
{
	return this;
}
