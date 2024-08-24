#ifndef SIM_COMPONENT_WIRINGCOMPONENT_H
#define SIM_COMPONENT_WIRINGCOMPONENT_H

#include <sim/Component/Component.h>
#include <sim/Support/PointerView.hpp>

namespace sim
{
	enum WiringComponentType
	{
		WIRING_COMP_WIRE,
		WIRING_COMP_TUNNEL
	};

	class Wire;

	class WiringComponent : public Component
	{
	  public:
		virtual ~WiringComponent() noexcept = default;

		virtual ComponentType componentType() const noexcept override;

		virtual const CircuitComponent *AsCircuit() const noexcept override;
		virtual CircuitComponent *AsCircuit() noexcept override;

		virtual const IOComponent *AsIO() const noexcept override;
		virtual IOComponent *AsIO() noexcept override;

		virtual const WiringComponent *AsWiring() const noexcept override;
		virtual WiringComponent *AsWiring() noexcept override;

		virtual void ConnectWire(const support::PointerView< WiringComponent > &wire) = 0;
		virtual void ConnectWire(support::PointerView< WiringComponent > &&wire) = 0;

		virtual WiringComponentType wiringComponentType() const noexcept = 0;

		bool IsWire() const noexcept;

		virtual const Wire *AsWire() const noexcept = 0;
		virtual Wire *AsWire() noexcept = 0;
	};

	using wire_t = support::PointerView< WiringComponent >;
}	 // namespace sim

#endif /* SIM_COMPONENT_WIRINGCOMPONENT_H */
