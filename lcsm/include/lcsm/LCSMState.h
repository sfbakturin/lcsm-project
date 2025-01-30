#ifndef LCSM_LCSMSTATE_H
#define LCSM_LCSMSTATE_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>

#include <deque>
#include <map>
#include <vector>

namespace lcsm
{
	class LCSMEngine;

	class LCSMState
	{
	  public:
		LCSMState(const LCSMState &other);
		LCSMState(LCSMState &&other) noexcept;

		LCSMState &operator=(const LCSMState &other);
		LCSMState &operator=(LCSMState &&other) noexcept;

		void swap(LCSMState &other) noexcept;

		const DataBits &valueOf(Identifier identifier) const;

		void putValue(Identifier identifier, const DataBits &databits);
		void putValue(Identifier identifier, DataBits &&databits);

		void step(unsigned n);
		void stepOnce();

	  private:
		friend class LCSMEngine;

		Timestamp m_globalTimer;
		std::map< Timestamp, std::deque< Event > > m_scheduled;
		std::unordered_map< Identifier, Context > m_values;
		std::vector< support::PointerView< EvaluatorNode > > m_roots;

		LCSMState(std::unordered_map< Identifier, std::shared_ptr< EvaluatorNode > > &objects);

		void schedule(support::PointerView< EvaluatorNode > &node);
		void scheduleAt(support::PointerView< EvaluatorNode > &node, Timestamp timer);

		void scheduleEvent(const Event &event, bool isFast = false);
		void scheduleEvent(const Event &event, bool isFast, Timestamp timer);

		void mainLoop(std::vector< support::PointerView< EvaluatorNode > > &nodes, bool isFast = false);
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMSTATE_H */
