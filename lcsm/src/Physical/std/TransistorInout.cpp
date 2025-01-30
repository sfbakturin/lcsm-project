// #include <lcsm/IR/CG.h>
// #include <lcsm/IR/CGObject.h>
// #include <lcsm/IR/Timestamp.h>
// #include <lcsm/Support/PointerView.hpp>

// #include <utility>

// lcsm::CGCollector::CGCollector() : lcsm::Object(lcsm::CGNodeType::Composite) {}

// lcsm::CGCollector::CGCollector(const lcsm::support::PointerView< lcsm::CGState > &state) :
// 	lcsm::Object(lcsm::CGNodeType::Composite), m_state(state)
// {
// }

// lcsm::CGCollector::CGCollector(lcsm::support::PointerView< lcsm::CGState > &&state) :
// 	lcsm::Object(lcsm::CGNodeType::Composite), m_state(std::move(state))
// {
// }

// lcsm::support::PointerView< lcsm::CGState > &lcsm::CGCollector::state() noexcept
// {
// 	return m_state;
// }

// const lcsm::support::PointerView< lcsm::CGState > &lcsm::CGCollector::state() const noexcept
// {
// 	return m_state;
// }

// void lcsm::CGCollector::setState(const support::PointerView< CGState > &newState) noexcept
// {
// 	m_state = newState;
// }

// void lcsm::CGCollector::setState(support::PointerView< CGState > &&newState) noexcept
// {
// 	m_state = std::move(newState);
// }

// void lcsm::CGCollector::pushBackInstruction(const lcsm::Instruction &instruction)
// {
// 	// TODO: Verify instruction for this object.
// 	lcsm::Object::pushBackInstruction(instruction);
// }

// void lcsm::CGCollector::pushBackInstruction(lcsm::Instruction &&instruction)
// {
// 	// TODO: Verify instruction for this object.
// 	lcsm::Object::pushBackInstruction(std::move(instruction));
// }

// std::vector< lcsm::Event > lcsm::CGCollector::invokeInstructions(const lcsm::Timestamp &now)
// {
// 	// TODO: Implement invoking for this object.
// 	return {};
// }
