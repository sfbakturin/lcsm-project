#ifndef SIM_MODEL_CIRCUIT_GROUND_H
#define SIM_MODEL_CIRCUIT_GROUND_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Width.h>

#include <cstdint>
#include <limits>

namespace sim
{
	class Ground : public Constant
	{
	  public:
		Ground(Width width = Width::W1);

		Ground(const Ground &other);
		Ground(Ground &&other) noexcept;

		Ground &operator=(const Ground &other);
		Ground &operator=(Ground &&other) noexcept;

		void Swap(Ground &other) noexcept;

	  private:
		static constexpr std::uint64_t GROUND_VALUE = std::numeric_limits< std::uint64_t >::min();
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_GROUND_H */
