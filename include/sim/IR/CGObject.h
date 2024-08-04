#ifndef SIM_IR_CGOBJECT_H
#define SIM_IR_CGOBJECT_H

#include <sim/IR/Value.h>
#include <sim/IR/Width.h>

namespace sim
{
	enum CGObjectT
	{
		OBJ_PIN_INPUT,
		OBJ_PIN_OUTPUT,
		OBJ_WIRE
	};

	class CGWire;
	class CGPinInput;
	class CGPinOutput;

	class CGObject
	{
	  public:
		virtual ~CGObject() noexcept = default;

		virtual Value &Read() = 0;
		virtual const Value &Read() const = 0;

		virtual void Write(const Value &value) = 0;
		virtual void Write(Value &&value) = 0;

		virtual Width GetWidth() const noexcept = 0;
		virtual bool CheckWidth(const Value &value) const noexcept = 0;

		virtual CGObjectT T() const noexcept = 0;

		bool IsWire() const noexcept;
		bool IsPinInput() const noexcept;
		bool IsPinOutput() const noexcept;

		virtual const CGWire *AsWire() const noexcept = 0;
		virtual CGWire *AsWire() noexcept = 0;

		virtual const CGPinInput *AsPinInput() const noexcept = 0;
		virtual CGPinInput *AsPinInput() noexcept = 0;

		virtual const CGPinOutput *AsPinOutput() const noexcept = 0;
		virtual CGPinOutput *AsPinOutput() noexcept = 0;
	};

	class CGPin : public CGObject
	{
	  public:
		CGPin() noexcept = default;

		CGPin(const Value &value);
		CGPin(Value &&value) noexcept;

		virtual Width GetWidth() const noexcept override;
		virtual bool CheckWidth(const Value &value) const noexcept override;

	  protected:
		Value m_value;
	};

	class CGPinInput : public CGPin
	{
	  public:
		virtual Value &Read() override;
		virtual const Value &Read() const override;

		virtual void Write(const Value &value) override;
		virtual void Write(Value &&value) override;

		virtual CGObjectT T() const noexcept override;

		void ExternalWrite(const Value &value);
		void ExternalWrite(Value &&value) noexcept;

		virtual const CGWire *AsWire() const noexcept override;
		virtual CGWire *AsWire() noexcept override;

		virtual const CGPinInput *AsPinInput() const noexcept override;
		virtual CGPinInput *AsPinInput() noexcept override;

		virtual const CGPinOutput *AsPinOutput() const noexcept override;
		virtual CGPinOutput *AsPinOutput() noexcept override;
	};

	class CGPinOutput : public CGPin
	{
	  public:
		virtual Value &Read() override;
		virtual const Value &Read() const override;

		virtual void Write(const Value &value) override;
		virtual void Write(Value &&value) override;

		virtual CGObjectT T() const noexcept override;

		const Value &ExternalRead() const noexcept;
		Value &ExternalRead() noexcept;

		virtual const CGWire *AsWire() const noexcept override;
		virtual CGWire *AsWire() noexcept override;

		virtual const CGPinInput *AsPinInput() const noexcept override;
		virtual CGPinInput *AsPinInput() noexcept override;

		virtual const CGPinOutput *AsPinOutput() const noexcept override;
		virtual CGPinOutput *AsPinOutput() noexcept override;
	};

	class CGWire : public CGObject
	{
	  public:
		virtual Value &Read() override;
		virtual const Value &Read() const override;

		virtual void Write(const Value &value) override;
		virtual void Write(Value &&value) override;

		virtual Width GetWidth() const noexcept override;
		virtual bool CheckWidth(const Value &value) const noexcept override;

		virtual CGObjectT T() const noexcept override;

		virtual const CGWire *AsWire() const noexcept override;
		virtual CGWire *AsWire() noexcept override;

		virtual const CGPinInput *AsPinInput() const noexcept override;
		virtual CGPinInput *AsPinInput() noexcept override;

		virtual const CGPinOutput *AsPinOutput() const noexcept override;
		virtual CGPinOutput *AsPinOutput() noexcept override;

	  private:
		Value m_value;
	};
}	 // namespace sim

#endif /* SIM_IR_CGOBJECT_H */
