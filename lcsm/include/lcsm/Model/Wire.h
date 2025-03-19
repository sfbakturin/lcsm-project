#ifndef LCSM_MODEL_WIRE_H
#define LCSM_MODEL_WIRE_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/PointerView.hpp>

#include <vector>

namespace lcsm
{
	namespace model
	{
		class Wire : public Circuit
		{
		  public:
			enum Port : portid_t
			{
				Wiring,
				Connect
			};

		  public:
			Wire() = default;
			Wire(label_t name);
			~Wire() noexcept;

			const std::vector< support::PointerView< Circuit > > &wires() const noexcept;
			const support::PointerView< Circuit > &connect() const noexcept;

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
			void connectConnect(Circuit *circuit);

			virtual Circuit *byPort(portid_t portId) noexcept override final;
			virtual portid_t findPort(const Circuit *circuit) const noexcept override final;

			virtual portid_t defaultPort() const noexcept override final;

			virtual void dump(LCSMFileWriter &writer, LCSMBuilder &builder) const override final;
			virtual void copy(Circuit *circuit, LCSMBuilder &builder) const override final;
			virtual void from(LCSMFileReader &reader, LCSMBuilder &builder) override final;

		  private:
			std::vector< support::PointerView< Circuit > > m_wires;
			support::PointerView< Circuit > m_connect;
			Identifier m_id;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_WIRE_H */
