#ifndef LCSM_PHYSICAL_EVENT_H
#define LCSM_PHYSICAL_EVENT_H

#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/lcsmconfig.h>

namespace lcsm
{
	class LCSM_API Event
	{
	  public:
		Event(const Instruction &instruction) noexcept;
		Event(Instruction &&instruction) noexcept;
		Event(const Instruction &instruction, Timestamp diff) noexcept;
		Event(Instruction &&instruction, Timestamp diff) noexcept;

		Event(const Event &other) noexcept;
		Event(Event &&other) noexcept;

		Event &operator=(const Event &other) noexcept;
		Event &operator=(Event &&other) noexcept;

		void swap(Event &other) noexcept;

		const Instruction &instruction() const noexcept;
		Instruction &instruction() noexcept;

		bool isInstant() const noexcept;
		bool isFuture() const noexcept;

		void toInstant() noexcept;

		Timestamp diff() const noexcept;

	  private:
		Instruction m_instruction;
		Timestamp m_diff;
	};
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_EVENT_H */
