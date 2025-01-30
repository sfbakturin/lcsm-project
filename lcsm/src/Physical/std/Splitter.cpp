// #include <lcsm/IR/CGObject.h>
// #include <lcsm/IR/DataBits.h>
// #include <lcsm/IR/Timestamp.h>
// #include <lcsm/Model/Width.h>

// lcsm::CGSplitter::CGSplitter() : lcsm::Object(lcsm::CGNodeType::Static) {}

// lcsm::CGObjectType lcsm::CGSplitter::objectType() const noexcept
// {
// 	return lcsm::CGObjectType::Splitter;
// }

// lcsm::CGSplitter *lcsm::CGSplitter::asSplitter() noexcept
// {
// 	return this;
// }

// const lcsm::CGSplitter *lcsm::CGSplitter::asSplitter() const noexcept
// {
// 	return this;
// }

// void lcsm::CGSplitter::pushBackInstruction(const lcsm::Instruction &instruction)
// {
// 	// TODO: Verify instruction for this object.
// 	lcsm::Object::pushBackInstruction(instruction);
// }

// void lcsm::CGSplitter::pushBackInstruction(lcsm::Instruction &&instruction)
// {
// 	// TODO: Verify instruction for this object.
// 	lcsm::Object::pushBackInstruction(std::move(instruction));
// }

// std::vector< lcsm::Event > lcsm::CGSplitter::invokeInstructions(const lcsm::Timestamp &now)
// {
// 	// TODO: Implement invoking for this object.
// 	return {};
// }
