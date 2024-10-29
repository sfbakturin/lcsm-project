#ifndef LCSM_COMPONENT_CIRCUITCOMPONENT_H
#define LCSM_COMPONENT_CIRCUITCOMPONENT_H

#include <lcsm/Component/Component.h>

namespace lcsm
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

	namespace model
	{
		class Pin;
		class Constant;
		class Power;
		class Ground;
		class Transistor;
	}	 // namespace model

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

		virtual model::Pin *asPin() noexcept;
		virtual const model::Pin *asPin() const noexcept;

		virtual model::Constant *asConstant() noexcept;
		virtual const model::Constant *asConstant() const noexcept;

		virtual model::Power *asPower() noexcept;
		virtual const model::Power *asPower() const noexcept;

		virtual model::Ground *asGround() noexcept;
		virtual const model::Ground *asGround() const noexcept;

		virtual model::Transistor *asTransistor() noexcept;
		virtual const model::Transistor *asTransistor() const noexcept;
	};
}	 // namespace lcsm

#endif /* LCSM_COMPONENT_CIRCUITCOMPONENT_H */
