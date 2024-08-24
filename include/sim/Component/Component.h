#ifndef SIM_COMPONENT_COMPONENT_H
#define SIM_COMPONENT_COMPONENT_H

#include <sim/Support/PointerView.hpp>

namespace sim
{
	enum ComponentType : unsigned
	{
		COMP_IO,
		COMP_WIRING,
		COMP_CIRCUIT
	};

	class IOComponent;
	class WiringComponent;
	class CircuitComponent;

	class Component
	{
	  public:
		virtual ~Component() noexcept = default;

		virtual ComponentType componentType() const noexcept = 0;

		bool IsIO() const noexcept;
		bool IsWiring() const noexcept;
		bool IsCircuit() const noexcept;

		virtual unsigned ID() const noexcept = 0;
		virtual void Identify(unsigned ID) noexcept = 0;

		virtual void
			ConnectIn(sim::support::PointerView< WiringComponent > &wire, std::size_t i) = 0;
		virtual void
			ConnectOut(sim::support::PointerView< WiringComponent > &wire, std::size_t i) = 0;

		virtual const CircuitComponent *AsCircuit() const noexcept = 0;
		virtual CircuitComponent *AsCircuit() noexcept = 0;

		virtual const IOComponent *AsIO() const noexcept = 0;
		virtual IOComponent *AsIO() noexcept = 0;

		virtual const WiringComponent *AsWiring() const noexcept = 0;
		virtual WiringComponent *AsWiring() noexcept = 0;
	};

	using component_t = support::PointerView< Component >;
}	 // namespace sim

#endif /* SIM_COMPONENT_COMPONENT_H */
