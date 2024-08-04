#ifndef SIM_MODEL_WIRING_WIRE_H
#define SIM_MODEL_WIRING_WIRE_H

#include <sim/Component/Component.h>
#include <sim/Component/WiringComponent.h>
#include <sim/Support/PointerView.hpp>

#include <vector>

namespace sim
{
	class Wire : public WiringComponent
	{
	  public:
		Wire();
		Wire(Component *comp);

		virtual unsigned ID() const noexcept override;
		virtual void Identify(unsigned ID) noexcept override;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

		virtual void ConnectWire(const wire_t &wire) override;
		virtual void ConnectWire(wire_t &&wire) override;

		void Connect(const component_t &connect);
		void Connect(component_t &&connect);

		const std::vector< wire_t > &Wires() const noexcept;
		const std::vector< component_t > &Connections() const noexcept;

		virtual const Wire *AsWire() const noexcept override;
		virtual Wire *AsWire() noexcept override;

	  private:
		unsigned m_id;

		std::vector< wire_t > m_wires;
		std::vector< component_t > m_connections;
	};
}	 // namespace sim

#endif /* SIM_MODEL_WIRING_WIRE_H */
