#include <lcsm/Model/Identifier.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Snapshot.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::Snapshot::Snapshot(const lcsm::Snapshot &other) : m_contexts(other.m_contexts) {}

lcsm::Snapshot::Snapshot(lcsm::Snapshot &&other) noexcept : m_contexts(std::move(other.m_contexts)) {}

lcsm::Snapshot &lcsm::Snapshot::operator=(const lcsm::Snapshot &other)
{
	return lcsm::support::CopyAssign< lcsm::Snapshot >(this, other);
}

lcsm::Snapshot &lcsm::Snapshot::operator=(lcsm::Snapshot &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::Snapshot >(this, std::move(other));
}

void lcsm::Snapshot::swap(lcsm::Snapshot &other) noexcept
{
	std::swap(m_contexts, other.m_contexts);
}

using ComparatorTy = std::function< bool(const lcsm::Context &, const lcsm::Context &r) >;

static inline bool
	EqualsImpl(const std::unordered_map< lcsm::Identifier, lcsm::Context > &lhs,
			   const std::unordered_map< lcsm::Identifier, lcsm::Context > &rhs,
			   const ComparatorTy &cmp)
{
	using It = std::unordered_map< lcsm::Identifier, lcsm::Context >::const_iterator;

	if (lhs.size() != rhs.size())
	{
		throw std::logic_error("Trying to compare snapshots with non equals sizes!");
	}

	for (It it = lhs.begin(); it != lhs.end(); it++)
	{
		const lcsm::Identifier id = it->first;
		const lcsm::Context &l = it->second;
		const It found = rhs.find(id);

		if (found == rhs.end())
		{
			throw std::logic_error("Identifier in other snapshot is not found!");
		}

		const lcsm::Context &r = found->second;

		if (!cmp(l, r))
		{
			return false;
		}
	}

	return true;
}

bool lcsm::Snapshot::equalsStrict(const lcsm::Snapshot &other) const
{
	static const ComparatorTy cmp = [](const lcsm::Context &lhs, const lcsm::Context &rhs)
	{
		return lhs == rhs;
	};
	return EqualsImpl(m_contexts, other.m_contexts, cmp);
}

bool lcsm::Snapshot::equalsValues(const lcsm::Snapshot &other) const
{
	static const ComparatorTy cmp = [](const lcsm::Context &lhs, const lcsm::Context &rhs)
	{
		return lhs.isEqualsValues(rhs);
	};
	return EqualsImpl(m_contexts, other.m_contexts, cmp);
}

lcsm::support::PointerView< lcsm::Context >
	lcsm::Snapshot::allocate(lcsm::Identifier id, std::size_t contextSize, std::size_t privateContextSize)
{
	m_contexts[id] = lcsm::Context(contextSize, privateContextSize);
	return { std::addressof(m_contexts[id]) };
}

lcsm::support::PointerView< lcsm::Context > lcsm::Snapshot::at(lcsm::Identifier id)
{
	return { std::addressof(m_contexts.at(id)) };
}

std::size_t lcsm::Snapshot::valueSize(lcsm::Identifier id) const
{
	return m_contexts.at(id).size();
}

const lcsm::DataBits &lcsm::Snapshot::valueOf(lcsm::Identifier id, std::size_t i) const
{
	return m_contexts.at(id).getValue(i);
}

const std::vector< lcsm::DataBits > &lcsm::Snapshot::valuesOf(lcsm::Identifier id) const
{
	return m_contexts.at(id).values();
}

void lcsm::Snapshot::clearPollution() noexcept
{
	for (std::pair< const lcsm::Identifier, lcsm::Context > &context : m_contexts)
	{
		context.second.setPolluted(false);
	}
}

bool lcsm::Snapshot::empty() const noexcept
{
	return m_contexts.empty();
}

lcsm::Snapshot::operator bool() const noexcept
{
	return !empty();
}
