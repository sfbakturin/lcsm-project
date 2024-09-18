#ifndef SIM_MODEL_IO_BUTTON_H
#define SIM_MODEL_IO_BUTTON_H

#include <sim/Component/IOComponent.h>
#include <sim/Component/Identifier.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
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

	  private:
		Identifier m_id;

		bool m_activeOnPress;
		Wire m_wireOut;
	};
}	 // namespace sim

#endif /* SIM_MODEL_IO_BUTTON_H */
