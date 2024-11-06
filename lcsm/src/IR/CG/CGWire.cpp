#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

#include <utility>

lcsm::DataBits &lcsm::CGWire::read()
{
	return m_value;
}

const lcsm::DataBits &lcsm::CGWire::read() const
{
	return m_value;
}

void lcsm::CGWire::write(const lcsm::DataBits &value)
{
	m_value = value;
}

void lcsm::CGWire::write(lcsm::DataBits &&value)
{
	m_value = std::move(value);
}

lcsm::model::Width lcsm::CGWire::width() const
{
	return m_value.width();
}

bool lcsm::CGWire::checkWidth(const lcsm::DataBits &value) const
{
	return value.width() == width();
}

lcsm::CGObjectType lcsm::CGWire::objectType() const noexcept
{
	return lcsm::CGObjectType::Wire;
}

lcsm::CGWire *lcsm::CGWire::asWire() noexcept
{
	return this;
}

const lcsm::CGWire *lcsm::CGWire::asWire() const noexcept
{
	return this;
}
