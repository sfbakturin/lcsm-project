#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Event.h>
#include <lcsm/Support/PointerView.hpp>

#include <deque>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::CGObject::CGObject(CGNodeType nodeType) : lcsm::CGNode(nodeType) {}

bool lcsm::CGObject::isWire() const noexcept
{
	return objectType() == lcsm::CGObjectType::Wire;
}

bool lcsm::CGObject::isPinInput() const noexcept
{
	return objectType() == lcsm::CGObjectType::PinInput;
}

bool lcsm::CGObject::isPinOutput() const noexcept
{
	return objectType() == lcsm::CGObjectType::PinOutput;
}

bool lcsm::CGObject::isConstant() const noexcept
{
	return objectType() == lcsm::CGObjectType::Constant;
}

bool lcsm::CGObject::isPower() const noexcept
{
	return objectType() == lcsm::CGObjectType::Power;
}

bool lcsm::CGObject::isGround() const noexcept
{
	return objectType() == lcsm::CGObjectType::Ground;
}

bool lcsm::CGObject::isSplitter() const noexcept
{
	return objectType() == lcsm::CGObjectType::Splitter;
}

bool lcsm::CGObject::isTransistorBase() const noexcept
{
	return objectType() == lcsm::CGObjectType::TransistorBase;
}

bool lcsm::CGObject::isTransistorInout() const noexcept
{
	return objectType() == lcsm::CGObjectType::TransistorInout;
}

bool lcsm::CGObject::isTransistorState() const noexcept
{
	return objectType() == lcsm::CGObjectType::TransistorState;
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

lcsm::CGSplitter *lcsm::CGObject::asSplitter() noexcept
{
	return nullptr;
}

const lcsm::CGSplitter *lcsm::CGObject::asSplitter() const noexcept
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

void lcsm::CGObject::pushBackInstruction(const Instruction &instruction)
{
	m_instants.push_back(instruction);
}

void lcsm::CGObject::pushBackInstruction(Instruction &&instruction)
{
	m_instants.push_back(std::move(instruction));
}

std::vector< lcsm::Event > lcsm::CGObject::invokeInstructions()
{
	throw std::logic_error("Can't invoke instants for unknown object.");
}
