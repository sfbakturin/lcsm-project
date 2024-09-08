#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>
#include <sim/IR/Width.h>

#include <utility>

sim::Value &sim::CGTransistorBase::read()
{
	return m_value;
}

const sim::Value &sim::CGTransistorBase::read() const
{
	return m_value;
}

void sim::CGTransistorBase::write(const sim::Value &value)
{
	m_value = value;
}

void sim::CGTransistorBase::write(sim::Value &&value)
{
	m_value = std::move(value);
}

sim::Width sim::CGTransistorBase::width() const
{
	return m_value.width();
}

bool sim::CGTransistorBase::checkWidth(const sim::Value &value) const
{
	return m_value.width() == value.width();
}

sim::CGObjectT sim::CGTransistorBase::T() const noexcept
{
	return sim::CGObjectT::OBJ_TRANSISTOR_BASE;
}

sim::CGTransistorBase *sim::CGTransistorBase::asTransistorBase() noexcept
{
	return this;
}

const sim::CGTransistorBase *sim::CGTransistorBase::asTransistorBase() const noexcept
{
	return this;
}

sim::Value &sim::CGTransistorInout::read()
{
	return m_value;
}

const sim::Value &sim::CGTransistorInout::read() const
{
	return m_value;
}

void sim::CGTransistorInout::write(const sim::Value &value)
{
	m_value = value;
}

void sim::CGTransistorInout::write(sim::Value &&value)
{
	m_value = std::move(value);
}

sim::Width sim::CGTransistorInout::width() const
{
	return m_value.width();
}

bool sim::CGTransistorInout::checkWidth(const sim::Value &value) const
{
	return m_value.width() == value.width();
}

sim::CGObjectT sim::CGTransistorInout::T() const noexcept
{
	return sim::CGObjectT::OBJ_TRANSISTOR_INOUT;
}

sim::CGTransistorInout *sim::CGTransistorInout::asTransistorInout() noexcept
{
	return this;
}

const sim::CGTransistorInout *sim::CGTransistorInout::asTransistorInout() const noexcept
{
	return this;
}

sim::CGObjectT sim::CGTransistorState::T() const noexcept
{
	return sim::CGObjectT::OBJ_TRANSISTOR_STATE;
}

sim::CGTransistorState *sim::CGTransistorState::asTransistorState() noexcept
{
	return this;
}

const sim::CGTransistorState *sim::CGTransistorState::asTransistorState() const noexcept
{
	return this;
}
