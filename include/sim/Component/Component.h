#ifndef SIM_COMPONENT_COMPONENT_H
#define SIM_COMPONENT_COMPONENT_H

#include <sim/Component/Identifier.h>
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

		bool isIO() const noexcept;
		bool isWiring() const noexcept;
		bool isCircuit() const noexcept;

		virtual Identifier ID() const noexcept = 0;
		virtual Identifier identify(Identifier ID) noexcept = 0;

		virtual void
			connectIn(sim::support::PointerView< WiringComponent > &wire, std::size_t i) = 0;
		virtual void
			connectOut(sim::support::PointerView< WiringComponent > &wire, std::size_t i) = 0;

		virtual CircuitComponent *asCircuit() noexcept;
		virtual const CircuitComponent *asCircuit() const noexcept;

		virtual IOComponent *asIO() noexcept;
		virtual const IOComponent *asIO() const noexcept;

		virtual WiringComponent *asWiring() noexcept;
		virtual const WiringComponent *asWiring() const noexcept;
	};

	using component_t = support::PointerView< Component >;
}	 // namespace sim

#endif /* SIM_COMPONENT_COMPONENT_H */
