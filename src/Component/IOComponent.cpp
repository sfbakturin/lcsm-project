#include <sim/Component/Component.h>
#include <sim/Component/IOComponent.h>

sim::ComponentType sim::IOComponent::componentType() const noexcept
{
	return sim::ComponentType::COMP_IO;
}

sim::IOComponent *sim::IOComponent::asIO() noexcept
{
	return this;
}

const sim::IOComponent *sim::IOComponent::asIO() const noexcept
{
	return this;
}
