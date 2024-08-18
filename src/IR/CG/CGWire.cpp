#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>
#include <sim/IR/Width.h>

#include <utility>

sim::Value &sim::CGWire::read()
{
	return m_value;
}

const sim::Value &sim::CGWire::read() const
{
	return m_value;
}

void sim::CGWire::write(const sim::Value &value)
{
	m_value = value;
}

void sim::CGWire::write(sim::Value &&value)
{
	m_value = std::move(value);
}

sim::Width sim::CGWire::width() const noexcept
{
	return m_value.width();
}

bool sim::CGWire::checkWidth(const sim::Value &value) const noexcept
{
	return value.width() == width();
}

sim::CGObjectT sim::CGWire::T() const noexcept
{
	return sim::CGObjectT::OBJ_WIRE;
}

sim::CGWire *sim::CGWire::asWire() noexcept
{
	return this;
}

const sim::CGWire *sim::CGWire::asWire() const noexcept
{
	return this;
}

sim::CGPinInput *sim::CGWire::asPinInput() noexcept
{
	return nullptr;
}

const sim::CGPinInput *sim::CGWire::asPinInput() const noexcept
{
	return nullptr;
}

sim::CGPinOutput *sim::CGWire::asPinOutput() noexcept
{
	return nullptr;
}

const sim::CGPinOutput *sim::CGWire::asPinOutput() const noexcept
{
	return nullptr;
}
