#ifndef SIM_MODEL_WIRING_TUNNEL_H
#define SIM_MODEL_WIRING_TUNNEL_H

#include <sim/Component/WiringComponent.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/Support/PointerView.hpp>

namespace sim
{
	class Tunnel : public WiringComponent
	{
	  public:
		Tunnel();

		Tunnel(const Tunnel &other);
		Tunnel(Tunnel &&other) noexcept;

		Tunnel &operator=(const Tunnel &other);
		Tunnel &operator=(Tunnel &&other) noexcept;

		void Swap(Tunnel &other) noexcept;

		virtual unsigned ID() const noexcept override;
		virtual void Identify(unsigned ID) noexcept override;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

		virtual void ConnectWire(const wire_t &wire) override;
		virtual void ConnectWire(wire_t &&wire) override;

		void Connect(const support::PointerView< Tunnel > &tunnel) noexcept;
		void Connect(support::PointerView< Tunnel > &&tunnel) noexcept;

		virtual const Wire *AsWire() const noexcept override;
		virtual Wire *AsWire() noexcept override;

	  private:
		unsigned m_id;

		sim::Wire m_wire;
		support::PointerView< Tunnel > m_tunnel;
	};

	using tunnel_t = support::PointerView< Tunnel >;
}	 // namespace sim

#endif /* SIM_MODEL_WIRING_TUNNEL_H */
