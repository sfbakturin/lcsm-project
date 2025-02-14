#ifndef LCSM_PHYSICAL_DATABITS_H
#define LCSM_PHYSICAL_DATABITS_H

#include <initializer_list>
#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Support/StaticArray.hpp>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>
#include <lcsm/Verilog/Value.h>

#include <cstddef>
#include <ostream>

namespace lcsm
{
	class DataBits
	{
	  public:
		DataBits() noexcept;

		DataBits(Width width) noexcept;
		DataBits(Width width, const verilog::Value &value) noexcept;
		DataBits(Width width, verilog::Value &&value) noexcept;
		DataBits(Width width, verilog::Bit bit) noexcept;
		DataBits(Width width, verilog::Strength strength, verilog::Bit bit) noexcept;

		DataBits(std::initializer_list< verilog::Bit > bits);
		DataBits(std::initializer_list< verilog::Value > values);

		DataBits(const DataBits &other) noexcept;
		DataBits(DataBits &&other) noexcept;

		DataBits &operator=(const DataBits &other) noexcept;
		DataBits &operator=(DataBits &&other) noexcept;

		DataBits operator|(const DataBits &other);
		DataBits &operator|=(const DataBits &other);

		bool operator==(const DataBits &other) const noexcept;
		bool operator!=(const DataBits &other) const noexcept;

		void swap(DataBits &other) noexcept;

		static DataBits fromModel(Width width, value_t value) noexcept;

		Width width() const noexcept;
		void setWidth(Width newWidth) noexcept;
		bool checkWidth(const DataBits &other) const noexcept;

		verilog::Bit bit(std::size_t index) const;

		verilog::Value &value(std::size_t index);
		const verilog::Value &value(std::size_t index) const;

		verilog::Value &operator[](std::size_t index);
		const verilog::Value &operator[](std::size_t index) const;

		void setBit(std::size_t index, verilog::Bit newBit);

		void setValue(std::size_t index, const verilog::Value &newValue);
		void setValue(std::size_t index, verilog::Value &&newValue);

		void reset() noexcept;

		friend std::ostream &operator<<(std::ostream &os, const DataBits &db);

		static constexpr std::size_t MaxWidth = Width::LastWidth;

	  private:
		Width m_width;
		support::StaticArray< verilog::Value, MaxWidth > m_bits;
	};
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_DATABITS_H */
