#ifndef SIM_COMPONENT_WIRINGCOMPONENT_H
#define SIM_COMPONENT_WIRINGCOMPONENT_H

#include <sim/Component/Component.h>
#include <sim/Support/PointerView.hpp>

namespace sim
{
	enum WiringComponentType
	{
		WIRING_COMP_WIRE,
		WIRING_COMP_TUNNEL
	};

	class Wire;
	class Tunnel;

	class WiringComponent : public Component
	{
	  public:
		virtual ~WiringComponent() noexcept = default;

		virtual ComponentType componentType() const noexcept override;

		virtual WiringComponent *asWiring() noexcept override final;
		virtual const WiringComponent *asWiring() const noexcept override final;

		virtual void connectWire(const support::PointerView< WiringComponent > &wire) = 0;
		virtual void connectWire(support::PointerView< WiringComponent > &&wire) = 0;

		virtual WiringComponentType wiringComponentType() const noexcept = 0;

		bool isWire() const noexcept;
		bool isTunnel() const noexcept;

		virtual Wire *asWire() noexcept;
		virtual const Wire *asWire() const noexcept;

		virtual Tunnel *asTunnel() noexcept;
		virtual const Tunnel *asTunnel() const noexcept;
	};

	using wire_t = support::PointerView< WiringComponent >;
}	 // namespace sim

#endif /* SIM_COMPONENT_WIRINGCOMPONENT_H */
