#ifndef SIM_MODEL_WIRING_TUNNEL_H
#define SIM_MODEL_WIRING_TUNNEL_H

#include <sim/Component/WiringComponent.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/Support/Ref.hpp>

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

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

		virtual void ConnectWire(const wire_t &wire) override;
		virtual void ConnectWire(wire_t &&wire) override;

		void Connect(const support::Ref< Tunnel > &tunnel) noexcept;
		void Connect(support::Ref< Tunnel > &&tunnel) noexcept;

	  private:
		sim::Wire m_wire;
		support::Ref< Tunnel > m_tunnel;
	};

	using tunnel_t = support::Ref< Tunnel >;
}	 // namespace sim

#endif /* SIM_MODEL_WIRING_TUNNEL_H */
