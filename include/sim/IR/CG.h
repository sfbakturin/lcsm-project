#ifndef SIM_IR_CG_H
#define SIM_IR_CG_H

#include <sim/IR/CGObject.h>
#include <sim/IR/Instruction.h>
#include <sim/Support/PointerView.hpp>

#include <memory>
#include <vector>

namespace sim
{
	class CGNode
	{
	  public:
		CGNode() = default;
		CGNode(const support::PointerView< CGObject > &target);
		CGNode(support::PointerView< CGObject > &&target);
		CGNode(const std::shared_ptr< Instruction > &run);
		CGNode(std::shared_ptr< Instruction > &&run);
		CGNode(const support::PointerView< CGObject > &target, const std::shared_ptr< Instruction > &run);
		CGNode(const support::PointerView< CGObject > &target, std::shared_ptr< Instruction > &&run);
		CGNode(support::PointerView< CGObject > &&target, const std::shared_ptr< Instruction > &run);
		CGNode(support::PointerView< CGObject > &&target, std::shared_ptr< Instruction > &&run);

		CGNode(const CGNode &other) = delete;
		CGNode(CGNode &&other) noexcept;

		CGNode &operator=(const CGNode &other) = delete;
		CGNode &operator=(CGNode &&other) noexcept;

		void Swap(CGNode &other) noexcept;

		const Instruction *GetInstruction() const noexcept;
		Instruction *GetInstruction() noexcept;
		const CGObject *GetTarget() const noexcept;
		CGObject *GetTarget() noexcept;
		const std::vector< CGNode > &GetConnections() const noexcept;
		std::vector< CGNode > &GetConnections() noexcept;

		void SetInstruction(const std::shared_ptr< Instruction > &run) noexcept;
		void SetInstruction(std::shared_ptr< Instruction > &&run) noexcept;
		void SetTarget(const support::PointerView< CGObject > &target) noexcept;
		void SetTarget(support::PointerView< CGObject > &&target) noexcept;
		CGNode *AddConnection(CGNode &&connection);

	  private:
		support::PointerView< CGObject > m_target;
		std::shared_ptr< Instruction > m_run;
		std::vector< CGNode > m_adjacent;
	};

	class CG
	{
	  public:
		CG() = default;
		CG(const support::PointerView< CGObject > &target);
		CG(support::PointerView< CGObject > &&target);

		CG(const CG &other) = delete;
		CG(CG &&other) noexcept;

		CG &operator=(const CG &other) = delete;
		CG &operator=(CG &&other) noexcept;

		void Swap(CG &other) noexcept;

		void SetTarget(const support::PointerView< CGObject > &target) noexcept;
		void SetTarget(support::PointerView< CGObject > &&target) noexcept;

		const CGNode *GetRoot() const noexcept;
		CGNode *GetRoot() noexcept;

	  private:
		CGNode m_root;
	};
}	 // namespace sim

#endif /* SIM_IR_CG_H */
