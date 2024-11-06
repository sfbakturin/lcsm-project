#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

lcsm::CGObjectType lcsm::CGGround::objectType() const noexcept
{
	return lcsm::CGObjectType::Ground;
}

lcsm::CGGround *lcsm::CGGround::asGround() noexcept
{
	return this;
}

const lcsm::CGGround *lcsm::CGGround::asGround() const noexcept
{
	return this;
}

void lcsm::CGGround::setWidth(lcsm::model::Width width)
{
	setDataBits(lcsm::DataBits(width));
}
