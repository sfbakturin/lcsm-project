#ifndef LCSM_PHYSICAL_EVALUATOR_H
#define LCSM_PHYSICAL_EVALUATOR_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <vector>

namespace lcsm
{
	class EvaluatorNode
	{
	  public:
		EvaluatorNode() noexcept;
		virtual ~EvaluatorNode() noexcept = default;

		ObjectType objectType() const noexcept;
		void setObjectType(ObjectType objectType) noexcept;

		virtual NodeType nodeType() const noexcept = 0;

		virtual std::size_t contextSize() const noexcept = 0;

		virtual const DataBits &read() const = 0;

		virtual Width width() const = 0;
		virtual bool checkWidth(const DataBits &value) const = 0;

		virtual void setContext(const support::PointerView< Context > &context) = 0;
		virtual void resetContext() noexcept = 0;

		virtual void addInstant(const Instruction &instruction) = 0;
		virtual void addInstant(Instruction &&instruction) = 0;

		virtual std::vector< Event > invokeInstants(const Timestamp &now) = 0;

	  private:
		ObjectType m_objectType;
	};

	class Evaluator
	{
	  public:
		Evaluator() = default;

		Evaluator(const Evaluator &other) = delete;
		Evaluator(Evaluator &&other) noexcept;

		Evaluator &operator=(const Evaluator &other) = delete;
		Evaluator &operator=(Evaluator &&other) noexcept;

		void swap(Evaluator &other) noexcept;

		void addRoot(const support::PointerView< EvaluatorNode > &node);
		void addRoot(support::PointerView< EvaluatorNode > &&node);

		const std::vector< support::PointerView< EvaluatorNode > > &roots() const noexcept;
		std::vector< support::PointerView< EvaluatorNode > > &roots() noexcept;

	  private:
		std::vector< support::PointerView< EvaluatorNode > > m_roots;
	};
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_EVALUATOR_H */
