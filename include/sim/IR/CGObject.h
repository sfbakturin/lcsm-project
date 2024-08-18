#ifndef SIM_IR_CGOBJECT_H
#define SIM_IR_CGOBJECT_H

#include <sim/IR/Value.h>
#include <sim/IR/Width.h>
#include <sim/Support/PointerView.hpp>

#include <deque>

namespace sim
{
	enum CGObjectT
	{
		OBJ_PIN_INPUT,
		OBJ_PIN_OUTPUT,
		OBJ_WIRE
	};

	class CGNode;

	class CGWire;
	class CGPinInput;
	class CGPinOutput;

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

		virtual CGWire *asWire() noexcept = 0;
		virtual const CGWire *asWire() const noexcept = 0;
		virtual CGPinInput *asPinInput() noexcept = 0;
		virtual const CGPinInput *asPinInput() const noexcept = 0;
		virtual CGPinOutput *asPinOutput() noexcept = 0;
		virtual const CGPinOutput *asPinOutput() const noexcept = 0;

		void addInstantInstr(support::PointerView< Instruction > &&instruction);
		void addInstantInstr(const support::PointerView< Instruction > &instruction);

		void invokeInstant();

	  protected:
		std::deque< support::PointerView< Instruction > > m_instants;
	};

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

		virtual CGWire *asWire() noexcept override;
		virtual const CGWire *asWire() const noexcept override;
		virtual CGPinInput *asPinInput() noexcept override;
		virtual const CGPinInput *asPinInput() const noexcept override;
		virtual CGPinOutput *asPinOutput() noexcept override;
		virtual const CGPinOutput *asPinOutput() const noexcept override;
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

		virtual CGWire *asWire() noexcept override;
		virtual const CGWire *asWire() const noexcept override;
		virtual CGPinInput *asPinInput() noexcept override;
		virtual const CGPinInput *asPinInput() const noexcept override;
		virtual CGPinOutput *asPinOutput() noexcept override;
		virtual const CGPinOutput *asPinOutput() const noexcept override;
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

		virtual CGWire *asWire() noexcept override;
		virtual const CGWire *asWire() const noexcept override;
		virtual CGPinInput *asPinInput() noexcept override;
		virtual const CGPinInput *asPinInput() const noexcept override;
		virtual CGPinOutput *asPinOutput() noexcept override;
		virtual const CGPinOutput *asPinOutput() const noexcept override;

	  private:
		Value m_value;
	};

	using CGObjectView = support::PointerView< CGObject >;
}	 // namespace sim

#endif /* SIM_IR_CGOBJECT_H */
