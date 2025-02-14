#ifndef LCSM_MODEL_TUNNEL_H
#define LCSM_MODEL_TUNNEL_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>

namespace lcsm
{
	namespace model
	{
		class Tunnel : public Circuit
		{
		  public:
			enum Port : portid_t
			{
				Wiring,
				Tunneling
			};

		  public:
			Tunnel();

			const Wire &wire() const noexcept;
			const support::PointerView< Circuit > &tunnel() const noexcept;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
			void connectToWire(const support::PointerView< Circuit > &circuit);
			void connectTunnel(const support::PointerView< Circuit > &circuit);

			virtual Circuit *byPort(portid_t portId) override final;

		  private:
			Wire m_wire;
			support::PointerView< Circuit > m_tunnel;
			Identifier m_id;
		};
	}	 // namespace model

	using tunnel_t = support::PointerView< model::Tunnel >;
}	 // namespace lcsm

#endif /* LCSM_MODEL_TUNNEL_H */
