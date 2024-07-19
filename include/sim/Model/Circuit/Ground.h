#ifndef SIM_MODEL_CIRCUIT_GROUND_H
#define SIM_MODEL_CIRCUIT_GROUND_H

#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Width.h>

#include <limits>
#include <cstdint>

namespace sim
{
	class Ground : public Constant
	{
		public:
			Ground() noexcept;
			Ground(Width width) noexcept;

			Ground(const Ground &other) noexcept;
			Ground(Ground &&other) noexcept;

			Ground &operator=(const Ground &other) noexcept;
			Ground &operator=(Ground &&other) noexcept;

			void Swap(Ground &other) noexcept;

			static constexpr std::uint64_t GROUND_VALUE = std::numeric_limits<std::uint64_t>::min();
	};
}

#endif /* SIM_MODEL_CIRCUIT_GROUND_H */
