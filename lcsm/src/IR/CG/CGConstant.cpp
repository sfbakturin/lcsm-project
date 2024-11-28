#include "lcsm/IR/Event.h"
#include "lcsm/Support/PointerView.hpp"
#include <lcsm/IR/CG.h>
#include <lcsm/IR/CGObject.h>
#include <lcsm/IR/DataBits.h>
#include <lcsm/Model/Width.h>

#include <stdexcept>

lcsm::CGConstant::CGConstant() : lcsm::CGObject(lcsm::CGNodeType::Static) {}

void lcsm::CGConstant::reset() noexcept {}

lcsm::DataBits &lcsm::CGConstant::read()
{
	return m_value;
}

const lcsm::DataBits &lcsm::CGConstant::read() const
{
	return m_value;
}

void lcsm::CGConstant::write(const lcsm::DataBits &)
{
	// Scheduler must prevent attempts to write to the Constant.
	throw std::logic_error("Attempt to write to the Constant element.");
}

void lcsm::CGConstant::write(lcsm::DataBits &&)
{
	// Scheduler must prevent attempts to write to the Constant.
	throw std::logic_error("Attempt to write to the Constant element.");
}

lcsm::model::Width lcsm::CGConstant::width() const
{
	return m_value.width();
}

bool lcsm::CGConstant::checkWidth(const lcsm::DataBits &value) const
{
	return value.width() == width();
}

lcsm::CGObjectType lcsm::CGConstant::objectType() const noexcept
{
	return lcsm::CGObjectType::Constant;
}

lcsm::CGConstant *lcsm::CGConstant::asConstant() noexcept
{
	return this;
}

const lcsm::CGConstant *lcsm::CGConstant::asConstant() const noexcept
{
	return this;
}

void lcsm::CGConstant::emplaceDataBits(lcsm::model::Width, std::uint64_t)
{
	// TODO: Do we really need this function?
}

void lcsm::CGConstant::pushBackInstruction(const lcsm::Instruction &)
{
	/* No instructions can be provided to Constant. */
	throw std::logic_error("Attempt to instant instruction for Constant");
}

void lcsm::CGConstant::pushBackInstruction(lcsm::Instruction &&)
{
	/* No instructions can be provided to Constant. */
	throw std::logic_error("Attempt to instant instruction for Constant");
}

std::vector< lcsm::Event > lcsm::CGConstant::invokeInstructions()
{
	/* No instructions can be provided to input Constant. */
	if (!m_instants.empty())
		throw std::logic_error("Attempt to instant instruction for Constant");

	/* Resulting events for future mini-steps. */
	std::vector< lcsm::Event > events;

	/* Target from this object. */
	const lcsm::support::PointerView< lcsm::CGNode > targetFrom = static_cast< lcsm::CGObject * >(this);

	for (lcsm::support::PointerView< lcsm::CGNode > &child : m_children)
	{
		lcsm::CGObject *c = static_cast< lcsm::CGObject * >(child.ptr());

		/* Only wires are available as constant's childrens. */
		if (!c->isWire())
			throw std::logic_error("Constant connected to something else");

		/* Write value to Wire. */
		lcsm::Instruction i = lcsm::CreateWriteValueInstruction(this, c);
		lcsm::Event e = lcsm::CreateInstantEvent(std::move(i), targetFrom, child);
		events.push_back(std::move(e));
	}

	return events;
}

void lcsm::CGConstant::setDataBits(const lcsm::DataBits &value)
{
	m_value = value;
}

void lcsm::CGConstant::setDataBits(lcsm::DataBits &&value) noexcept
{
	m_value = std::move(value);
}
