#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Value.h>
#include <lcsm/IR/Width.h>

#include <utility>

lcsm::Value &lcsm::CGWire::read()
{
	return m_value;
}

const lcsm::Value &lcsm::CGWire::read() const
{
	return m_value;
}

void lcsm::CGWire::write(const lcsm::Value &value)
{
	m_value = value;
}

void lcsm::CGWire::write(lcsm::Value &&value)
{
	m_value = std::move(value);
}

lcsm::Width lcsm::CGWire::width() const
{
	return m_value.width();
}

bool lcsm::CGWire::checkWidth(const lcsm::Value &value) const
{
	return value.width() == width();
}

lcsm::CGObjectT lcsm::CGWire::T() const noexcept
{
	return lcsm::CGObjectT::OBJ_WIRE;
}

lcsm::CGWire *lcsm::CGWire::asWire() noexcept
{
	return this;
}

const lcsm::CGWire *lcsm::CGWire::asWire() const noexcept
{
	return this;
}
