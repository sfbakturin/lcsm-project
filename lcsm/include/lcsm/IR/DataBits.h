#ifndef LCSM_IR_DATABITS_H
#define LCSM_IR_DATABITS_H

#include <initializer_list>
#include <lcsm/Model/Width.h>
#include <lcsm/Support/StaticArray.hpp>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>
#include <lcsm/Verilog/Value.h>

#include <cstddef>

namespace lcsm
{
	class DataBits
	{
	  public:
		DataBits() noexcept;

		DataBits(model::Width width) noexcept;
		DataBits(model::Width width, const verilog::Value &value) noexcept;
		DataBits(model::Width width, verilog::Value &&value) noexcept;
		DataBits(model::Width width, verilog::Bit bit) noexcept;
		DataBits(model::Width width, verilog::Strength strength, verilog::Bit bit) noexcept;

		DataBits(std::initializer_list< verilog::Bit > bits);
		DataBits(std::initializer_list< verilog::Value > values);

		DataBits(const DataBits &other) noexcept;
		DataBits(DataBits &&other) noexcept;

		DataBits &operator=(const DataBits &other) noexcept;
		DataBits &operator=(DataBits &&other) noexcept;

		void swap(DataBits &other) noexcept;

		model::Width width() const noexcept;
		bool checkWidth(const DataBits &other) const noexcept;

		verilog::Bit bit(std::size_t index) const;

		verilog::Value &value(std::size_t index);
		const verilog::Value &value(std::size_t index) const;

		verilog::Value &operator[](std::size_t index);
		const verilog::Value &operator[](std::size_t index) const;

		void setBit(std::size_t index, verilog::Bit newBit);

		void setValue(std::size_t index, const verilog::Value &newValue);
		void setValue(std::size_t index, verilog::Value &&newValue);

		static constexpr std::size_t MaxWidth = model::Width::MaxWidth;

	  private:
		model::Width m_width;
		support::StaticArray< verilog::Value, MaxWidth > m_bits;
	};
}	 // namespace lcsm

#endif /* LCSM_IR_DATABITS_H */
