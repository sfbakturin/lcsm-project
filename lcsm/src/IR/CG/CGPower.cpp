#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Verilog/Bit.h>

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
	setDataBits(lcsm::DataBits(width, lcsm::verilog::Bit::True));
}
