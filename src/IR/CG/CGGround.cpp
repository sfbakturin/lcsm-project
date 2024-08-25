#include <sim/IR/Bit.h>
#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>
#include <sim/IR/Width.h>

sim::CGObjectT sim::CGGround::T() const noexcept
{
	return sim::CGObjectT::OBJ_GROUND;
}

sim::CGGround *sim::CGGround::asGround() noexcept
{
	return this;
}

const sim::CGGround *sim::CGGround::asGround() const noexcept
{
	return this;
}

void sim::CGGround::setWidth(sim::Width width)
{
	setValue(sim::Value(width, sim::LogisimBit::LOGISIM_FALSE));
}
