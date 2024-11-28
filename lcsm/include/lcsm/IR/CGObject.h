#ifndef LCSM_IR_CGOBJECT_H
#define LCSM_IR_CGOBJECT_H

#include <lcsm/IR/CG.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/IR/Event.h>
#include <lcsm/IR/Instruction.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstdint>
#include <deque>

namespace lcsm
{
	enum CGObjectType : unsigned
	{
		PinInput,
		PinOutput,
		Wire,
		Constant,
		Power,
		Ground,
		Splitter,
		ClockInout,
		ClockState,
		TransistorBase,
		TransistorInout,
		TransistorState
	};

	class CGWire;
	class CGPinInput;
	class CGPinOutput;
	class CGConstant;
	class CGPower;
	class CGGround;
	class CGSplitter;
	class CGClockInout;
	class CGClockState;
	class CGTransistorBase;
	class CGTransistorInout;
	class CGTransistorState;

	class Instruction;

	class CGObject : public CGNode
	{
	  public:
		CGObject(CGNodeType nodeType);
		virtual ~CGObject() noexcept = default;

		virtual void reset() noexcept = 0;

		virtual DataBits &read() = 0;
		virtual const DataBits &read() const = 0;

		virtual void write(const DataBits &value) = 0;
		virtual void write(DataBits &&value) = 0;

		virtual model::Width width() const = 0;
		virtual bool checkWidth(const DataBits &value) const = 0;

		virtual CGObjectType objectType() const noexcept = 0;

		bool isWire() const noexcept;
		bool isPinInput() const noexcept;
		bool isPinOutput() const noexcept;
		bool isConstant() const noexcept;
		bool isPower() const noexcept;
		bool isGround() const noexcept;
		bool isSplitter() const noexcept;
		bool isTransistorBase() const noexcept;
		bool isTransistorInout() const noexcept;
		bool isTransistorState() const noexcept;

		virtual CGWire *asWire() noexcept;
		virtual const CGWire *asWire() const noexcept;
		virtual CGPinInput *asPinInput() noexcept;
		virtual const CGPinInput *asPinInput() const noexcept;
		virtual CGPinOutput *asPinOutput() noexcept;
		virtual const CGPinOutput *asPinOutput() const noexcept;
		virtual CGConstant *asConstant() noexcept;
		virtual const CGConstant *asConstant() const noexcept;
		virtual CGPower *asPower() noexcept;
		virtual const CGPower *asPower() const noexcept;
		virtual CGGround *asGround() noexcept;
		virtual const CGGround *asGround() const noexcept;
		virtual CGSplitter *asSplitter() noexcept;
		virtual const CGSplitter *asSplitter() const noexcept;
		virtual CGTransistorBase *asTransistorBase() noexcept;
		virtual const CGTransistorBase *asTransistorBase() const noexcept;
		virtual CGTransistorInout *asTransistorInout() noexcept;
		virtual const CGTransistorInout *asTransistorInout() const noexcept;
		virtual CGTransistorState *asTransistorState() noexcept;
		virtual const CGTransistorState *asTransistorState() const noexcept;

		virtual void pushBackInstruction(const Instruction &instruction);
		virtual void pushBackInstruction(Instruction &&instruction);
		virtual std::vector< Event > invokeInstructions();

	  protected:
		std::deque< Instruction > m_instants;
	};

	using CGObjectView = support::PointerView< CGObject >;

	class CGPin : public CGObject
	{
	  public:
		CGPin();

		virtual ~CGPin() noexcept = default;

		CGPin(const DataBits &value);
		CGPin(DataBits &&value);

		virtual model::Width width() const override;
		virtual bool checkWidth(const DataBits &value) const override;

	  protected:
		DataBits m_value;
	};

	class CGPinInput : public CGPin
	{
	  public:
		CGPinInput();

		virtual void reset() noexcept override final;

		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual CGObjectType objectType() const noexcept override;

		void externalWrite(const DataBits &value);
		void externalWrite(DataBits &&value) noexcept;

		virtual CGPinInput *asPinInput() noexcept override final;
		virtual const CGPinInput *asPinInput() const noexcept override final;

		virtual void pushBackInstruction(const Instruction &instruction) override final;
		virtual void pushBackInstruction(Instruction &&instruction) override final;
		virtual std::vector< Event > invokeInstructions() override final;
	};

	class CGPinOutput : public CGPin
	{
	  public:
		virtual void reset() noexcept override final;

		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual CGObjectType objectType() const noexcept override;

		DataBits &externalRead() noexcept;
		const DataBits &externalRead() const noexcept;

		virtual CGPinOutput *asPinOutput() noexcept override final;
		virtual const CGPinOutput *asPinOutput() const noexcept override final;

		virtual void pushBackInstruction(const Instruction &instruction) override final;
		virtual void pushBackInstruction(Instruction &&instruction) override final;
		virtual std::vector< Event > invokeInstructions() override final;
	};

	class CGWire : public CGObject
	{
	  public:
		CGWire();

		virtual void reset() noexcept override final;

		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual model::Width width() const override final;
		virtual bool checkWidth(const DataBits &value) const override final;

		virtual CGObjectType objectType() const noexcept override;

		virtual CGWire *asWire() noexcept override final;
		virtual const CGWire *asWire() const noexcept override final;

		virtual void pushBackInstruction(const Instruction &instruction) override final;
		virtual void pushBackInstruction(Instruction &&instruction) override final;
		virtual std::vector< Event > invokeInstructions() override final;

	  private:
		DataBits m_value;
	};

	class CGConstant : public CGObject
	{
	  public:
		CGConstant();

		virtual void reset() noexcept override final;

		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual model::Width width() const override final;
		virtual bool checkWidth(const DataBits &value) const override final;

		virtual CGObjectType objectType() const noexcept override;

		virtual CGConstant *asConstant() noexcept override final;
		virtual const CGConstant *asConstant() const noexcept override final;

		void emplaceDataBits(model::Width width, std::uint64_t value);

		virtual void pushBackInstruction(const Instruction &instruction) override final;
		virtual void pushBackInstruction(Instruction &&instruction) override final;
		virtual std::vector< Event > invokeInstructions() override final;

	  protected:
		void setDataBits(const DataBits &value);
		void setDataBits(DataBits &&value) noexcept;

		DataBits m_value;
	};

	class CGPower : public CGConstant
	{
	  public:
		virtual CGObjectType objectType() const noexcept override;

		virtual CGPower *asPower() noexcept override final;
		virtual const CGPower *asPower() const noexcept override final;

		void setWidth(model::Width width);
	};

	class CGGround : public CGConstant
	{
	  public:
		virtual CGObjectType objectType() const noexcept override;

		virtual CGGround *asGround() noexcept override final;
		virtual const CGGround *asGround() const noexcept override final;

		void setWidth(model::Width width);
	};

	class CGSplitter : public CGObject
	{
	  public:
		CGSplitter();

		virtual void reset() noexcept override final;

		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual model::Width width() const override final;
		virtual bool checkWidth(const DataBits &value) const override final;

		virtual CGObjectType objectType() const noexcept override;

		virtual CGSplitter *asSplitter() noexcept override final;
		virtual const CGSplitter *asSplitter() const noexcept override final;

		virtual void pushBackInstruction(const Instruction &instruction) override final;
		virtual void pushBackInstruction(Instruction &&instruction) override final;
		virtual std::vector< Event > invokeInstructions() override final;
	};

	class CGState : public CGObject
	{
	  public:
		CGState();

		virtual void reset() noexcept override final;

		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual model::Width width() const override final;
		virtual bool checkWidth(const DataBits &value) const override final;

		virtual void pushBackInstruction(const Instruction &instruction) override final;
		virtual void pushBackInstruction(Instruction &&instruction) override final;
	};

	class CGCollector : public CGObject
	{
	  public:
		CGCollector();
		CGCollector(const support::PointerView< CGState > &state);
		CGCollector(support::PointerView< CGState > &&state);

		virtual ~CGCollector() noexcept = default;

		virtual void reset() noexcept override final;

		support::PointerView< CGState > &state() noexcept;
		const support::PointerView< CGState > &state() const noexcept;

		void setState(const support::PointerView< CGState > &newState) noexcept;
		void setState(support::PointerView< CGState > &&newState) noexcept;

		virtual void pushBackInstruction(const Instruction &instruction) override final;
		virtual void pushBackInstruction(Instruction &&instruction) override final;
		virtual std::vector< Event > invokeInstructions() override final;

	  private:
		support::PointerView< CGState > m_state;
	};

	class CGTransistorBase : public CGCollector
	{
	  public:
		CGTransistorBase() = default;

		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual model::Width width() const override final;
		virtual bool checkWidth(const DataBits &value) const override final;

		virtual CGObjectType objectType() const noexcept override;

		virtual CGTransistorBase *asTransistorBase() noexcept override final;
		virtual const CGTransistorBase *asTransistorBase() const noexcept override final;

	  private:
		DataBits m_value;
	};

	class CGTransistorInout : public CGCollector
	{
	  public:
		CGTransistorInout() = default;

		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual model::Width width() const override final;
		virtual bool checkWidth(const DataBits &value) const override final;

		virtual CGObjectType objectType() const noexcept override;

		virtual CGTransistorInout *asTransistorInout() noexcept override final;
		virtual const CGTransistorInout *asTransistorInout() const noexcept override final;

	  private:
		DataBits m_value;
	};

	class CGTransistorState : public CGState
	{
	  public:
		CGTransistorState() = default;

		virtual CGObjectType objectType() const noexcept override;

		virtual CGTransistorState *asTransistorState() noexcept override final;
		virtual const CGTransistorState *asTransistorState() const noexcept override final;

		virtual std::vector< Event > invokeInstructions() override final;
	};
}	 // namespace lcsm

#endif /* LCSM_IR_CGOBJECT_H */
