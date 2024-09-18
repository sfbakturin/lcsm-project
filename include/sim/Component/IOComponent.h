#ifndef SIM_COMPONENT_IOCOMPONENT_H
#define SIM_COMPONENT_IOCOMPONENT_H

#include <sim/Component/Component.h>

namespace sim
{
	class IOComponent : public Component
	{
	  public:
		virtual ~IOComponent() noexcept = default;

		virtual ComponentType componentType() const noexcept override;

		virtual IOComponent *asIO() noexcept override final;
		virtual const IOComponent *asIO() const noexcept override final;
	};
}	 // namespace sim

#endif /* SIM_COMPONENT_IOCOMPONENT_H */
