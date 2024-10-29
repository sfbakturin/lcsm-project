#include <lcsm/Component/Identifier.h>
#include <lcsm/IR/CG.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>

#include <deque>
#include <utility>

lcsm::LCSMState::LCSMState(std::unordered_map< lcsm::Identifier, std::shared_ptr< CGObject > > &objects) :
	m_globalTimer(0), m_objects(objects)
{
}

lcsm::LCSMState::LCSMState(lcsm::LCSMState &&other) noexcept :
	m_objects(other.m_objects), m_scheduled(std::move(other.m_scheduled))
{
}

lcsm::LCSMState &lcsm::LCSMState::operator=(lcsm::LCSMState &&other) noexcept
{
	if (this != &other)
		lcsm::LCSMState(std::move(other)).swap(*this);
	return *this;
}

void lcsm::LCSMState::swap(lcsm::LCSMState &other) noexcept
{
	std::swap(m_globalTimer, other.m_globalTimer);
	std::swap(m_scheduled, other.m_scheduled);
	std::swap(m_objects, other.m_objects);
}

void lcsm::LCSMState::schedule(const lcsm::CGNodeView &node)
{
	m_scheduled[m_globalTimer].push_back(node);
}

void lcsm::LCSMState::schedule(lcsm::CGNodeView &&node)
{
	m_scheduled[m_globalTimer].push_back(std::move(node));
}

bool lcsm::LCSMState::step()
{
	if (m_scheduled.empty())
		return false;

	const auto found = m_scheduled.find(m_globalTimer);
	if (found == m_scheduled.cend())
	{
		m_globalTimer++;
		return true;
	}

	std::deque< lcsm::support::PointerView< lcsm::CGNode > > scheduler = std::move(found->second);
	m_scheduled.erase(found);

	do
	{
		break;
	} while (true);

	m_globalTimer++;

	return true;
}
