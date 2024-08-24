#ifndef SIM_MODEL_CIRCUIT_PIN_H
#define SIM_MODEL_CIRCUIT_PIN_H

#include <sim/Component/CircuitComponent.h>
#include <sim/IR/Width.h>
#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
{
	class Pin : public CircuitComponent
	{
	  public:
		Pin(bool output, Width width);

		Pin(const Pin &other);
		Pin(Pin &&other) noexcept;

		Pin &operator=(const Pin &other);
		Pin &operator=(Pin &&other) noexcept;

		void Swap(Pin &other) noexcept;

		bool IsOutput() const noexcept;
		Width GetWidth() const noexcept;
		Wire &GetWire() noexcept;
		const Wire &GetWire() const noexcept;

		void SetOutput(bool newOutput) noexcept;
		void SetWidth(Width newWidth) noexcept;

		virtual unsigned ID() const noexcept override;
		virtual void Identify(unsigned ID) noexcept override;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

		void Connect(wire_t &wire);

		virtual CircuitComponentType circuitComponentType() const noexcept override;

		virtual const Pin *AsPin() const noexcept override;
		virtual Pin *AsPin() noexcept override;

	  private:
		unsigned m_id;

		bool m_output;
		Width m_width;
		Wire m_wire;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_PIN_H */
