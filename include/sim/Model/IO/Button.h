#ifndef SIM_MODEL_IO_BUTTON_H
#define SIM_MODEL_IO_BUTTON_H

#include <sim/Component/IOComponent.h>
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

		void Swap(Button &other) noexcept;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

	  private:
		bool m_activeOnPress;
		sim::Wire m_wireOut;
	};
}	 // namespace sim

#endif /* SIM_MODEL_IO_BUTTON_H */
