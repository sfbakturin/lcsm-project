#include <lcsm/IR/Bit.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Value.h>
#include <lcsm/IR/Width.h>

lcsm::CGObjectT lcsm::CGPower::T() const noexcept
{
	return lcsm::CGObjectT::OBJ_POWER;
}

lcsm::CGPower *lcsm::CGPower::asPower() noexcept
{
	return this;
}

const lcsm::CGPower *lcsm::CGPower::asPower() const noexcept
{
	return this;
}

void lcsm::CGPower::setWidth(lcsm::Width width)
{
	setValue(lcsm::Value(width, lcsm::LogisimBit::LOGISIM_TRUE));
}
