#ifndef SIM_MODEL_CIRCUIT_PIN_H
#define SIM_MODEL_CIRCUIT_PIN_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Component/PinComponent.h>
#include <sim/IR/Width.h>
#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
{
	class Pin : public CircuitComponent, public PinComponent
	{
	  public:
		Pin(bool output, Width width, std::uint64_t value);

		Pin(const Pin &other);
		Pin(Pin &&other) noexcept;

		Pin &operator=(const Pin &other);
		Pin &operator=(Pin &&other) noexcept;

		void Swap(Pin &other) noexcept;

		Width GetWidth() const noexcept;
		std::uint64_t GetValue() const noexcept;

		void SetWidth(Width newWidth) noexcept;
		void SetValue(std::uint64_t newValue) noexcept;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

		void Connect(const wire_t &wire);

	  private:
		bool m_output;
		Width m_width;
		std::uint64_t m_value;
		sim::Wire m_wire;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_PIN_H */
