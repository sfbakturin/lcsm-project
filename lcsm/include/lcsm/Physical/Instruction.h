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
		Instruction(InstructionType type, EvaluatorNode *caller, EvaluatorNode *target, const DataBits &value) noexcept;
		Instruction(InstructionType type, EvaluatorNode *caller, EvaluatorNode *target, DataBits &&value) noexcept;

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

		bool hasValue() const noexcept;
		const DataBits &value() const;

	  private:
		InstructionType m_type;
		EvaluatorNode *m_caller;
		EvaluatorNode *m_target;

		bool m_hasValue;
		DataBits m_value;
	};

	Instruction CreateWriteValueInstruction(EvaluatorNode *caller, EvaluatorNode *target) noexcept;
	Instruction CreateWriteValueInstruction(EvaluatorNode *caller, EvaluatorNode *target, const DataBits &value) noexcept;
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_INSTRUCTION_H */
