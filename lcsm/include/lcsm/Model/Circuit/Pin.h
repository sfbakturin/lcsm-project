#ifndef LCSM_MODEL_CIRCUIT_PIN_H
#define LCSM_MODEL_CIRCUIT_PIN_H

#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/Circuit/Constant.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wiring/Wire.h>

namespace lcsm
{
	namespace model
	{
		class Pin : public CircuitComponent
		{
		  public:
			Pin(bool output, Width width);

			Pin(const Pin &other);
			Pin(Pin &&other) noexcept;

			Pin &operator=(const Pin &other);
			Pin &operator=(Pin &&other) noexcept;

			void swap(Pin &other) noexcept;

			bool isOutput() const noexcept;
			Width width() const noexcept;
			Wire &wire() noexcept;
			const Wire &wire() const noexcept;

			void setOutput(bool newOutput) noexcept;
			void setWidth(Width newWidth) noexcept;

			virtual Identifier ID() const noexcept override final;
			virtual Identifier identify(Identifier ID) noexcept override final;

			virtual void connectIn(wire_t &wire, std::size_t i) override;
			virtual void connectOut(wire_t &wire, std::size_t i) override;

			void connect(wire_t &wire);

			virtual CircuitComponentType circuitComponentType() const noexcept override final;

			virtual Pin *asPin() noexcept override final;
			virtual const Pin *asPin() const noexcept override final;

		  private:
			Identifier m_id;

			bool m_output;
			Width m_width;
			Wire m_wire;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_CIRCUIT_PIN_H */
