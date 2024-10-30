#include <lcsm/Model/Circuit/Constant.h>
#include <lcsm/Model/Circuit/Power.h>
#include <lcsm/Model/Width.h>

#include <utility>

lcsm::model::Power::Power(lcsm::model::Width width) : lcsm::model::Constant(width, lcsm::model::Power::POWER_VALUE) {}

lcsm::model::Power::Power(const lcsm::model::Power &other) : lcsm::model::Constant(other) {}

lcsm::model::Power::Power(lcsm::model::Power &&other) noexcept : lcsm::model::Constant(std::move(other)) {}

lcsm::model::Power &lcsm::model::Power::operator=(const lcsm::model::Power &other)
{
	if (this != &other)
		lcsm::model::Power(other).swap(*this);
	return *this;
}

lcsm::model::Power &lcsm::model::Power::operator=(lcsm::model::Power &&other) noexcept
{
	if (this != &other)
		lcsm::model::Power(std::move(other)).swap(*this);
	return *this;
}

void lcsm::model::Power::swap(lcsm::model::Power &other) noexcept
{
	lcsm::model::Constant &left = *this;
	left.swap(other);
}

lcsm::CircuitComponentType lcsm::model::Power::circuitComponentType() const noexcept
{
	return lcsm::CircuitComponentType::CIRCUIT_COMP_POWER;
}

lcsm::model::Power *lcsm::model::Power::asPower() noexcept
{
	return this;
}

const lcsm::model::Power *lcsm::model::Power::asPower() const noexcept
{
	return this;
}
