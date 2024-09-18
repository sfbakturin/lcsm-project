#ifndef SIM_COMPONENT_CIRCUITCOMPONENT_H
#define SIM_COMPONENT_CIRCUITCOMPONENT_H

#include <sim/Component/Component.h>

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
	class Constant;
	class Power;
	class Ground;
	class Transistor;

	class CircuitComponent : public Component
	{
	  public:
		virtual ~CircuitComponent() noexcept = default;

		virtual ComponentType componentType() const noexcept override;

		virtual CircuitComponent *asCircuit() noexcept override final;
		virtual const CircuitComponent *asCircuit() const noexcept override final;

		virtual CircuitComponentType circuitComponentType() const noexcept = 0;

		bool isPin() const noexcept;
		bool isConstant() const noexcept;
		bool isGround() const noexcept;
		bool isPower() const noexcept;
		bool isTransistor() const noexcept;

		virtual Pin *asPin() noexcept;
		virtual const Pin *asPin() const noexcept;

		virtual Constant *asConstant() noexcept;
		virtual const Constant *asConstant() const noexcept;

		virtual Power *asPower() noexcept;
		virtual const Power *asPower() const noexcept;

		virtual Ground *asGround() noexcept;
		virtual const Ground *asGround() const noexcept;

		virtual Transistor *asTransistor() noexcept;
		virtual const Transistor *asTransistor() const noexcept;
	};
}	 // namespace sim

#endif /* SIM_COMPONENT_CIRCUITCOMPONENT_H */
