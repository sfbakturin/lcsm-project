#include <sim/IR/CG.h>
#include <sim/IR/CGObject.h>
#include <sim/Support/PointerView.hpp>

#include <deque>
#include <utility>

bool sim::CGObject::isWire() const noexcept
{
	return T() == sim::CGObjectT::OBJ_WIRE;
}

bool sim::CGObject::isPinInput() const noexcept
{
	return T() == sim::CGObjectT::OBJ_PIN_INPUT;
}

bool sim::CGObject::isPinOutput() const noexcept
{
	return T() == sim::CGObjectT::OBJ_PIN_OUTPUT;
}

bool sim::CGObject::isConstant() const noexcept
{
	return T() == sim::CGObjectT::OBJ_CONSTANT;
}

bool sim::CGObject::isPower() const noexcept
{
	return T() == sim::CGObjectT::OBJ_POWER;
}

bool sim::CGObject::isGround() const noexcept
{
	return T() == sim::CGObjectT::OBJ_GROUND;
}

bool sim::CGObject::isTransistorBase() const noexcept
{
	return T() == sim::CGObjectT::OBJ_TRANSISTOR_BASE;
}

bool sim::CGObject::isTransistorInout() const noexcept
{
	return T() == sim::CGObjectT::OBJ_TRANSISTOR_INOUT;
}

bool sim::CGObject::isTransistorState() const noexcept
{
	return T() == sim::CGObjectT::OBJ_TRANSISTOR_STATE;
}

sim::CGWire *sim::CGObject::asWire() noexcept
{
	return nullptr;
}

const sim::CGWire *sim::CGObject::asWire() const noexcept
{
	return nullptr;
}

sim::CGPinInput *sim::CGObject::asPinInput() noexcept
{
	return nullptr;
}

const sim::CGPinInput *sim::CGObject::asPinInput() const noexcept
{
	return nullptr;
}

sim::CGPinOutput *sim::CGObject::asPinOutput() noexcept
{
	return nullptr;
}

const sim::CGPinOutput *sim::CGObject::asPinOutput() const noexcept
{
	return nullptr;
}

sim::CGConstant *sim::CGObject::asConstant() noexcept
{
	return nullptr;
}

const sim::CGConstant *sim::CGObject::asConstant() const noexcept
{
	return nullptr;
}

sim::CGPower *sim::CGObject::asPower() noexcept
{
	return nullptr;
}

const sim::CGPower *sim::CGObject::asPower() const noexcept
{
	return nullptr;
}

sim::CGGround *sim::CGObject::asGround() noexcept
{
	return nullptr;
}

const sim::CGGround *sim::CGObject::asGround() const noexcept
{
	return nullptr;
}

sim::CGTransistorBase *sim::CGObject::asTransistorBase() noexcept
{
	return nullptr;
}

const sim::CGTransistorBase *sim::CGObject::asTransistorBase() const noexcept
{
	return nullptr;
}

sim::CGTransistorInout *sim::CGObject::asTransistorInout() noexcept
{
	return nullptr;
}

const sim::CGTransistorInout *sim::CGObject::asTransistorInout() const noexcept
{
	return nullptr;
}

sim::CGTransistorState *sim::CGObject::asTransistorState() noexcept
{
	return nullptr;
}

const sim::CGTransistorState *sim::CGObject::asTransistorState() const noexcept
{
	return nullptr;
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
