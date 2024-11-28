#ifndef LCSM_LCSMSTATE_H
#define LCSM_LCSMSTATE_H

#include <lcsm/Component/Identifier.h>
#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/Event.h>
#include <lcsm/LCSMC.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>

#include <deque>
#include <vector>

namespace lcsm
{
	class LCSMEngine;

	class LCSMState
	{
	  public:
		LCSMState(const LCSMState &other) = delete;
		LCSMState(LCSMState &&other) noexcept;

		LCSMState &operator=(const LCSMState &other) = delete;
		LCSMState &operator=(LCSMState &&other) noexcept;

		~LCSMState() noexcept;

		void swap(LCSMState &other) noexcept;

		void schedulePutValue(Identifier identifier, const DataBits &value);
		void schedulePutValue(Identifier identifier, DataBits &&value);

		void step(unsigned n);
		void stepOnce();

	  private:
		friend class LCSMEngine;

		timer_t m_globalTimer;
		support::PointerView< std::vector< support::PointerView< CGNode > > > m_nodesView;
		support::PointerView< LCSMEngine > m_engineView;
		std::unordered_map< timer_t, std::deque< Event > > m_scheduled;

		LCSMState(std::vector< support::PointerView< CGNode > > &nodes, LCSMEngine &engine);

		void schedule(support::PointerView< CGNode > &node);
		void scheduleAt(support::PointerView< CGNode > &node, timer_t timer);

		void scheduleEvent(const Event &event, bool isFast = false);
		void scheduleEvent(const Event &event, bool isFast, timer_t timer);

		void mainLoop(std::vector< support::PointerView< CGNode > > &nodes, bool isFast = false);
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMSTATE_H */
