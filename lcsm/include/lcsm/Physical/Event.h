#ifndef LCSM_PHYSICAL_EVENT_H
#define LCSM_PHYSICAL_EVENT_H

#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/PointerView.hpp>

namespace lcsm
{
	class EvaluatorNode;

	class Event
	{
	  public:
		Event(const Instruction &instruction,
			  const support::PointerView< EvaluatorNode > &targetFrom,
			  const support::PointerView< EvaluatorNode > &targetTo,
			  const Timestamp &diff) noexcept;
		Event(Instruction &&instruction,
			  const support::PointerView< EvaluatorNode > &targetFrom,
			  const support::PointerView< EvaluatorNode > &targetTo,
			  const Timestamp &diff) noexcept;

		Event(const Event &other) noexcept;
		Event(Event &&other) noexcept;

		Event &operator=(const Event &other) noexcept;
		Event &operator=(Event &&other) noexcept;

		void swap(Event &other) noexcept;

		const Instruction &instruction() const noexcept;
		Instruction &instruction() noexcept;

		const support::PointerView< EvaluatorNode > &targetTo() const noexcept;
		support::PointerView< EvaluatorNode > &targetTo() noexcept;

		const support::PointerView< EvaluatorNode > &targetFrom() const noexcept;
		support::PointerView< EvaluatorNode > &targetFrom() noexcept;

		bool isInstant() const noexcept;
		bool isFuture() const noexcept;

		Event toInstant() const noexcept;

		const Timestamp &diff() const noexcept;
		void setDiff(const Timestamp &newDiff) noexcept;
		void setDiff(Timestamp &&newDiff) noexcept;

	  private:
		Instruction m_instruction;
		support::PointerView< EvaluatorNode > m_targetFrom;
		support::PointerView< EvaluatorNode > m_targetTo;
		Timestamp m_diff;
	};

	Event CreateInstantEvent(const Instruction &instruction,
							 const support::PointerView< EvaluatorNode > &targetFrom,
							 const support::PointerView< EvaluatorNode > &targetTo) noexcept;
	Event CreateInstantEvent(Instruction &&instruction,
							 const support::PointerView< EvaluatorNode > &targetFrom,
							 const support::PointerView< EvaluatorNode > &targetTo) noexcept;

	Event CreateFutureEvent(
		const Instruction &instruction,
		const support::PointerView< EvaluatorNode > &targetFrom,
		const support::PointerView< EvaluatorNode > &targetTo,
		const Timestamp &diff) noexcept;
	Event CreateFutureEvent(
		Instruction &&instruction,
		const support::PointerView< EvaluatorNode > &targetFrom,
		const support::PointerView< EvaluatorNode > &targetTo,
		const Timestamp &diff) noexcept;
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_EVENT_H */
