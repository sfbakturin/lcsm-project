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

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

		virtual void ConnectWire(const wire_t &wire) override;
		virtual void ConnectWire(wire_t &&wire) override;

		void Connect(const support::PointerView< Tunnel > &tunnel) noexcept;
		void Connect(support::PointerView< Tunnel > &&tunnel) noexcept;

		virtual WiringComponentType wiringComponentType() const noexcept override;

		virtual const Wire *AsWire() const noexcept override;
		virtual Wire *AsWire() noexcept override;

	  private:
		Identifier m_id;

		Wire m_wire;
		support::PointerView< Tunnel > m_tunnel;
	};

	using tunnel_t = support::PointerView< Tunnel >;
}	 // namespace sim

#endif /* SIM_MODEL_WIRING_TUNNEL_H */
