#ifndef SIM_IR_CGOBJECT_H
#define SIM_IR_CGOBJECT_H

#include <sim/IR/Value.h>
#include <sim/IR/Width.h>
#include <sim/Support/PointerView.hpp>

#include <cstdint>
#include <deque>

namespace sim
{
	enum CGObjectT
	{
		OBJ_PIN_INPUT,
		OBJ_PIN_OUTPUT,
		OBJ_WIRE,
		OBJ_CONSTANT,
		OBJ_POWER,
		OBJ_GROUND
	};

	class CGWire;
	class CGPinInput;
	class CGPinOutput;
	class CGConstant;
	class CGPower;
	class CGGround;

	class Instruction;

	class CGObject
	{
	  public:
		virtual ~CGObject() noexcept = default;

		virtual Value &read() = 0;
		virtual const Value &read() const = 0;

		virtual void write(const Value &value) = 0;
		virtual void write(Value &&value) = 0;

		virtual Width width() const noexcept = 0;
		virtual bool checkWidth(const Value &value) const noexcept = 0;

		virtual CGObjectT T() const noexcept = 0;

		bool isWire() const noexcept;
		bool isPinInput() const noexcept;
		bool isPinOutput() const noexcept;
		bool isConstant() const noexcept;
		bool isPower() const noexcept;
		bool isGround() const noexcept;

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

		CGPin(const Value &value);
		CGPin(Value &&value) noexcept;

		virtual Width width() const noexcept override;
		virtual bool checkWidth(const Value &value) const noexcept override;

	  protected:
		Value m_value;
	};

	class CGPinInput : public CGPin
	{
	  public:
		virtual Value &read() override;
		virtual const Value &read() const override;

		virtual void write(const Value &value) override;
		virtual void write(Value &&value) override;

		virtual CGObjectT T() const noexcept override;

		void externalWrite(const Value &value);
		void externalWrite(Value &&value) noexcept;

		virtual CGPinInput *asPinInput() noexcept override final;
		virtual const CGPinInput *asPinInput() const noexcept override final;
	};

	class CGPinOutput : public CGPin
	{
	  public:
		virtual Value &read() override;
		virtual const Value &read() const override;

		virtual void write(const Value &value) override;
		virtual void write(Value &&value) override;

		virtual CGObjectT T() const noexcept override;

		Value &externalRead() noexcept;
		const Value &externalRead() const noexcept;

		virtual CGPinOutput *asPinOutput() noexcept override final;
		virtual const CGPinOutput *asPinOutput() const noexcept override final;
	};

	class CGWire : public CGObject
	{
	  public:
		virtual Value &read() override;
		virtual const Value &read() const override;

		virtual void write(const Value &value) override;
		virtual void write(Value &&value) override;

		virtual Width width() const noexcept override;
		virtual bool checkWidth(const Value &value) const noexcept override;

		virtual CGObjectT T() const noexcept override;

		virtual CGWire *asWire() noexcept override final;
		virtual const CGWire *asWire() const noexcept override final;

	  private:
		Value m_value;
	};

	class CGConstant : public CGObject
	{
	  public:
		CGConstant() noexcept = default;

		virtual Value &read() override;
		virtual const Value &read() const override;

		virtual void write(const Value &value) override;
		virtual void write(Value &&value) override;

		virtual Width width() const noexcept override;
		virtual bool checkWidth(const Value &value) const noexcept override;

		virtual CGObjectT T() const noexcept override;

		virtual CGConstant *asConstant() noexcept override final;
		virtual const CGConstant *asConstant() const noexcept override final;

		void emplaceValue(Width width, std::uint64_t value);

	  protected:
		void setValue(const Value &value);
		void setValue(Value &&value) noexcept;

		Value m_value;
	};

	class CGPower : public CGConstant
	{
	  public:
		virtual CGObjectT T() const noexcept override;

		virtual CGPower *asPower() noexcept override final;
		virtual const CGPower *asPower() const noexcept override final;

		void setWidth(Width width);
	};

	class CGGround : public CGConstant
	{
	  public:
		virtual CGObjectT T() const noexcept override;

		virtual CGGround *asGround() noexcept override final;
		virtual const CGGround *asGround() const noexcept override final;

		void setWidth(Width width);
	};
}	 // namespace sim

#endif /* SIM_IR_CGOBJECT_H */
