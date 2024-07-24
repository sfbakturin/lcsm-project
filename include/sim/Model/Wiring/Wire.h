#ifndef SIM_MODEL_WIRING_WIRE_H
#define SIM_MODEL_WIRING_WIRE_H

#include <sim/Component/Component.h>
#include <sim/Component/WiringComponent.h>
#include <sim/Support/Ref.hpp>

#include <vector>

namespace sim
{
	class Wire : public WiringComponent
	{
	  public:
		Wire() = default;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

		virtual void ConnectWire(const wire_t &wire) override;
		virtual void ConnectWire(wire_t &&wire) override;

		void Connect(const component_t &connect);
		void Connect(component_t &&connect);

	  private:
		std::vector< wire_t > m_wires;
		std::vector< component_t > m_connections;
	};
}	 // namespace sim

#endif /* SIM_MODEL_WIRING_WIRE_H */
