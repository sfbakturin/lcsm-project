#ifndef SIM_SIMSTATE_H
#define SIM_SIMSTATE_H

#include <sim/Component/Identifier.h>
#include <sim/Component/Listener.h>
#include <sim/IR/CG.h>
#include <sim/Support/PointerView.hpp>
#include <unordered_map>

#include <deque>

namespace sim
{
	class SimState
	{
	  public:
		SimState(std::unordered_map< Identifier, std::shared_ptr< CGObject > > &objects);
		~SimState() noexcept = default;

		SimState(const SimState &other) = delete;
		SimState(SimState &&other) noexcept;

		SimState &operator=(const SimState &other) = delete;
		SimState &operator=(SimState &&other) noexcept;

		void swap(SimState &other) noexcept;

		void schedule(const CGNodeView &node);
		void schedule(CGNodeView &&node);

		bool step();

	  private:
		unsigned m_globalTimer;
		std::unordered_map< Identifier, std::shared_ptr< CGObject > > &m_objects;
		std::unordered_map< unsigned, std::deque< support::PointerView< CGNode > > > m_scheduled;
	};
}	 // namespace sim

#endif /* SIM_SIMSTATE_H */
