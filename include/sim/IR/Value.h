#ifndef SIM_IR_VALUE_H
#define SIM_IR_VALUE_H

#include <sim/IR/Bit.h>
#include <sim/IR/Width.h>

#include <array>
#include <cstddef>

namespace sim
{
	class Value
	{
	  public:
		Value() noexcept;
		Value(sim::Width width) noexcept;

		Value(const Value &other);
		Value(Value &&other) noexcept;

		Value &operator=(const Value &other);
		Value &operator=(Value &&other) noexcept;

		bool operator==(const Value &other);
		bool operator!=(const Value &other);

		void Swap(Value &other) noexcept;

		sim::Width GetWidth() const noexcept;
		bool IsError() const noexcept;
		bool IsOK() const noexcept;

		LogisimBit GetBit(std::size_t i) const;

	  private:
		static constexpr std::size_t VALUE_SIZE = Width::W64;

		sim::Width m_width;
		std::array< LogisimBit, VALUE_SIZE > m_bits;

		void ResetBits() noexcept;
	};
}	 // namespace sim

#endif /* SIM_VALUE_H */
