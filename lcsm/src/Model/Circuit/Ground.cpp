#include <lcsm/Model/Circuit/Constant.h>
#include <lcsm/Model/Circuit/Ground.h>
#include <lcsm/Model/Width.h>

#include <utility>

lcsm::model::Ground::Ground(lcsm::model::Width width) : lcsm::model::Constant(width, lcsm::model::Ground::GROUND_VALUE)
{
}

lcsm::model::Ground::Ground(const lcsm::model::Ground &other) : lcsm::model::Constant(other) {}
lcsm::model::Ground::Ground(lcsm::model::Ground &&other) noexcept : lcsm::model::Constant(std::move(other)) {}

lcsm::model::Ground &lcsm::model::Ground::operator=(const lcsm::model::Ground &other)
{
	if (this != &other)
		lcsm::model::Ground(other).swap(*this);
	return *this;
}
lcsm::model::Ground &lcsm::model::Ground::operator=(lcsm::model::Ground &&other) noexcept
{
	if (this != &other)
		lcsm::model::Ground(std::move(other)).swap(*this);
	return *this;
}

void lcsm::model::Ground::swap(lcsm::model::Ground &other) noexcept
{
	lcsm::model::Constant &left = *this;
	left.swap(other);
}

lcsm::CircuitComponentType lcsm::model::Ground::circuitComponentType() const noexcept
{
	return lcsm::CircuitComponentType::CIRCUIT_COMP_GROUND;
}

lcsm::model::Ground *lcsm::model::Ground::asGround() noexcept
{
	return this;
}

const lcsm::model::Ground *lcsm::model::Ground::asGround() const noexcept
{
	return this;
}
