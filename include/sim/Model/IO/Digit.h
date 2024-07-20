#ifndef SIM_MODEL_IO_DIGIT_H
#define SIM_MODEL_IO_DIGIT_H

#include <sim/Component/IOComponent.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
{
	class Digit : public IOComponent
	{
	  public:
		Digit() noexcept;
		Digit(bool hasDecimalPoint) noexcept;

		Digit(const Digit &other) noexcept;
		Digit(Digit &&other) noexcept;

		Digit &operator=(const Digit &other) noexcept;
		Digit &operator=(Digit &other) noexcept;

		void Swap(Digit &other) noexcept;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

	  private:
		bool m_hasDecimalPoint;
		wire_t m_data;
		wire_t m_decimalPoint;
	};
}	 // namespace sim

#endif /* SIM_MODEL_IO_DIGIT_H */
