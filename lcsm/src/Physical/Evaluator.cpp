#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>

#include <utility>
#include <vector>

lcsm::Evaluator::Evaluator(lcsm::Evaluator &&other) noexcept : m_roots(std::move(other.m_roots)) {}

lcsm::Evaluator &lcsm::Evaluator::operator=(lcsm::Evaluator &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::Evaluator >(this, std::move(other));
}

void lcsm::Evaluator::swap(lcsm::Evaluator &other) noexcept
{
	std::swap(m_roots, other.m_roots);
}

void lcsm::Evaluator::addRoot(const lcsm::support::PointerView< lcsm::EvaluatorNode > &node)
{
	m_roots.push_back(node);
}

void lcsm::Evaluator::addRoot(lcsm::support::PointerView< lcsm::EvaluatorNode > &&node)
{
	m_roots.push_back(std::move(node));
}

const std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > &lcsm::Evaluator::roots() const noexcept
{
	return m_roots;
}

std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > &lcsm::Evaluator::roots() noexcept
{
	return m_roots;
}
