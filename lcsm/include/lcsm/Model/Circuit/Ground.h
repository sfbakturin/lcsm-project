#ifndef LCSM_MODEL_CIRCUIT_GROUND_H
#define LCSM_MODEL_CIRCUIT_GROUND_H

#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Model/Circuit/Constant.h>
#include <lcsm/Model/Width.h>

#include <cstdint>
#include <limits>

namespace lcsm
{
	namespace model
	{
		class Ground : public Constant
		{
		  public:
			Ground(Width width);

			Ground(const Ground &other);
			Ground(Ground &&other) noexcept;

			Ground &operator=(const Ground &other);
			Ground &operator=(Ground &&other) noexcept;

			void swap(Ground &other) noexcept;

			virtual CircuitComponentType circuitComponentType() const noexcept override final;

			virtual Ground *asGround() noexcept override final;
			virtual const Ground *asGround() const noexcept override final;

		  private:
			static constexpr std::uint64_t GROUND_VALUE = std::numeric_limits< std::uint64_t >::min();
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_CIRCUIT_GROUND_H */
