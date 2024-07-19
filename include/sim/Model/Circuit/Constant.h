#ifndef SIM_MODEL_CIRCUIT_CONSTANT_H
#define SIM_MODEL_CIRCUIT_CONSTANT_H

#include <sim/Model/Width.h>

#include <cstdint>

namespace sim
{
	class Constant
	{
		public:
			Constant() noexcept;
			Constant(Width width) noexcept;
			Constant(Width width, std::uint64_t value) noexcept;

			Constant(const Constant &other) noexcept;
			Constant(Constant &&other) noexcept;

			Constant &operator=(const Constant &other) noexcept;
			Constant &operator=(Constant &&other) noexcept;

			Width GetWidth() const noexcept;
			std::uint64_t GetValue() const noexcept;

			void SetWidth(Width newWidth) noexcept;
			void SetValue(std::uint64_t newValue) noexcept;

			void Swap(Constant &other) noexcept;

		private:
			Width m_width;
			std::uint64_t m_value;
	};
}

#endif /* SIM_MODEL_CIRCUIT_CONSTANT_H */
