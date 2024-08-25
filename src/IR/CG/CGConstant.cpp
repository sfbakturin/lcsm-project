#include <sim/IR/Bit.h>
#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>
#include <sim/IR/Width.h>

#include <stdexcept>

sim::Value &sim::CGConstant::read()
{
	return m_value;
}

const sim::Value &sim::CGConstant::read() const
{
	return m_value;
}

void sim::CGConstant::write(const sim::Value &)
{
	throw std::logic_error("Not implemented");
}

void sim::CGConstant::write(sim::Value &&)
{
	throw std::logic_error("Not implemented");
}

sim::Width sim::CGConstant::width() const noexcept
{
	return m_value.width();
}

bool sim::CGConstant::checkWidth(const sim::Value &value) const noexcept
{
	return value.width() == width();
}

sim::CGObjectT sim::CGConstant::T() const noexcept
{
	return sim::CGObjectT::OBJ_CONSTANT;
}

sim::CGConstant *sim::CGConstant::asConstant() noexcept
{
	return this;
}

const sim::CGConstant *sim::CGConstant::asConstant() const noexcept
{
	return this;
}

void sim::CGConstant::emplaceValue(sim::Width width, std::uint64_t value)
{
	m_value.setWidth(width);
	for (std::size_t i = 0; i < width; i++)
	{
		sim::LogisimBit bit =
			value & 1 ? sim::LogisimBit::LOGISIM_TRUE : sim::LogisimBit::LOGISIM_FALSE;
		m_value.setBit(i, bit);
		value >>= 1;
	}
}

void sim::CGConstant::setValue(const sim::Value &value)
{
	m_value = value;
}

void sim::CGConstant::setValue(sim::Value &&value) noexcept
{
	m_value = std::move(value);
}
