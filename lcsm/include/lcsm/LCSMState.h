#ifndef LCSM_LCSMSTATE_H
#define LCSM_LCSMSTATE_H

#include <initializer_list>
#include <lcsm/LCSM.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Snapshot.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/lcsmconfig.h>
#include <unordered_map>

#include <deque>
#include <vector>

namespace lcsm
{
	class LCSM_API LCSMEngine;

	class LCSM_API LCSMState
	{
	  public:
		LCSMState(const LCSMState &other);
		LCSMState(LCSMState &&other) noexcept;

		LCSMState &operator=(const LCSMState &other);
		LCSMState &operator=(LCSMState &&other) noexcept;

		void swap(LCSMState &other) noexcept;

		std::size_t valueSize(Identifier id) const;
		const DataBits &valueOf(Identifier id, std::size_t i = 0) const;
		const std::vector< DataBits > &valuesOf(Identifier id) const;

		void putValue(Identifier id, const DataBits &databits, std::size_t i = 0);
		void putValue(Identifier id, std::initializer_list< DataBits > databits);

		void ticks(unsigned n);
		void tick();

	  private:
		friend class LCSMEngine;

		Timestamp m_timestamp;
		std::unordered_map< Timestamp, std::deque< Event > > m_queue;
		Snapshot m_snapshot;
		std::vector< support::PointerView< EvaluatorNode > > m_roots;
		support::PointerView< LCSMEngine > m_engine;

	  private:
		LCSMState(LCSMEngine *engine);
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMSTATE_H */
