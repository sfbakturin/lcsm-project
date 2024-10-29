#ifndef LCSM_MODEL_WIRING_WIRE_H
#define LCSM_MODEL_WIRING_WIRE_H

#include <lcsm/Component/Component.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/Support/PointerView.hpp>

#include <vector>

namespace lcsm
{
	namespace model
	{
		class Wire : public WiringComponent
		{
		  public:
			Wire() = default;
			Wire(Component *comp);

			virtual Identifier ID() const noexcept override final;
			virtual Identifier identify(Identifier ID) noexcept override final;

			virtual void connectIn(wire_t &wire, std::size_t i) override final;
			virtual void connectOut(wire_t &wire, std::size_t i) override final;

			virtual void connectWire(const wire_t &wire) override final;
			virtual void connectWire(wire_t &&wire) override final;

			void connect(const component_t &connect);
			void connect(component_t &&connect);

			const std::vector< wire_t > &wires() const noexcept;
			const std::vector< component_t > &connections() const noexcept;

			virtual WiringComponentType wiringComponentType() const noexcept override final;

			virtual Wire *asWire() noexcept override final;
			virtual const Wire *asWire() const noexcept override final;

		  private:
			Identifier m_id;

			std::vector< wire_t > m_wires;
			std::vector< component_t > m_connections;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_WIRING_WIRE_H */
