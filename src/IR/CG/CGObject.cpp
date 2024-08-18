#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/Support/PointerView.hpp>

#include <deque>
#include <utility>

bool sim::CGObject::isPinInput() const noexcept
{
	return T() == sim::CGObjectT::OBJ_PIN_INPUT;
}

bool sim::CGObject::isPinOutput() const noexcept
{
	return T() == sim::CGObjectT::OBJ_PIN_OUTPUT;
}

bool sim::CGObject::isWire() const noexcept
{
	return T() == sim::CGObjectT::OBJ_WIRE;
}

void sim::CGObject::addInstantInstr(const sim::support::PointerView< Instruction > &instruction)
{
	m_instants.push_back(instruction);
}

void sim::CGObject::addInstantInstr(sim::support::PointerView< Instruction > &&instruction)
{
	m_instants.push_back(std::move(instruction));
}

void sim::CGObject::invokeInstant() {}
