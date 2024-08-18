#ifndef SIM_IR_INSTRUCTION_H
#define SIM_IR_INSTRUCTION_H

#include <sim/IR/CGObject.h>

namespace sim
{
	enum InstructionT : unsigned
	{
		INSTR_WRITE_VALUE,
		INSTR_BROADCAST_VALUE,
		INSTR_UPDATE_ELEMENT_STATE,
		INSTR_WRITE_VALUE_STABILIZED,
	};

	class Instruction
	{
	  public:
		virtual ~Instruction() noexcept = default;

		virtual void invoke() = 0;

		virtual InstructionT T() const noexcept = 0;

		bool isWrite() const noexcept;
		bool isBroadcast() const noexcept;
		bool isUpdateElementState() const noexcept;
		bool isWriteValueStabilized() const noexcept;

		virtual bool mayStabilized() const noexcept = 0;
	};

	class WriteValue : public Instruction
	{
	  public:
		WriteValue(CGObject *targetFrom, CGObject *targetTo) noexcept;

		virtual void invoke() override;

		virtual InstructionT T() const noexcept override;

		virtual bool mayStabilized() const noexcept override;

	  private:
		CGObject *m_targetFrom;
		CGObject *m_targetTo;
	};

	class BroadcastValue : public Instruction
	{
	  public:
		BroadcastValue(CGObject *targetFrom, CGObject *targetTo) noexcept;

		virtual void invoke() override;

		virtual InstructionT T() const noexcept override;

		virtual bool mayStabilized() const noexcept override;

	  private:
		CGObject *m_targetFrom;
		CGObject *m_targetTo;
	};

	class UpdateElementState : public Instruction
	{
	  public:
		UpdateElementState(CGObject *target) noexcept;

		virtual void invoke() override;

		virtual InstructionT T() const noexcept override;

		virtual bool mayStabilized() const noexcept override;

	  private:
		CGObject *m_target;
	};

	class WriteValueStabilized : public Instruction
	{
	  public:
		WriteValueStabilized(CGObject *targetFrom, CGObject *targetTo) noexcept;

		virtual void invoke() override;

		virtual InstructionT T() const noexcept override;

		virtual bool mayStabilized() const noexcept override;

	  private:
		CGObject *m_targetFrom;
		CGObject *m_targetTo;
	};
}	 // namespace sim

#endif /* SIM_IR_INSTRUCTION_H */
