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
#include <string>
#include <utility>

namespace lcsm
{
	class DataBits
	{
	  public:
		DataBits() noexcept;

		DataBits(Width width) noexcept;
		DataBits(Width width, verilog::Value value) noexcept;
		DataBits(Width width, verilog::Bit bit) noexcept;
		DataBits(Width width, verilog::Strength strength, verilog::Bit bit) noexcept;

		DataBits(width_t width) noexcept;
		DataBits(width_t width, verilog::Value value) noexcept;
		DataBits(width_t width, verilog::Bit bit) noexcept;
		DataBits(width_t width, verilog::Strength strength, verilog::Bit bit) noexcept;

		DataBits(std::initializer_list< verilog::Bit > bits) noexcept;
		DataBits(std::initializer_list< std::pair< verilog::Strength, verilog::Bit > > strengthBits) noexcept;
		DataBits(std::initializer_list< verilog::Value > values) noexcept;

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
		static DataBits fromModel(value_t value) noexcept;

		width_t width() const noexcept;
		void setWidth(width_t width) noexcept;
		bool checkWidth(const DataBits &other) const noexcept;

		verilog::Bit bit(std::size_t index) const;

		verilog::Value &value(std::size_t index);
		const verilog::Value &value(std::size_t index) const;

		verilog::Value &operator[](std::size_t index);
		const verilog::Value &operator[](std::size_t index) const;

		void setBit(std::size_t index, verilog::Bit bit);
		void setValue(std::size_t index, verilog::Value value);
		void set(std::size_t index, verilog::Bit bit, verilog::Strength strength);

		DataBits subdatabits(std::size_t begin) const noexcept;
		DataBits subdatabits(std::size_t begin, std::size_t end) const noexcept;

		void reset() noexcept;

		friend std::ostream &operator<<(std::ostream &os, const DataBits &db);

		std::string toString() const;

		static constexpr lcsm::width_t MaxWidth = Width::LastWidth;

	  private:
		width_t m_width;
		support::StaticArray< verilog::Value, MaxWidth > m_bits;
	};
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_DATABITS_H */
