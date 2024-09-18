#ifndef SIM_MODEL_WIRING_TUNNEL_H
#define SIM_MODEL_WIRING_TUNNEL_H

#include <sim/Component/Identifier.h>
#include <sim/Component/WiringComponent.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/Support/PointerView.hpp>

namespace sim
{
	class Tunnel : public WiringComponent
	{
	  public:
		Tunnel() = default;

		Tunnel(const Tunnel &other);
		Tunnel(Tunnel &&other) noexcept;

		Tunnel &operator=(const Tunnel &other);
		Tunnel &operator=(Tunnel &&other) noexcept;

		void Swap(Tunnel &other) noexcept;

		virtual Identifier ID() const noexcept override final;
		virtual Identifier identify(Identifier ID) noexcept override final;

		virtual void connectIn(wire_t &wire, std::size_t i) override final;
		virtual void connectOut(wire_t &wire, std::size_t i) override final;

		virtual void connectWire(const wire_t &wire) override final;
		virtual void connectWire(wire_t &&wire) override final;

		void connect(const support::PointerView< Tunnel > &tunnel) noexcept;
		void connect(support::PointerView< Tunnel > &&tunnel) noexcept;

		virtual WiringComponentType wiringComponentType() const noexcept override final;

		virtual Tunnel *asTunnel() noexcept override final;
		virtual const Tunnel *asTunnel() const noexcept override final;

	  private:
		Identifier m_id;

		Wire m_wire;
		support::PointerView< Tunnel > m_tunnel;
	};

	using tunnel_t = support::PointerView< Tunnel >;
}	 // namespace sim

#endif /* SIM_MODEL_WIRING_TUNNEL_H */
