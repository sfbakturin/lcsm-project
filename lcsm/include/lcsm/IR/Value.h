#ifndef LCSM_IR_VALUE_H
#define LCSM_IR_VALUE_H

#include <initializer_list>
#include <lcsm/IR/Bit.h>
#include <lcsm/IR/Width.h>

#include <array>
#include <cstddef>

namespace lcsm
{
	class Value
	{
	  public:
		Value() noexcept;

		Value(Width width) noexcept;
		Value(Width width, LogisimBit bit) noexcept;
		Value(std::initializer_list< LogisimBit > value);

		Value(const Value &other);
		Value(Value &&other) noexcept;

		Value &operator=(const Value &other);
		Value &operator=(Value &&other) noexcept;

		bool operator==(const Value &other);
		bool operator!=(const Value &other);

		void swap(Value &other) noexcept;

		Width width() const noexcept;
		void setWidth(lcsm::Width newWidth) noexcept;

		bool isError() const noexcept;
		bool isOK() const noexcept;

		LogisimBit bit(std::size_t i) const;
		void setBit(std::size_t i, LogisimBit b);

	  private:
		static constexpr std::size_t VALUE_SIZE = Width::W64;

		Width m_width;
		std::array< LogisimBit, VALUE_SIZE > m_bits;

		void resetBits() noexcept;
	};
}	 // namespace lcsm

#endif /* LCSM_VALUE_H */
