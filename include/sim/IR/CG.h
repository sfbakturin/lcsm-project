#ifndef SIM_IR_CG_H
#define SIM_IR_CG_H

#include <sim/IR/CGObject.h>
#include <sim/IR/Instruction.h>
#include <sim/Support/PointerView.hpp>

#include <memory>
#include <utility>
#include <vector>

namespace sim
{
	class CGEdge
	{
	  public:
		CGEdge();
		CGEdge(const CGObjectView &targetFrom,
			   const CGObjectView &targetTo,
			   const std::shared_ptr< Instruction > &run);
		CGEdge(const CGObjectView &targetFrom,
			   const CGObjectView &targetTo,
			   std::shared_ptr< Instruction > &&run);

		CGEdge(const CGEdge &other) = delete;
		CGEdge(CGEdge &&other) noexcept;

		CGEdge &operator=(const CGEdge &other) = delete;
		CGEdge &operator=(CGEdge &&other) noexcept;

		void swap(CGEdge &other) noexcept;

		Instruction *instruction() noexcept;
		const Instruction *instruction() const noexcept;
		CGObject *targetFrom() noexcept;
		const CGObject *targetFrom() const noexcept;
		CGObject *targetTo() noexcept;
		const CGObject *targetTo() const noexcept;

		void setInstruction(const InstructionShared &run) noexcept;
		void setInstruction(InstructionShared &&run) noexcept;
		void setTargetFrom(const CGObjectView &targetFrom) noexcept;
		void setTargetFrom(CGObjectView &&targetFrom) noexcept;
		void setTargetTo(const CGObjectView &targetTo) noexcept;
		void setTargetTo(CGObjectView &&targetTo) noexcept;

	  private:
		CGObjectView m_targetFrom;
		CGObjectView m_targetTo;
		InstructionShared m_run;
		unsigned m_timer;
	};

	using CGEdgeView = support::PointerView< CGEdge >;

	class CGNode
	{
	  public:
		using view_type = support::PointerView< CGNode >;

		CGNode() = default;

		CGNode(const CGObjectView &object);
		CGNode(CGObjectView &&object);
		CGNode(CGObject *object);

		CGNode(const CGNode &other) = delete;
		CGNode(CGNode &&other) noexcept;

		CGNode &operator=(const CGNode &other) = delete;
		CGNode &operator=(CGNode &&other) noexcept;

		void swap(CGNode &other) noexcept;

		CGObjectView &object() noexcept;
		const CGObjectView &object() const noexcept;
		void setObject(const CGObjectView &object) noexcept;
		void setObject(CGObjectView &&object) noexcept;
		void setObject(CGObject *object) noexcept;

		std::vector< std::pair< CGEdge, view_type > > &instructions() noexcept;
		const std::vector< std::pair< CGEdge, view_type > > &instructions() const noexcept;
		void addInstruction(CGEdge &&I, CGNode *N);
		void addInstruction(CGEdge &&I, const view_type &N);
		void addInstruction(CGEdge &&I, view_type &&N);

	  private:
		CGObjectView m_target;
		std::vector< std::pair< CGEdge, view_type > > m_instructions;
	};

	using CGNodeView = CGNode::view_type;

	class CG
	{
	  public:
		CG() = default;

		CG(const CG &other) = delete;
		CG(CG &&other) noexcept;

		CG &operator=(const CG &other) = delete;
		CG &operator=(CG &&other) noexcept;

		void swap(CG &other) noexcept;

		void addRoot(const CGNodeView &node);
		void addRoot(CGNodeView &&node);

	  private:
		std::vector< CGNodeView > m_roots;
	};
}	 // namespace sim

#endif /* SIM_IR_CG_H */
