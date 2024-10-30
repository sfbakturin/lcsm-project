#ifndef LCSM_MODEL_CIRCUIT_POWER_H
#define LCSM_MODEL_CIRCUIT_POWER_H

#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Model/Circuit/Constant.h>
#include <lcsm/Model/Width.h>

#include <cstdint>
#include <limits>

namespace lcsm
{
	namespace model
	{
		class Power : public Constant
		{
		  public:
			Power(Width width);

			Power(const Power &other);
			Power(Power &&other) noexcept;

			Power &operator=(const Power &other);
			Power &operator=(Power &&other) noexcept;

			void swap(Power &other) noexcept;

			virtual CircuitComponentType circuitComponentType() const noexcept override final;

			virtual Power *asPower() noexcept override final;
			virtual const Power *asPower() const noexcept override final;

		  private:
			static constexpr std::uint64_t POWER_VALUE = std::numeric_limits< std::uint64_t >::max();
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_CIRCUIT_POWER_H */
