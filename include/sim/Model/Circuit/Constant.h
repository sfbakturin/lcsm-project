#ifndef SIM_MODEL_CIRCUIT_CONSTANT_H
#define SIM_MODEL_CIRCUIT_CONSTANT_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Model/Width.h>
#include <sim/Model/Wiring/Wire.h>

#include <cstdint>

namespace sim
{
	class Constant : public CircuitComponent
	{
	  public:
		Constant(Width width = Width::W1, std::uint64_t value = 1);

		Constant(const Constant &other);
		Constant(Constant &&other) noexcept;

		Constant &operator=(const Constant &other);
		Constant &operator=(Constant &&other) noexcept;

		void Swap(Constant &other) noexcept;

		Width GetWidth() const noexcept;
		std::uint64_t GetValue() const noexcept;

		void SetWidth(Width newWidth) noexcept;
		void SetValue(std::uint64_t newValue) noexcept;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

		void Connect(const wire_t &wire);

	  protected:
		Width m_width;
		std::uint64_t m_value;
		sim::Wire m_wireOut;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_CONSTANT_H */
