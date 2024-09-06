#include <sim/IR/CGObject.h>

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
