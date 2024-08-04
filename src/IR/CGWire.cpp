#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>
#include <sim/IR/Width.h>

#include <utility>

sim::Value &sim::CGWire::Read()
{
	return m_value;
}

const sim::Value &sim::CGWire::Read() const
{
	return m_value;
}

void sim::CGWire::Write(const sim::Value &value)
{
	m_value = value;
}

void sim::CGWire::Write(sim::Value &&value)
{
	m_value = std::move(value);
}

sim::Width sim::CGWire::GetWidth() const noexcept
{
	return m_value.GetWidth();
}

bool sim::CGWire::CheckWidth(const sim::Value &value) const noexcept
{
	return value.GetWidth() == GetWidth();
}

sim::CGObjectT sim::CGWire::T() const noexcept
{
	return sim::CGObjectT::OBJ_WIRE;
}

const sim::CGWire *sim::CGWire::AsWire() const noexcept
{
	return this;
}

sim::CGWire *sim::CGWire::AsWire() noexcept
{
	return this;
}

const sim::CGPinInput *sim::CGWire::AsPinInput() const noexcept
{
	return nullptr;
}

sim::CGPinInput *sim::CGWire::AsPinInput() noexcept
{
	return nullptr;
}

const sim::CGPinOutput *sim::CGWire::AsPinOutput() const noexcept
{
	return nullptr;
}

sim::CGPinOutput *sim::CGWire::AsPinOutput() noexcept
{
	return nullptr;
}
