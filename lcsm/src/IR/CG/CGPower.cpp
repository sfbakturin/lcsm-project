#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Verilog/Bit.h>

lcsm::CGObjectType lcsm::CGPower::objectType() const noexcept
{
	return lcsm::CGObjectType::Power;
}

lcsm::CGPower *lcsm::CGPower::asPower() noexcept
{
	return this;
}

const lcsm::CGPower *lcsm::CGPower::asPower() const noexcept
{
	return this;
}

void lcsm::CGPower::setWidth(lcsm::model::Width width)
{
	setDataBits(lcsm::DataBits(width, lcsm::verilog::Bit::True));
}
