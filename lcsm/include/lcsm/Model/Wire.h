#ifndef LCSM_MODEL_WIRING_WIRE_H
#define LCSM_MODEL_WIRING_WIRE_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
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
				Wiring = 0,
				Connect = 1
			};

		  public:
			Wire() = default;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
			void connectToWire(const support::PointerView< Circuit > &circuit);
			void connectConnect(const support::PointerView< Circuit > &circuit);

			virtual Circuit *byPort(portid_t portId) override final;

			const std::vector< support::PointerView< Circuit > > &wires() const noexcept;
			const support::PointerView< Circuit > &connect() const noexcept;

		  private:
			std::vector< support::PointerView< Circuit > > m_wires;
			support::PointerView< Circuit > m_connect;
			Identifier m_id;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_WIRING_WIRE_H */
