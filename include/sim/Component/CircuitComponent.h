#ifndef SIM_COMPONENT_CIRCUITCOMPONENT_H
#define SIM_COMPONENT_CIRCUITCOMPONENT_H

#include <sim/Component/Component.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
{
	enum CircuitComponentType : unsigned
	{
		CIRCUIT_COMP_PIN,
		CIRCUIT_COMP_CONSTANT,
		CIRCUIT_COMP_POWER,
		CIRCUIT_COMP_GROUND,
		CIRCUIT_COMP_SPLITTER,
		CIRCUIT_COMP_CLOCK,
		CIRCUIT_COMP_TRANSISTOR,
		CIRCUIT_COMP_TRANSMISSION_GATE
	};

	class Pin;

	class CircuitComponent : public Component
	{
	  public:
		virtual ~CircuitComponent() noexcept = default;

		virtual ComponentType componentType() const noexcept override;

		virtual const CircuitComponent *AsCircuit() const noexcept override;
		virtual CircuitComponent *AsCircuit() noexcept override;

		virtual const IOComponent *AsIO() const noexcept override;
		virtual IOComponent *AsIO() noexcept override;

		virtual const WiringComponent *AsWiring() const noexcept override;
		virtual WiringComponent *AsWiring() noexcept override;

		virtual CircuitComponentType circuitComponentType() const noexcept = 0;

		bool IsPin() const noexcept;

		virtual const Pin *AsPin() const noexcept = 0;
		virtual Pin *AsPin() noexcept = 0;
	};
}	 // namespace sim

#endif /* SIM_COMPONENT_CIRCUITCOMPONENT_H */
