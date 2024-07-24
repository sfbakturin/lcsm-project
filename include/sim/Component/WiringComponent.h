#ifndef SIM_COMPONENT_WIRINGCOMPONENT_H
#define SIM_COMPONENT_WIRINGCOMPONENT_H

#include <sim/Component/Component.h>
#include <sim/Support/Ref.hpp>

namespace sim
{
	class WiringComponent : public Component
	{
	  public:
		virtual ~WiringComponent() noexcept = default;

		virtual const CircuitComponent *AsCircuit() const noexcept override;
		virtual CircuitComponent *AsCircuit() noexcept override;

		virtual const IOComponent *AsIO() const noexcept override;
		virtual IOComponent *AsIO() noexcept override;

		virtual const WiringComponent *AsWiring() const noexcept override;
		virtual WiringComponent *AsWiring() noexcept override;

		virtual void ConnectWire(const support::Ref< WiringComponent > &wire) = 0;
		virtual void ConnectWire(support::Ref< WiringComponent > &&wire) = 0;
	};

	using wire_t = support::Ref< WiringComponent >;
}	 // namespace sim

#endif /* SIM_COMPONENT_WIRINGCOMPONENT_H */
