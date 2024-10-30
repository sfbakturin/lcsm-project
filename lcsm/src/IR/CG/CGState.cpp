#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

#include <stdexcept>

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

lcsm::Width lcsm::CGState::width() const
{
	throw std::logic_error("There is no certain width of State object.");
}

bool lcsm::CGState::checkWidth(const DataBits &) const
{
	throw std::logic_error("There is no certain width of State object.");
}
