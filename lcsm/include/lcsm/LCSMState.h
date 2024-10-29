#ifndef LCSM_LCSMSTATE_H
#define LCSM_LCSMSTATE_H

#include <lcsm/Component/Identifier.h>
#include <lcsm/Component/Listener.h>
#include <lcsm/IR/CG.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>

#include <deque>

namespace lcsm
{
	class LCSMState
	{
	  public:
		LCSMState(std::unordered_map< Identifier, std::shared_ptr< CGObject > > &objects);
		~LCSMState() noexcept = default;

		LCSMState(const LCSMState &other) = delete;
		LCSMState(LCSMState &&other) noexcept;

		LCSMState &operator=(const LCSMState &other) = delete;
		LCSMState &operator=(LCSMState &&other) noexcept;

		void swap(LCSMState &other) noexcept;

		void schedule(const CGNodeView &node);
		void schedule(CGNodeView &&node);

		bool step();

	  private:
		unsigned m_globalTimer;
		std::unordered_map< Identifier, std::shared_ptr< CGObject > > &m_objects;
		std::unordered_map< unsigned, std::deque< support::PointerView< CGNode > > > m_scheduled;
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMSTATE_H */
