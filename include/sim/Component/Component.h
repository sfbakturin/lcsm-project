#ifndef SIM_COMPONENT_COMPONENT_H
#define SIM_COMPONENT_COMPONENT_H

#include <sim/Support/Ref.hpp>

namespace sim
{
	class IOComponent;
	class WiringComponent;
	class CircuitComponent;

	class Component
	{
	  public:
		virtual ~Component() noexcept = default;

		bool IsIO() const noexcept;
		bool IsWiring() const noexcept;
		bool IsCircuit() const noexcept;

		virtual const CircuitComponent *AsCircuit() const noexcept = 0;
		virtual CircuitComponent *AsCircuit() noexcept = 0;

		virtual const IOComponent *AsIO() const noexcept = 0;
		virtual IOComponent *AsIO() noexcept = 0;

		virtual const WiringComponent *AsWiring() const noexcept = 0;
		virtual WiringComponent *AsWiring() noexcept = 0;
	};

	using component_t = Ref< Component >;
}	 // namespace sim

#endif /* SIM_COMPONENT_COMPONENT_H */
