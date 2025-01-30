#ifndef LCSM_MODEL_CIRCUIT_H
#define LCSM_MODEL_CIRCUIT_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>

namespace lcsm
{
	using portid_t = std::size_t;

	class Circuit
	{
	  public:
		Circuit() noexcept = default;
		virtual ~Circuit() noexcept = default;

		virtual Identifier id() const noexcept = 0;
		virtual Identifier identify(Identifier id) noexcept = 0;

		virtual ObjectType objectType() const noexcept = 0;
		virtual CircuitType circuitType() const noexcept = 0;

		virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) = 0;
	};
}	 // namespace lcsm

#endif /* LCSM_MODEL_CIRCUIT_H */
