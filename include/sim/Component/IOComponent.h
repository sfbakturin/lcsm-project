#ifndef SIM_COMPONENT_IOCOMPONENT_H
#define SIM_COMPONENT_IOCOMPONENT_H

#include <sim/Component/Component.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
{
	class IOComponent : public Component
	{
	  public:
		virtual ~IOComponent() noexcept = default;

		virtual ComponentType componentType() const noexcept override;

		virtual const CircuitComponent *AsCircuit() const noexcept override;
		virtual CircuitComponent *AsCircuit() noexcept override;

		virtual const IOComponent *AsIO() const noexcept override;
		virtual IOComponent *AsIO() noexcept override;

		virtual const WiringComponent *AsWiring() const noexcept override;
		virtual WiringComponent *AsWiring() noexcept override;
	};
}	 // namespace sim

#endif /* SIM_COMPONENT_IOCOMPONENT_H */
