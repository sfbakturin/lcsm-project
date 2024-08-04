#ifndef SIM_MODEL_IO_DIGIT_H
#define SIM_MODEL_IO_DIGIT_H

#include <sim/Component/IOComponent.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
{
	class Digit : public IOComponent
	{
	  public:
		Digit(bool hasDecimalPoint);

		Digit(const Digit &other);
		Digit(Digit &&other) noexcept;

		Digit &operator=(const Digit &other);
		Digit &operator=(Digit &other) noexcept;

		void Swap(Digit &other) noexcept;

		virtual unsigned ID() const noexcept override;
		virtual void Identify(unsigned ID) noexcept override;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

	  private:
		unsigned m_id;

		bool m_hasDecimalPoint;
		sim::Wire m_data;
		sim::Wire m_decimalPoint;
	};
}	 // namespace sim

#endif /* SIM_MODEL_IO_DIGIT_H */
