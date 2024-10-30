#ifndef LCSM_IR_CGOBJECT_H
#define LCSM_IR_CGOBJECT_H

#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstdint>
#include <deque>

namespace lcsm
{
	enum CGObjectT
	{
		OBJ_PIN_INPUT,
		OBJ_PIN_OUTPUT,
		OBJ_WIRE,
		OBJ_CONSTANT,
		OBJ_POWER,
		OBJ_GROUND,
		OBJ_TRANSISTOR_BASE,
		OBJ_TRANSISTOR_INOUT,
		OBJ_TRANSISTOR_STATE
	};

	class CGWire;
	class CGPinInput;
	class CGPinOutput;
	class CGConstant;
	class CGPower;
	class CGGround;
	class CGTransistorBase;
	class CGTransistorInout;
	class CGTransistorState;

	class Instruction;

	class CGObject
	{
	  public:
		virtual ~CGObject() noexcept = default;

		virtual DataBits &read() = 0;
		virtual const DataBits &read() const = 0;

		virtual void write(const DataBits &value) = 0;
		virtual void write(DataBits &&value) = 0;

		virtual model::Width width() const = 0;
		virtual bool checkWidth(const DataBits &value) const = 0;

		virtual CGObjectT T() const noexcept = 0;

		bool isWire() const noexcept;
		bool isPinInput() const noexcept;
		bool isPinOutput() const noexcept;
		bool isConstant() const noexcept;
		bool isPower() const noexcept;
		bool isGround() const noexcept;
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
		virtual CGTransistorBase *asTransistorBase() noexcept;
		virtual const CGTransistorBase *asTransistorBase() const noexcept;
		virtual CGTransistorInout *asTransistorInout() noexcept;
		virtual const CGTransistorInout *asTransistorInout() const noexcept;
		virtual CGTransistorState *asTransistorState() noexcept;
		virtual const CGTransistorState *asTransistorState() const noexcept;

		void addInstantInstr(support::PointerView< Instruction > &&instruction);
		void addInstantInstr(const support::PointerView< Instruction > &instruction);

		void invokeInstant();

	  protected:
		std::deque< support::PointerView< Instruction > > m_instants;
	};

	using CGObjectView = support::PointerView< CGObject >;

	class CGPin : public CGObject
	{
	  public:
		CGPin() noexcept = default;

		virtual ~CGPin() noexcept = default;

		CGPin(const DataBits &value);
		CGPin(DataBits &&value) noexcept;

		virtual model::Width width() const override;
		virtual bool checkWidth(const DataBits &value) const override;

	  protected:
		DataBits m_value;
	};

	class CGPinInput : public CGPin
	{
	  public:
		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual CGObjectT T() const noexcept override;

		void externalWrite(const DataBits &value);
		void externalWrite(DataBits &&value) noexcept;

		virtual CGPinInput *asPinInput() noexcept override final;
		virtual const CGPinInput *asPinInput() const noexcept override final;
	};

	class CGPinOutput : public CGPin
	{
	  public:
		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual CGObjectT T() const noexcept override;

		DataBits &externalRead() noexcept;
		const DataBits &externalRead() const noexcept;

		virtual CGPinOutput *asPinOutput() noexcept override final;
		virtual const CGPinOutput *asPinOutput() const noexcept override final;
	};

	class CGWire : public CGObject
	{
	  public:
		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual model::Width width() const override final;
		virtual bool checkWidth(const DataBits &value) const override final;

		virtual CGObjectT T() const noexcept override;

		virtual CGWire *asWire() noexcept override final;
		virtual const CGWire *asWire() const noexcept override final;

	  private:
		DataBits m_value;
	};

	class CGConstant : public CGObject
	{
	  public:
		CGConstant() noexcept = default;

		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual model::Width width() const override final;
		virtual bool checkWidth(const DataBits &value) const override final;

		virtual CGObjectT T() const noexcept override;

		virtual CGConstant *asConstant() noexcept override final;
		virtual const CGConstant *asConstant() const noexcept override final;

		void emplaceDataBits(model::Width width, std::uint64_t value);

	  protected:
		void setDataBits(const DataBits &value);
		void setDataBits(DataBits &&value) noexcept;

		DataBits m_value;
	};

	class CGPower : public CGConstant
	{
	  public:
		virtual CGObjectT T() const noexcept override;

		virtual CGPower *asPower() noexcept override final;
		virtual const CGPower *asPower() const noexcept override final;

		void setWidth(model::Width width);
	};

	class CGGround : public CGConstant
	{
	  public:
		virtual CGObjectT T() const noexcept override;

		virtual CGGround *asGround() noexcept override final;
		virtual const CGGround *asGround() const noexcept override final;

		void setWidth(model::Width width);
	};

	class CGState : public CGObject
	{
	  public:
		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual model::Width width() const override final;
		virtual bool checkWidth(const DataBits &value) const override final;
	};

	class CGTransistorBase : public CGObject
	{
	  public:
		CGTransistorBase() noexcept = default;

		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual model::Width width() const override final;
		virtual bool checkWidth(const DataBits &value) const override final;

		virtual CGObjectT T() const noexcept override;

		virtual CGTransistorBase *asTransistorBase() noexcept override final;
		virtual const CGTransistorBase *asTransistorBase() const noexcept override final;

	  private:
		DataBits m_value;
	};

	class CGTransistorInout : public CGObject
	{
	  public:
		CGTransistorInout() noexcept = default;

		virtual DataBits &read() override final;
		virtual const DataBits &read() const override final;

		virtual void write(const DataBits &value) override final;
		virtual void write(DataBits &&value) override final;

		virtual model::Width width() const override final;
		virtual bool checkWidth(const DataBits &value) const override final;

		virtual CGObjectT T() const noexcept override;

		virtual CGTransistorInout *asTransistorInout() noexcept override final;
		virtual const CGTransistorInout *asTransistorInout() const noexcept override final;

	  private:
		DataBits m_value;
	};

	class CGTransistorState : public CGState
	{
	  public:
		CGTransistorState() noexcept = default;

		virtual CGObjectT T() const noexcept override;

		virtual CGTransistorState *asTransistorState() noexcept override final;
		virtual const CGTransistorState *asTransistorState() const noexcept override final;
	};
}	 // namespace lcsm

#endif /* LCSM_IR_CGOBJECT_H */
