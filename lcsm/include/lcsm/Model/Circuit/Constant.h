#ifndef LCSM_MODEL_CIRCUIT_CONSTANT_H
#define LCSM_MODEL_CIRCUIT_CONSTANT_H

#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wiring/Wire.h>

#include <cstdint>

namespace lcsm
{
	namespace model
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

			void swap(Constant &other) noexcept;

			Width width() const noexcept;
			std::uint64_t value() const noexcept;

			void setWidth(Width newWidth) noexcept;
			void setValue(std::uint64_t newValue) noexcept;

			Wire &wire() noexcept;
			const Wire &wire() const noexcept;

			virtual Identifier ID() const noexcept override final;
			virtual Identifier identify(Identifier ID) noexcept override final;

			virtual void connectIn(wire_t &wire, std::size_t i) override final;
			virtual void connectOut(wire_t &wire, std::size_t i) override final;

			void connect(wire_t &wire);

			virtual CircuitComponentType circuitComponentType() const noexcept override;

			virtual Constant *asConstant() noexcept override final;
			virtual const Constant *asConstant() const noexcept override final;

		  protected:
			Identifier m_id;

			Width m_width;
			std::uint64_t m_value;

			Wire m_wireOut;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_CIRCUIT_CONSTANT_H */
