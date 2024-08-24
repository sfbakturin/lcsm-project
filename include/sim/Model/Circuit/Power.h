#ifndef SIM_MODEL_CIRCUIT_POWER_H
#define SIM_MODEL_CIRCUIT_POWER_H

#include <sim/Component/CircuitComponent.h>
#include <sim/IR/Width.h>
#include <sim/Model/Circuit/Constant.h>

#include <cstdint>
#include <limits>

namespace sim
{
	class Power : public Constant
	{
	  public:
		Power(Width width);

		Power(const Power &other);
		Power(Power &&other) noexcept;

		Power &operator=(const Power &other);
		Power &operator=(Power &&other) noexcept;

		void Swap(Power &other) noexcept;

		virtual CircuitComponentType circuitComponentType() const noexcept override;

	  private:
		static constexpr std::uint64_t POWER_VALUE =
			std::numeric_limits< std::uint64_t >::max();
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_POWER_H */
