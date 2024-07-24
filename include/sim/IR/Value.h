#ifndef SIM_IR_VALUE_H
#define SIM_IR_VALUE_H

#include <sim/IR/Bit.h>

#include <array>
#include <cstddef>

namespace sim
{
	class Value
	{
	  public:
		Value() noexcept;
		Value(std::size_t width) noexcept;
		Value(const Value &other);
		Value(Value &&other) noexcept;

		Value &operator=(const Value &other);
		Value &operator=(Value &&other) noexcept;

		void Swap(Value &other) noexcept;

		std::size_t GetWidth() const noexcept;
		bool IsError() const noexcept;
		bool IsOK() const noexcept;

		Bit GetBit(std::size_t i) const;

	  private:
		std::size_t m_width;
		std::array< Bit, 64 > m_bits;

		void ResetBits() noexcept;
	};
}	 // namespace sim

#endif /* SIM_VALUE_H */
