#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

#include <stdexcept>

lcsm::DataBits &lcsm::CGSplitter::read()
{
	throw std::logic_error("Not implemented");
}

const lcsm::DataBits &lcsm::CGSplitter::read() const
{
	throw std::logic_error("Not implemented");
}

void lcsm::CGSplitter::write(const lcsm::DataBits &)
{
	throw std::logic_error("Not implemented");
}

void lcsm::CGSplitter::write(lcsm::DataBits &&)
{
	throw std::logic_error("Not implemented");
}

lcsm::model::Width lcsm::CGSplitter::width() const
{
	throw std::logic_error("Not implemented");
}

bool lcsm::CGSplitter::checkWidth(const lcsm::DataBits &) const
{
	throw std::logic_error("Not implemented");
}

lcsm::CGObjectType lcsm::CGSplitter::objectType() const noexcept
{
	return lcsm::CGObjectType::Splitter;
}

lcsm::CGSplitter *lcsm::CGSplitter::asSplitter() noexcept
{
	return this;
}

const lcsm::CGSplitter *lcsm::CGSplitter::asSplitter() const noexcept
{
	return this;
}
