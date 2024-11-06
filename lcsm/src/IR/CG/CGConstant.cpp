#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

#include <stdexcept>

lcsm::DataBits &lcsm::CGConstant::read()
{
	return m_value;
}

const lcsm::DataBits &lcsm::CGConstant::read() const
{
	return m_value;
}

void lcsm::CGConstant::write(const lcsm::DataBits &)
{
	throw std::logic_error("Not implemented");
}

void lcsm::CGConstant::write(lcsm::DataBits &&)
{
	throw std::logic_error("Not implemented");
}

lcsm::model::Width lcsm::CGConstant::width() const
{
	return m_value.width();
}

bool lcsm::CGConstant::checkWidth(const lcsm::DataBits &value) const
{
	return value.width() == width();
}

lcsm::CGObjectType lcsm::CGConstant::objectType() const noexcept
{
	return lcsm::CGObjectType::Constant;
}

lcsm::CGConstant *lcsm::CGConstant::asConstant() noexcept
{
	return this;
}

const lcsm::CGConstant *lcsm::CGConstant::asConstant() const noexcept
{
	return this;
}

void lcsm::CGConstant::emplaceDataBits(lcsm::model::Width width, std::uint64_t value)
{
	// TODO: emplace value with width.
	// m_value.setWidth(width);
	// for (std::size_t i = 0; i < width; i++)
	// {
	// lcsm::LogisimBit bit = value & 1 ? lcsm::LogisimBit::LOGISIM_TRUE : lcsm::LogisimBit::LOGISIM_FALSE;
	// m_value.setBit(i, bit);
	// value >>= 1;
	// }
}

void lcsm::CGConstant::setDataBits(const lcsm::DataBits &value)
{
	m_value = value;
}

void lcsm::CGConstant::setDataBits(lcsm::DataBits &&value) noexcept
{
	m_value = std::move(value);
}
