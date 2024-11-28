#ifndef LCSM_IR_INSTRUCTION_H
#define LCSM_IR_INSTRUCTION_H

namespace lcsm
{
	class CGObject;

	enum InstructionType : unsigned
	{
		WriteValue,
		BroadcastValue,
		Request,
		UpdateState
	};

	class Instruction
	{
	  public:
		Instruction(InstructionType type, CGObject *caller, CGObject *target) noexcept;

		Instruction(const Instruction &other) noexcept;
		Instruction(Instruction &&other) noexcept;

		Instruction &operator=(const Instruction &other) noexcept;
		Instruction &operator=(Instruction &&other) noexcept;

		void swap(Instruction &other) noexcept;

		bool isWrite() const noexcept;
		bool isBroadcast() const noexcept;
		bool isRequest() const noexcept;
		bool isUpdateState() const noexcept;

		const CGObject *caller() const noexcept;
		CGObject *caller() noexcept;

		const CGObject *target() const noexcept;
		CGObject *target() noexcept;

	  private:
		InstructionType m_type;
		CGObject *m_caller;
		CGObject *m_target;
	};

	Instruction CreateWriteValueInstruction(CGObject *caller, CGObject *target) noexcept;
	Instruction CreateBroadcastValueInstruction(CGObject *caller, CGObject *target) noexcept;
	Instruction CreateRequestInstruction(CGObject *caller, CGObject *target) noexcept;
	Instruction CreateUpdateStateInstruction(CGObject *caller, CGObject *target) noexcept;
}	 // namespace lcsm

#endif /* LCSM_IR_INSTRUCTION_H */
