#ifndef LCSM_COMPONENT_IOCOMPONENT_H
#define LCSM_COMPONENT_IOCOMPONENT_H

#include <lcsm/Component/Component.h>

namespace lcsm
{
	enum IOComponentType : unsigned
	{
		IO_COMP_BUTTON,
		IO_COMP_DIGIT,
		IO_COMP_PROBE
	};

	namespace model
	{
		class Button;
		class Digit;
		class Probe;
	}	 // namespace model

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

		virtual model::Button *asButton() noexcept;
		virtual const model::Button *asButton() const noexcept;
		virtual model::Digit *asDigit() noexcept;
		virtual const model::Digit *asDigit() const noexcept;
		virtual model::Probe *asProbe() noexcept;
		virtual const model::Probe *asProbe() const noexcept;
	};
}	 // namespace lcsm

#endif /* LCSM_COMPONENT_IOCOMPONENT_H */
