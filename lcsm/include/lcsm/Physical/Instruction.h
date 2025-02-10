#ifndef LCSM_PHYSICAL_INSTRUCTION_H
#define LCSM_PHYSICAL_INSTRUCTION_H

#include <lcsm/LCSM.h>
#include <lcsm/Physical/DataBits.h>

namespace lcsm
{
	class EvaluatorNode;

	class Instruction
	{
	  public:
		Instruction(InstructionType type, EvaluatorNode *caller, EvaluatorNode *target) noexcept;

		Instruction(const Instruction &other) noexcept;
		Instruction(Instruction &&other) noexcept;

		Instruction &operator=(const Instruction &other) noexcept;
		Instruction &operator=(Instruction &&other) noexcept;

		void swap(Instruction &other) noexcept;

		InstructionType type() const noexcept;

		const EvaluatorNode *caller() const noexcept;
		EvaluatorNode *caller() noexcept;

		const EvaluatorNode *target() const noexcept;
		EvaluatorNode *target() noexcept;

		const DataBits &value() const noexcept;
		void setValue(const DataBits &value) noexcept;

	  private:
		InstructionType m_type;
		EvaluatorNode *m_caller;
		EvaluatorNode *m_target;
		DataBits m_value;
	};

	Instruction CreateWriteValueInstruction(EvaluatorNode *caller, EvaluatorNode *target) noexcept;
	Instruction CreateRequestInstruction(EvaluatorNode *caller, EvaluatorNode *target) noexcept;
	Instruction CreateUpdateStateInstruction(EvaluatorNode *caller, EvaluatorNode *target) noexcept;
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_INSTRUCTION_H */
