#ifndef SIM_MODEL_IO_DIGIT_H
#define SIM_MODEL_IO_DIGIT_H

#include <sim/Component/IOComponent.h>
#include <sim/Component/Identifier.h>
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

		virtual Identifier ID() const noexcept override final;
		virtual Identifier identify(Identifier ID) noexcept override final;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

	  private:
		Identifier m_id;

		bool m_hasDecimalPoint;
		Wire m_data;
		Wire m_decimalPoint;
	};
}	 // namespace sim

#endif /* SIM_MODEL_IO_DIGIT_H */
