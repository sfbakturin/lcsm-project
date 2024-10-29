#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Value.h>
#include <lcsm/IR/Width.h>

#include <stdexcept>

lcsm::Value &lcsm::CGState::read()
{
	throw std::logic_error("State can't be read.");
}

const lcsm::Value &lcsm::CGState::read() const
{
	throw std::logic_error("State can't be read.");
}

void lcsm::CGState::write(const lcsm::Value &)
{
	throw std::logic_error("State can't be written by value");
}

void lcsm::CGState::write(lcsm::Value &&)
{
	throw std::logic_error("State can't be written by value");
}

lcsm::Width lcsm::CGState::width() const
{
	throw std::logic_error("There is no certain width of State object.");
}

bool lcsm::CGState::checkWidth(const Value &) const
{
	throw std::logic_error("There is no certain width of State object.");
}
