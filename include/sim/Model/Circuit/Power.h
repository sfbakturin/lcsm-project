#ifndef SIM_MODEL_CIRCUIT_POWER_H
#define SIM_MODEL_CIRCUIT_POWER_H

#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Width.h>

#include <limits>
#include <cstdint>

namespace sim
{
	class Power : public Constant
	{
		public:
			Power() noexcept;
			Power(Width width) noexcept;

			Power(const Power &other) noexcept;
			Power(Power &&other) noexcept;

			Power &operator=(const Power &other) noexcept;
			Power &operator=(Power &&other) noexcept;

			void Swap(Power &other) noexcept;

			static constexpr std::uint64_t POWER_VALUE = std::numeric_limits<std::uint64_t>::max();
	};
}

#endif /* SIM_MODEL_CIRCUIT_POWER_H */
