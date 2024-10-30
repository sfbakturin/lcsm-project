#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

#include <utility>

lcsm::DataBits &lcsm::CGTransistorBase::read()
{
	return m_value;
}

const lcsm::DataBits &lcsm::CGTransistorBase::read() const
{
	return m_value;
}

void lcsm::CGTransistorBase::write(const lcsm::DataBits &value)
{
	m_value = value;
}

void lcsm::CGTransistorBase::write(lcsm::DataBits &&value)
{
	m_value = std::move(value);
}

lcsm::model::Width lcsm::CGTransistorBase::width() const
{
	return m_value.width();
}

bool lcsm::CGTransistorBase::checkWidth(const lcsm::DataBits &value) const
{
	return m_value.width() == value.width();
}

lcsm::CGObjectT lcsm::CGTransistorBase::T() const noexcept
{
	return lcsm::CGObjectT::OBJ_TRANSISTOR_BASE;
}

lcsm::CGTransistorBase *lcsm::CGTransistorBase::asTransistorBase() noexcept
{
	return this;
}

const lcsm::CGTransistorBase *lcsm::CGTransistorBase::asTransistorBase() const noexcept
{
	return this;
}

lcsm::DataBits &lcsm::CGTransistorInout::read()
{
	return m_value;
}

const lcsm::DataBits &lcsm::CGTransistorInout::read() const
{
	return m_value;
}

void lcsm::CGTransistorInout::write(const lcsm::DataBits &value)
{
	m_value = value;
}

void lcsm::CGTransistorInout::write(lcsm::DataBits &&value)
{
	m_value = std::move(value);
}

lcsm::model::Width lcsm::CGTransistorInout::width() const
{
	return m_value.width();
}

bool lcsm::CGTransistorInout::checkWidth(const lcsm::DataBits &value) const
{
	return m_value.width() == value.width();
}

lcsm::CGObjectT lcsm::CGTransistorInout::T() const noexcept
{
	return lcsm::CGObjectT::OBJ_TRANSISTOR_INOUT;
}

lcsm::CGTransistorInout *lcsm::CGTransistorInout::asTransistorInout() noexcept
{
	return this;
}

const lcsm::CGTransistorInout *lcsm::CGTransistorInout::asTransistorInout() const noexcept
{
	return this;
}

lcsm::CGObjectT lcsm::CGTransistorState::T() const noexcept
{
	return lcsm::CGObjectT::OBJ_TRANSISTOR_STATE;
}

lcsm::CGTransistorState *lcsm::CGTransistorState::asTransistorState() noexcept
{
	return this;
}

const lcsm::CGTransistorState *lcsm::CGTransistorState::asTransistorState() const noexcept
{
	return this;
}
