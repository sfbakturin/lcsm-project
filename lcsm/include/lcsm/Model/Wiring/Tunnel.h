#ifndef LCSM_MODEL_WIRING_TUNNEL_H
#define LCSM_MODEL_WIRING_TUNNEL_H

#include <lcsm/Component/Identifier.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/Model/Wiring/Wire.h>
#include <lcsm/Support/PointerView.hpp>

namespace lcsm
{
	namespace model
	{
		class Tunnel : public WiringComponent
		{
		  public:
			Tunnel() = default;

			Tunnel(const Tunnel &other);
			Tunnel(Tunnel &&other) noexcept;

			Tunnel &operator=(const Tunnel &other);
			Tunnel &operator=(Tunnel &&other) noexcept;

			void swap(Tunnel &other) noexcept;

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
	}	 // namespace model

	using tunnel_t = support::PointerView< model::Tunnel >;
}	 // namespace lcsm

#endif /* LCSM_MODEL_WIRING_TUNNEL_H */
