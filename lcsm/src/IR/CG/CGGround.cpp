#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

lcsm::CGObjectT lcsm::CGGround::T() const noexcept
{
	return lcsm::CGObjectT::OBJ_GROUND;
}

lcsm::CGGround *lcsm::CGGround::asGround() noexcept
{
	return this;
}

const lcsm::CGGround *lcsm::CGGround::asGround() const noexcept
{
	return this;
}

void lcsm::CGGround::setWidth(lcsm::Width width)
{
	setDataBits(lcsm::DataBits(width));
}
