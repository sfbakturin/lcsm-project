#ifndef LCSM_MODEL_TUNNEL_H
#define LCSM_MODEL_TUNNEL_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/lcsmconfig.h>

#include <cstddef>
#include <memory>
#include <vector>

namespace lcsm
{
	namespace model
	{
		class LCSM_API Tunnel : public Component
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
			const std::vector< lcsm::Component * > &tunnels() const noexcept;

			virtual std::size_t numOfWires() const noexcept override final;
			virtual void provideWires(const std::vector< std::shared_ptr< model::Wire > > &wires) override final;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual object_type_t objectType() const noexcept override final;
			virtual ComponentType componentType() const noexcept override final;

			virtual void connect(portid_t portId, Component *circuit) override final;

			virtual void disconnect(Component *circuit) noexcept override final;
			virtual void disconnectAll() noexcept override final;

			void connectToWire(Component *circuit);
			void connectTunnel(Component *circuit);

			virtual Component *byPort(portid_t portId) noexcept override final;
			virtual portid_t findPort(const Component *circuit) const noexcept override final;

			virtual portid_t defaultPort() const noexcept override final;

			virtual void dump(LCSMFileWriter &writer, LCSMBuilder &builder) const override final;
			virtual void copy(Component *circuit, LCSMBuilder &builder) const override final;
			virtual void from(LCSMFileReader &reader, LCSMBuilder &builder) override final;

		  private:
			std::shared_ptr< Wire > m_wire;
			std::vector< Component * > m_tunnels;
			Identifier m_id;
		};
	}	 // namespace model

	using tunnel_t = support::PointerView< model::Tunnel >;
}	 // namespace lcsm

#endif /* LCSM_MODEL_TUNNEL_H */
