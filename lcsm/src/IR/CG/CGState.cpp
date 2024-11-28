#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

#include <stdexcept>

lcsm::CGState::CGState() : lcsm::CGObject(lcsm::CGNodeType::Dynamic) {}

void lcsm::CGState::reset() noexcept {}

lcsm::DataBits &lcsm::CGState::read()
{
	throw std::logic_error("State can't be read.");
}

const lcsm::DataBits &lcsm::CGState::read() const
{
	throw std::logic_error("State can't be read.");
}

void lcsm::CGState::write(const lcsm::DataBits &)
{
	throw std::logic_error("State can't be written by value");
}

void lcsm::CGState::write(lcsm::DataBits &&)
{
	throw std::logic_error("State can't be written by value");
}

lcsm::model::Width lcsm::CGState::width() const
{
	throw std::logic_error("There is no certain width of State object.");
}

bool lcsm::CGState::checkWidth(const DataBits &) const
{
	throw std::logic_error("There is no certain width of State object.");
}

void lcsm::CGState::pushBackInstruction(const lcsm::Instruction &instruction)
{
	// TODO: Verify instruction for this object.
	lcsm::CGObject::pushBackInstruction(instruction);
}

void lcsm::CGState::pushBackInstruction(lcsm::Instruction &&instruction)
{
	// TODO: Verify instruction for this object.
	lcsm::CGObject::pushBackInstruction(std::move(instruction));
}
