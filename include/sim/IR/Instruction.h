#ifndef SIM_IR_INSTRUCTION_H
#define SIM_IR_INSTRUCTION_H

#include <sim/IR/CGObject.h>

namespace sim
{
	enum InstructionT
	{
		INSTR_WRITE_VALUE,
		INSTR_BROADCAST_VALUE,
		INSTR_READ_VALUE
	};

	class Instruction
	{
	  public:
		virtual ~Instruction() noexcept = default;

		virtual void Invoke() = 0;

		virtual InstructionT T() const noexcept = 0;

		bool IsWrite() const noexcept;
		bool IsRead() const noexcept;
		bool IsBroadcast() const noexcept;
	};

	class WriteValue : public Instruction
	{
	  public:
		WriteValue(CGObject *targetTo, CGObject *targetFrom) noexcept;

		virtual void Invoke() override;

		virtual InstructionT T() const noexcept override;

	  private:
		CGObject *m_targetFrom;
		CGObject *m_targetTo;
	};

	class BroadcastValue : public Instruction
	{
	  public:
		BroadcastValue(CGObject *targetTo, CGObject *targetFrom) noexcept;

		virtual void Invoke() override;

		virtual InstructionT T() const noexcept override;

	  private:
		CGObject *m_targetFrom;
		CGObject *m_targetTo;
	};

	class ReadValue : public Instruction
	{
	  public:
		ReadValue(CGObject *targetTo, CGObject *targetFrom) noexcept;

		virtual void Invoke() override;

		virtual InstructionT T() const noexcept override;

	  private:
		CGObject *m_targetFrom;
		CGObject *m_targetTo;
	};
}	 // namespace sim

#endif /* SIM_IR_INSTRUCTION_H */
