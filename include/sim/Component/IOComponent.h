#ifndef SIM_COMPONENT_IOCOMPONENT_H
#define SIM_COMPONENT_IOCOMPONENT_H

#include <sim/Component/Component.h>
#include <sim/Model/Wiring/Wire.h>

#include <cstddef>

namespace sim
{
	class IOComponent : public Component
	{
	  public:
		virtual ~IOComponent() noexcept = default;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) = 0;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) = 0;

		virtual const CircuitComponent *AsCircuit() const noexcept override;
		virtual CircuitComponent *AsCircuit() noexcept override;

		virtual const IOComponent *AsIO() const noexcept override;
		virtual IOComponent *AsIO() noexcept override;

		virtual const WiringComponent *AsWiring() const noexcept override;
		virtual WiringComponent *AsWiring() noexcept override;
	};
}	 // namespace sim

#endif /* SIM_COMPONENT_IOCOMPONENT_H */
