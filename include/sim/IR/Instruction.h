#ifndef SIM_IR_INSTRUCTION_H
#define SIM_IR_INSTRUCTION_H

#include <sim/Support/PointerView.hpp>

#include <memory>

namespace sim
{
	class CGObject;

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

	using InstructionView = support::PointerView< Instruction >;
	using InstructionShared = std::shared_ptr< Instruction >;

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

	class UpdateState : public Instruction
	{
	  public:
		UpdateState(CGObject *target) noexcept;

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

	std::shared_ptr< Instruction > CreateWriteValue(CGObject *targetFrom, CGObject *targetTo);
	std::shared_ptr< Instruction >
		CreateBroadcastValue(CGObject *targetFrom, CGObject *targetTo);
	std::shared_ptr< Instruction > CreateUpdateState(CGObject *target);
	std::shared_ptr< Instruction >
		CreateWriteValueStabilized(CGObject *targetFrom, CGObject *targetTo);
}	 // namespace sim

#endif /* SIM_IR_INSTRUCTION_H */
