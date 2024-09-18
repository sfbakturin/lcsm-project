#include <sim/Component/Identifier.h>
#include <sim/IR/CG.h>
#include <sim/SimState.h>
#include <sim/Support/PointerView.hpp>
#include <unordered_map>

#include <deque>
#include <utility>

sim::SimState::SimState(std::unordered_map< sim::Identifier, std::shared_ptr< CGObject > > &objects) :
	m_globalTimer(0), m_objects(objects)
{
}

sim::SimState::SimState(sim::SimState &&other) noexcept :
	m_objects(other.m_objects), m_scheduled(std::move(other.m_scheduled))
{
}

sim::SimState &sim::SimState::operator=(sim::SimState &&other) noexcept
{
	if (this != &other)
		sim::SimState(std::move(other)).swap(*this);
	return *this;
}

void sim::SimState::swap(sim::SimState &other) noexcept
{
	std::swap(m_globalTimer, other.m_globalTimer);
	std::swap(m_scheduled, other.m_scheduled);
	std::swap(m_objects, other.m_objects);
}

void sim::SimState::schedule(const sim::CGNodeView &node)
{
	m_scheduled[m_globalTimer].push_back(node);
}

void sim::SimState::schedule(sim::CGNodeView &&node)
{
	m_scheduled[m_globalTimer].push_back(std::move(node));
}

bool sim::SimState::step()
{
	if (m_scheduled.empty())
		return false;

	const auto found = m_scheduled.find(m_globalTimer);
	if (found == m_scheduled.cend())
	{
		m_globalTimer++;
		return true;
	}

	std::deque< sim::support::PointerView< sim::CGNode > > scheduler =
		std::move(found->second);
	m_scheduled.erase(found);

	do
	{
		break;
	} while (true);

	m_globalTimer++;

	return true;
}
