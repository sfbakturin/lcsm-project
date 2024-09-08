#include <sim/IR/CGObject.h>
#include <sim/IR/Value.h>
#include <sim/IR/Width.h>

#include <stdexcept>

sim::Value &sim::CGState::read()
{
	throw std::logic_error("State can't be read.");
}

const sim::Value &sim::CGState::read() const
{
	throw std::logic_error("State can't be read.");
}

void sim::CGState::write(const sim::Value &)
{
	throw std::logic_error("State can't be written by value");
}

void sim::CGState::write(sim::Value &&)
{
	throw std::logic_error("State can't be written by value");
}

sim::Width sim::CGState::width() const
{
	throw std::logic_error("There is no certain width of State object.");
}

bool sim::CGState::checkWidth(const Value &) const
{
	throw std::logic_error("There is no certain width of State object.");
}
