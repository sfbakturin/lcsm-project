#include <lcsm/IR/CGObject.h>
#include <lcsm/Support/PointerView.hpp>

#include <utility>

lcsm::support::PointerView< lcsm::CGState > &lcsm::CGCollector::state() noexcept
{
	return m_state;
}

const lcsm::support::PointerView< lcsm::CGState > &lcsm::CGCollector::state() const noexcept
{
	return m_state;
}

void lcsm::CGCollector::setState(const support::PointerView< CGState > &newState) noexcept
{
	m_state = newState;
}

void lcsm::CGCollector::setState(support::PointerView< CGState > &&newState) noexcept
{
	m_state = std::move(newState);
}
