#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/Support/PointerView.hpp>

#include <deque>
#include <utility>

bool lcsm::CGObject::isWire() const noexcept
{
	return T() == lcsm::CGObjectT::OBJ_WIRE;
}

bool lcsm::CGObject::isPinInput() const noexcept
{
	return T() == lcsm::CGObjectT::OBJ_PIN_INPUT;
}

bool lcsm::CGObject::isPinOutput() const noexcept
{
	return T() == lcsm::CGObjectT::OBJ_PIN_OUTPUT;
}

bool lcsm::CGObject::isConstant() const noexcept
{
	return T() == lcsm::CGObjectT::OBJ_CONSTANT;
}

bool lcsm::CGObject::isPower() const noexcept
{
	return T() == lcsm::CGObjectT::OBJ_POWER;
}

bool lcsm::CGObject::isGround() const noexcept
{
	return T() == lcsm::CGObjectT::OBJ_GROUND;
}

bool lcsm::CGObject::isTransistorBase() const noexcept
{
	return T() == lcsm::CGObjectT::OBJ_TRANSISTOR_BASE;
}

bool lcsm::CGObject::isTransistorInout() const noexcept
{
	return T() == lcsm::CGObjectT::OBJ_TRANSISTOR_INOUT;
}

bool lcsm::CGObject::isTransistorState() const noexcept
{
	return T() == lcsm::CGObjectT::OBJ_TRANSISTOR_STATE;
}

lcsm::CGWire *lcsm::CGObject::asWire() noexcept
{
	return nullptr;
}

const lcsm::CGWire *lcsm::CGObject::asWire() const noexcept
{
	return nullptr;
}

lcsm::CGPinInput *lcsm::CGObject::asPinInput() noexcept
{
	return nullptr;
}

const lcsm::CGPinInput *lcsm::CGObject::asPinInput() const noexcept
{
	return nullptr;
}

lcsm::CGPinOutput *lcsm::CGObject::asPinOutput() noexcept
{
	return nullptr;
}

const lcsm::CGPinOutput *lcsm::CGObject::asPinOutput() const noexcept
{
	return nullptr;
}

lcsm::CGConstant *lcsm::CGObject::asConstant() noexcept
{
	return nullptr;
}

const lcsm::CGConstant *lcsm::CGObject::asConstant() const noexcept
{
	return nullptr;
}

lcsm::CGPower *lcsm::CGObject::asPower() noexcept
{
	return nullptr;
}

const lcsm::CGPower *lcsm::CGObject::asPower() const noexcept
{
	return nullptr;
}

lcsm::CGGround *lcsm::CGObject::asGround() noexcept
{
	return nullptr;
}

const lcsm::CGGround *lcsm::CGObject::asGround() const noexcept
{
	return nullptr;
}

lcsm::CGTransistorBase *lcsm::CGObject::asTransistorBase() noexcept
{
	return nullptr;
}

const lcsm::CGTransistorBase *lcsm::CGObject::asTransistorBase() const noexcept
{
	return nullptr;
}

lcsm::CGTransistorInout *lcsm::CGObject::asTransistorInout() noexcept
{
	return nullptr;
}

const lcsm::CGTransistorInout *lcsm::CGObject::asTransistorInout() const noexcept
{
	return nullptr;
}

lcsm::CGTransistorState *lcsm::CGObject::asTransistorState() noexcept
{
	return nullptr;
}

const lcsm::CGTransistorState *lcsm::CGObject::asTransistorState() const noexcept
{
	return nullptr;
}

void lcsm::CGObject::addInstantInstr(const lcsm::support::PointerView< Instruction > &instruction)
{
	m_instants.push_back(instruction);
}

void lcsm::CGObject::addInstantInstr(lcsm::support::PointerView< Instruction > &&instruction)
{
	m_instants.push_back(std::move(instruction));
}

void lcsm::CGObject::invokeInstant() {}
