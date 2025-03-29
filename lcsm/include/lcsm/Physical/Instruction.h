#ifndef LCSM_PHYSICAL_INSTRUCTION_H
#define LCSM_PHYSICAL_INSTRUCTION_H

#include <lcsm/LCSM.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/lcsmconfig.h>

namespace lcsm
{
	class LCSM_API EvaluatorNode;

	class LCSM_API Instruction
	{
	  public:
		Instruction(EvaluatorNode *caller, instruction_t type) noexcept;
		Instruction(EvaluatorNode *caller, EvaluatorNode *target, instruction_t type) noexcept;
		Instruction(EvaluatorNode *caller, EvaluatorNode *target, instruction_t type, const DataBits &databits) noexcept;
		Instruction(EvaluatorNode *caller, EvaluatorNode *target, instruction_t type, DataBits &&databits) noexcept;

		Instruction(const Instruction &other) noexcept;
		Instruction(Instruction &&other) noexcept;

		Instruction &operator=(const Instruction &other) noexcept;
		Instruction &operator=(Instruction &&other) noexcept;

		void swap(Instruction &other) noexcept;

		instruction_t type() const noexcept;
		bool operator==(instruction_t type) const noexcept;
		bool isInstruction() const noexcept;
		bool isSimulatorInstruction() const noexcept;

		const EvaluatorNode *caller() const noexcept;
		EvaluatorNode *caller() noexcept;

		const EvaluatorNode *target() const noexcept;
		EvaluatorNode *target() noexcept;

		bool valued() const noexcept;
		const DataBits &value() const;
		DataBits &value();

	  private:
		EvaluatorNode *m_caller;
		EvaluatorNode *m_target;
		instruction_t m_type;
		DataBits m_databits;
		bool m_valued;
	};

	LCSM_API Instruction CreateWriteValueInstruction(EvaluatorNode *caller, EvaluatorNode *target, const DataBits &value) noexcept;
	LCSM_API Instruction CreateWriteValueInstruction(EvaluatorNode *caller, EvaluatorNode *target, DataBits &&value) noexcept;

	LCSM_API Instruction CreatePolluteValueInstruction(EvaluatorNode *caller, EvaluatorNode *target) noexcept;

	LCSM_API Instruction CreatePolluteCircuitSimulatorInstruction(EvaluatorNode *caller) noexcept;
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_INSTRUCTION_H */
