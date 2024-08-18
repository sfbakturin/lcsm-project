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
		CGNode();

		CGNode(const CGNode &other) = delete;
		CGNode(CGNode &&other) noexcept;

		CGNode &operator=(const CGNode &other) = delete;
		CGNode &operator=(CGNode &&other) noexcept;

		void swap(CGNode &other) noexcept;

		Instruction *instruction() noexcept;
		const Instruction *instruction() const noexcept;
		CGObject *targetFrom() noexcept;
		const CGObject *targetFrom() const noexcept;
		CGObject *targetTo() noexcept;
		const CGObject *targetTo() const noexcept;
		std::vector< CGNode > &connections() noexcept;
		const std::vector< CGNode > &connections() const noexcept;

		void setInstruction(const std::shared_ptr< Instruction > &run) noexcept;
		void setInstruction(std::shared_ptr< Instruction > &&run) noexcept;
		void setTargetFrom(const CGObjectView &targetFrom) noexcept;
		void setTargetFrom(CGObjectView &&targetFrom) noexcept;
		void setTargetTo(const CGObjectView &targetTo) noexcept;
		void setTargetTo(CGObjectView &&targetTo) noexcept;

		CGNode *addChild(CGNode &&connection);

	  private:
		CGObjectView m_targetFrom;
		CGObjectView m_targetTo;
		std::shared_ptr< Instruction > m_run;
		std::vector< CGNode > m_adjacent;
		unsigned m_timer;
	};

	using CGNodeView = support::PointerView< CGNode >;

	class CG
	{
	  public:
		CG() = default;

		CG(const CG &other) = delete;
		CG(CG &&other) noexcept;

		CG &operator=(const CG &other) = delete;
		CG &operator=(CG &&other) noexcept;

		void swap(CG &other) noexcept;

		CGNode *root() noexcept;
		const CGNode *root() const noexcept;

	  private:
		CGNode m_root;
	};
}	 // namespace sim

#endif /* SIM_IR_CG_H */
