#ifndef LCSM_LCSMCIRCUIT_H
#define LCSM_LCSMCIRCUIT_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>

#include <memory>

namespace lcsm
{
	class LCSMBuilder;

	class LCSMCircuit
	{
	  public:
		LCSMCircuit() = default;

		LCSMCircuit(const LCSMCircuit &other);
		LCSMCircuit(LCSMCircuit &&other) noexcept;

		LCSMCircuit &operator=(const LCSMCircuit &other);
		LCSMCircuit &operator=(LCSMCircuit &&other) noexcept;

		void swap(LCSMCircuit &other) noexcept;

		const std::unordered_map< Identifier, std::shared_ptr< Circuit > > &components() const noexcept;

	  private:
		friend class LCSMBuilder;

		Identifier m_globalId;

		std::unordered_map< Identifier, std::shared_ptr< Circuit > > m_components;

	  private:
		Circuit *registerElement(std::shared_ptr< Circuit > &&circuit);
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMCIRCUIT_H */
