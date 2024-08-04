#ifndef SIM_MODEL_CIRCUIT_CONSTANT_H
#define SIM_MODEL_CIRCUIT_CONSTANT_H

#include <sim/Component/CircuitComponent.h>
#include <sim/IR/Width.h>
#include <sim/Model/Wiring/Wire.h>

#include <cstdint>

namespace sim
{
	class Pin;

	class Constant : public CircuitComponent
	{
	  public:
		Constant(Width width, std::uint64_t value);

		Constant(const Constant &other);
		Constant(Constant &&other) noexcept;

		Constant &operator=(const Constant &other);
		Constant &operator=(Constant &&other) noexcept;

		void Swap(Constant &other) noexcept;

		Width GetWidth() const noexcept;
		std::uint64_t GetValue() const noexcept;

		void SetWidth(Width newWidth) noexcept;
		void SetValue(std::uint64_t newValue) noexcept;

		virtual unsigned ID() const noexcept override;
		virtual void Identify(unsigned ID) noexcept override;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

		void Connect(wire_t &wire);

		virtual const Pin *AsPin() const noexcept override;
		virtual Pin *AsPin() noexcept override;

	  protected:
		unsigned m_id;

		Width m_width;
		std::uint64_t m_value;
		Wire m_wireOut;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_CONSTANT_H */
