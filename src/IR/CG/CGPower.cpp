#include <sim/IR/Bit.h>
#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>
#include <sim/IR/Width.h>

sim::CGObjectT sim::CGPower::T() const noexcept
{
	return sim::CGObjectT::OBJ_POWER;
}

sim::CGPower *sim::CGPower::asPower() noexcept
{
	return this;
}

const sim::CGPower *sim::CGPower::asPower() const noexcept
{
	return this;
}

void sim::CGPower::setWidth(sim::Width width)
{
	setValue(sim::Value(width, sim::LogisimBit::LOGISIM_TRUE));
}
