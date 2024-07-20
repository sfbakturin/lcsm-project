#ifndef SIM_MODEL_IO_DIGIT_H
#define SIM_MODEL_IO_DIGIT_H

#include <sim/Component/IOComponent.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
{
	class Digit : public IOComponent
	{
	  public:
		Digit(bool hasDecimalPoint = true);

		Digit(const Digit &other);
		Digit(Digit &&other) noexcept;

		Digit &operator=(const Digit &other);
		Digit &operator=(Digit &other) noexcept;

		void Swap(Digit &other) noexcept;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

	  private:
		bool m_hasDecimalPoint;
		sim::Wire m_data;
		sim::Wire m_decimalPoint;
	};
}	 // namespace sim

#endif /* SIM_MODEL_IO_DIGIT_H */
