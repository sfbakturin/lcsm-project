#ifndef LCSM_MODEL_TUNNEL_H
#define LCSM_MODEL_TUNNEL_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <vector>

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
			Tunnel() = default;
			Tunnel(label_t name);
			~Tunnel() noexcept;

			const Wire *wire() const noexcept;
			const std::vector< lcsm::Circuit * > &tunnels() const noexcept;

			virtual std::size_t numOfWires() const noexcept override final;
			virtual void provideWires(const std::vector< std::shared_ptr< model::Wire > > &wires) override final;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual object_type_t objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, Circuit *circuit) override final;

			virtual void disconnect(Circuit *circuit) noexcept override final;
			virtual void disconnectAll() noexcept override final;

			void connectToWire(Circuit *circuit);
			void connectTunnel(Circuit *circuit);

			virtual Circuit *byPort(portid_t portId) noexcept override final;
			virtual portid_t findPort(const Circuit *circuit) const noexcept override final;

			virtual portid_t defaultPort() const noexcept override final;

			virtual void dumpToLCSMFile(LCSMFileWriter &writer, LCSMBuilder &builder) const override final;

		  private:
			std::shared_ptr< Wire > m_wire;
			std::vector< Circuit * > m_tunnels;
			Identifier m_id;
		};
	}	 // namespace model

	using tunnel_t = support::PointerView< model::Tunnel >;
}	 // namespace lcsm

#endif /* LCSM_MODEL_TUNNEL_H */
