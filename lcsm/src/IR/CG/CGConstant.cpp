#include <lcsm/IR/Bit.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Value.h>
#include <lcsm/IR/Width.h>

#include <stdexcept>

lcsm::Value &lcsm::CGConstant::read()
{
	return m_value;
}

const lcsm::Value &lcsm::CGConstant::read() const
{
	return m_value;
}

void lcsm::CGConstant::write(const lcsm::Value &)
{
	throw std::logic_error("Not implemented");
}

void lcsm::CGConstant::write(lcsm::Value &&)
{
	throw std::logic_error("Not implemented");
}

lcsm::Width lcsm::CGConstant::width() const
{
	return m_value.width();
}

bool lcsm::CGConstant::checkWidth(const lcsm::Value &value) const
{
	return value.width() == width();
}

lcsm::CGObjectT lcsm::CGConstant::T() const noexcept
{
	return lcsm::CGObjectT::OBJ_CONSTANT;
}

lcsm::CGConstant *lcsm::CGConstant::asConstant() noexcept
{
	return this;
}

const lcsm::CGConstant *lcsm::CGConstant::asConstant() const noexcept
{
	return this;
}

void lcsm::CGConstant::emplaceValue(lcsm::Width width, std::uint64_t value)
{
	m_value.setWidth(width);
	for (std::size_t i = 0; i < width; i++)
	{
		lcsm::LogisimBit bit = value & 1 ? lcsm::LogisimBit::LOGISIM_TRUE : lcsm::LogisimBit::LOGISIM_FALSE;
		m_value.setBit(i, bit);
		value >>= 1;
	}
}

void lcsm::CGConstant::setValue(const lcsm::Value &value)
{
	m_value = value;
}

void lcsm::CGConstant::setValue(lcsm::Value &&value) noexcept
{
	m_value = std::move(value);
}
