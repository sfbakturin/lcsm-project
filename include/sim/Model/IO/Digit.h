#ifndef SIM_MODEL_IO_DIGIT_H
#define SIM_MODEL_IO_DIGIT_H

#include <sim/Component/IOComponent.h>
#include <sim/Component/Identifier.h>
#include <sim/Model/Wiring/Wire.h>

#include <cstddef>

namespace sim
{
	class Digit : public IOComponent
	{
	  public:
		enum CompositeIndex : std::size_t
		{
			DIGIT_INDEX_DATA = 0,
			DIGIT_INDEX_DECIMAL_POINT = 1
		};

		Digit(bool hasDecimalPoint);

		Digit(const Digit &other);
		Digit(Digit &&other) noexcept;

		Digit &operator=(const Digit &other);
		Digit &operator=(Digit &other) noexcept;

		void swap(Digit &other) noexcept;

		virtual Identifier ID() const noexcept override final;
		virtual Identifier identify(Identifier ID) noexcept override final;

		virtual void connectIn(wire_t &wire, std::size_t i) override final;
		virtual void connectOut(wire_t &wire, std::size_t i) override final;

		bool hasDecimalPoint() const noexcept;
		void setHasDecimalPoint(bool hasDecimalPoint) noexcept;

		virtual IOComponentType ioComponentType() const noexcept final;

	  private:
		Identifier m_id;

		bool m_hasDecimalPoint;
		Wire m_data;
		Wire m_decimalPoint;
	};
}	 // namespace sim

#endif /* SIM_MODEL_IO_DIGIT_H */
