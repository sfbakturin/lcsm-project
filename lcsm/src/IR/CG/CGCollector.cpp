#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/Support/PointerView.hpp>

#include <utility>

lcsm::CGCollector::CGCollector() : lcsm::CGObject(lcsm::CGNodeType::Composite) {}

lcsm::CGCollector::CGCollector(const lcsm::support::PointerView< lcsm::CGState > &state) :
	lcsm::CGObject(lcsm::CGNodeType::Composite), m_state(state)
{
}

lcsm::CGCollector::CGCollector(lcsm::support::PointerView< lcsm::CGState > &&state) :
	lcsm::CGObject(lcsm::CGNodeType::Composite), m_state(std::move(state))
{
}

void lcsm::CGCollector::reset() noexcept {}

lcsm::support::PointerView< lcsm::CGState > &lcsm::CGCollector::state() noexcept
{
	return m_state;
}

const lcsm::support::PointerView< lcsm::CGState > &lcsm::CGCollector::state() const noexcept
{
	return m_state;
}

void lcsm::CGCollector::setState(const support::PointerView< CGState > &newState) noexcept
{
	m_state = newState;
}

void lcsm::CGCollector::setState(support::PointerView< CGState > &&newState) noexcept
{
	m_state = std::move(newState);
}

void lcsm::CGCollector::pushBackInstruction(const lcsm::Instruction &instruction)
{
	// TODO: Verify instruction for this object.
	lcsm::CGObject::pushBackInstruction(instruction);
}

void lcsm::CGCollector::pushBackInstruction(lcsm::Instruction &&instruction)
{
	// TODO: Verify instruction for this object.
	lcsm::CGObject::pushBackInstruction(std::move(instruction));
}

std::vector< lcsm::Event > lcsm::CGCollector::invokeInstructions()
{
	// TODO: Implement invoking for this object.
	return {};
}
