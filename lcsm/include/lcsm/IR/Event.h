#ifndef LCSM_IR_EVENT_H
#define LCSM_IR_EVENT_H

#include <lcsm/IR/CG.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/Support/PointerView.hpp>

namespace lcsm
{
	class Event
	{
	  public:
		Event(const Instruction &instruction,
			  const support::PointerView< CGNode > &targetFrom,
			  const support::PointerView< CGNode > &targetTo,
			  unsigned diff) noexcept;
		Event(Instruction &&instruction,
			  const support::PointerView< CGNode > &targetFrom,
			  const support::PointerView< CGNode > &targetTo,
			  unsigned diff) noexcept;

		Event(const Event &other) noexcept;
		Event(Event &&other) noexcept;

		Event &operator=(const Event &other) noexcept;
		Event &operator=(Event &&other) noexcept;

		void swap(Event &other) noexcept;

		const Instruction &instruction() const noexcept;
		Instruction &instruction() noexcept;

		const support::PointerView< CGNode > &targetTo() const noexcept;
		support::PointerView< CGNode > &targetTo() noexcept;

		const support::PointerView< CGNode > &targetFrom() const noexcept;
		support::PointerView< CGNode > &targetFrom() noexcept;

		bool isInstant() const noexcept;
		bool isFuture() const noexcept;

		Event toInstant() const noexcept;

		unsigned diff() const noexcept;
		void setDiff(unsigned newDiff) noexcept;

	  private:
		Instruction m_instruction;
		support::PointerView< CGNode > m_targetFrom;
		support::PointerView< CGNode > m_targetTo;
		unsigned m_diff;
	};

	Event CreateInstantEvent(const Instruction &instruction, const support::PointerView< CGNode > &targetFrom, const support::PointerView< CGNode > &targetTo) noexcept;
	Event CreateInstantEvent(Instruction &&instruction, const support::PointerView< CGNode > &targetFrom, const support::PointerView< CGNode > &targetTo) noexcept;

	Event CreateFutureEvent(
		const Instruction &instruction,
		const support::PointerView< CGNode > &targetFrom,
		const support::PointerView< CGNode > &targetTo,
		unsigned diff) noexcept;
	Event CreateFutureEvent(
		Instruction &&instruction,
		const support::PointerView< CGNode > &targetFrom,
		const support::PointerView< CGNode > &targetTo,
		unsigned diff) noexcept;
}	 // namespace lcsm

#endif /* LCSM_IR_EVENT_H */
