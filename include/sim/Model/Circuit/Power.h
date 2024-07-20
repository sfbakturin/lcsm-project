#ifndef SIM_MODEL_CIRCUIT_POWER_H
#define SIM_MODEL_CIRCUIT_POWER_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Width.h>

#include <cstdint>
#include <limits>

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

	  private:
		static constexpr std::uint64_t POWER_VALUE = std::numeric_limits< std::uint64_t >::max();
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_POWER_H */
