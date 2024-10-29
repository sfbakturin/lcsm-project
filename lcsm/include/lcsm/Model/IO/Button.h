#ifndef LCSM_MODEL_IO_BUTTON_H
#define LCSM_MODEL_IO_BUTTON_H

#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/Wiring/Wire.h>

namespace lcsm
{
	namespace model
	{
		class Button : public IOComponent
		{
		  public:
			Button(bool activeOnPress);

			Button(const Button &other);
			Button(Button &&other) noexcept;

			Button &operator=(const Button &other);
			Button &operator=(Button &&other) noexcept;

			void swap(Button &other) noexcept;

			virtual Identifier ID() const noexcept override final;
			virtual Identifier identify(Identifier ID) noexcept override final;

			virtual void connectIn(wire_t &wire, std::size_t i) override final;
			virtual void connectOut(wire_t &wire, std::size_t i) override final;

			bool activeOnPress() const noexcept;
			void setActiveOnPress(bool activeOnPress) noexcept;

			virtual IOComponentType ioComponentType() const noexcept final;

		  private:
			Identifier m_id;

			bool m_activeOnPress;
			Wire m_wireOut;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_IO_BUTTON_H */
