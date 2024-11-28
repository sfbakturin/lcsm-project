#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

#include <stdexcept>

lcsm::CGSplitter::CGSplitter() : lcsm::CGObject(lcsm::CGNodeType::Static) {}

void lcsm::CGSplitter::reset() noexcept {}

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

void lcsm::CGSplitter::pushBackInstruction(const lcsm::Instruction &instruction)
{
	// TODO: Verify instruction for this object.
	lcsm::CGObject::pushBackInstruction(instruction);
}

void lcsm::CGSplitter::pushBackInstruction(lcsm::Instruction &&instruction)
{
	// TODO: Verify instruction for this object.
	lcsm::CGObject::pushBackInstruction(std::move(instruction));
}

std::vector< lcsm::Event > lcsm::CGSplitter::invokeInstructions()
{
	// TODO: Implement invoking for this object.
	return {};
}
