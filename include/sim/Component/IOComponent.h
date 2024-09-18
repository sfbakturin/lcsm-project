#ifndef SIM_COMPONENT_IOCOMPONENT_H
#define SIM_COMPONENT_IOCOMPONENT_H

#include <sim/Component/Component.h>

namespace sim
{
	enum IOComponentType : unsigned
	{
		IO_COMP_BUTTON,
		IO_COMP_DIGIT,
		IO_COMP_PROBE
	};

	class Button;
	class Digit;
	class Probe;

	class IOComponent : public Component
	{
	  public:
		virtual ~IOComponent() noexcept = default;

		virtual ComponentType componentType() const noexcept override;

		virtual IOComponent *asIO() noexcept override final;
		virtual const IOComponent *asIO() const noexcept override final;

		virtual IOComponentType ioComponentType() const noexcept = 0;

		bool isButton() const noexcept;
		bool isDigit() const noexcept;
		bool isProbe() const noexcept;

		virtual Button *asButton() noexcept;
		virtual const Button *asButton() const noexcept;
		virtual Digit *asDigit() noexcept;
		virtual const Digit *asDigit() const noexcept;
		virtual Probe *asProbe() noexcept;
		virtual const Probe *asProbe() const noexcept;
	};
}	 // namespace sim

#endif /* SIM_COMPONENT_IOCOMPONENT_H */
