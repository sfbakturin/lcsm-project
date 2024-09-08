#ifndef SIM_MODEL_CIRCUIT_CONSTANT_H
#define SIM_MODEL_CIRCUIT_CONSTANT_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Identifier.h>
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

		Wire &wire() noexcept;
		const Wire &wire() const noexcept;

		virtual Identifier ID() const noexcept override final;
		virtual Identifier identify(Identifier ID) noexcept override final;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

		void Connect(wire_t &wire);

		virtual CircuitComponentType circuitComponentType() const noexcept override;

		virtual Constant *asConstant() noexcept override final;
		virtual const Constant *asConstant() const noexcept override final;

	  protected:
		Identifier m_id;

		Width m_width;
		std::uint64_t m_value;
		Wire m_wireOut;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_CONSTANT_H */
