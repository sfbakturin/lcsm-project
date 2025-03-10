#ifndef LCSM_VERILOG_VALUE_H
#define LCSM_VERILOG_VALUE_H

#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>

#include <ostream>
#include <string>

namespace lcsm
{
	namespace verilog
	{
		class Value
		{
		  public:
			Value() noexcept;
			Value(Bit bit) noexcept;
			Value(Strength strength, Bit bit) noexcept;

			Value(const Value &other) noexcept;
			Value(Value &&other) noexcept;

			Value &operator=(const Value &other) noexcept;
			Value &operator=(Value &&other) noexcept;

			Value operator|(const Value &other) noexcept;
			Value &operator|=(const Value &other) noexcept;

			bool operator==(const Value &other) const noexcept;
			bool operator!=(const Value &other) const noexcept;

			static int compareInner(const Value &left, const Value &right) noexcept;
			static int compareOuter(const Value &left, const Value &right) noexcept;

			bool lessInner(const Value &other) const noexcept;
			bool greaterInner(const Value &other) const noexcept;
			bool equalsInner(const Value &other) const noexcept;

			bool lessOuter(const Value &other) const noexcept;
			bool greaterOuter(const Value &other) const noexcept;
			bool equalsOuter(const Value &other) const noexcept;

			void swap(Value &other) noexcept;

			static Value one(Strength strength) noexcept;
			static Value zero(Strength strength) noexcept;

			Strength strength() const noexcept;
			void setStrength(Strength strength) noexcept;

			Bit bit() const noexcept;
			void setBit(Bit bit) noexcept;
			void flip() noexcept;

			friend std::ostream &operator<<(std::ostream &os, const Value &v);

			std::string toString() const;

		  private:
			Strength m_strength;
			Bit m_bit;
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif
