#ifndef LCSM_COMPONENT_WIRINGCOMPONENT_H
#define LCSM_COMPONENT_WIRINGCOMPONENT_H

#include <lcsm/Component/Component.h>
#include <lcsm/Support/PointerView.hpp>

namespace lcsm
{
	enum WiringComponentType
	{
		WIRING_COMP_WIRE,
		WIRING_COMP_TUNNEL
	};

	namespace model
	{
		class Wire;
		class Tunnel;
	}	 // namespace model

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

		virtual model::Wire *asWire() noexcept;
		virtual const model::Wire *asWire() const noexcept;

		virtual model::Tunnel *asTunnel() noexcept;
		virtual const model::Tunnel *asTunnel() const noexcept;
	};

	using wire_t = support::PointerView< WiringComponent >;
}	 // namespace lcsm

#endif /* LCSM_COMPONENT_WIRINGCOMPONENT_H */
