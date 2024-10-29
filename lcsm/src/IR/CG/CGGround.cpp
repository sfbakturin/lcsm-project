#include <lcsm/IR/Bit.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Value.h>
#include <lcsm/IR/Width.h>

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
	setValue(lcsm::Value(width, lcsm::LogisimBit::LOGISIM_FALSE));
}
