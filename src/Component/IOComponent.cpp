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

bool sim::IOComponent::isButton() const noexcept
{
	return ioComponentType() == sim::IOComponentType::IO_COMP_BUTTON;
}

bool sim::IOComponent::isDigit() const noexcept
{
	return ioComponentType() == sim::IOComponentType::IO_COMP_DIGIT;
}

bool sim::IOComponent::isProbe() const noexcept
{
	return ioComponentType() == sim::IOComponentType::IO_COMP_PROBE;
}

sim::Button *sim::IOComponent::asButton() noexcept
{
	return nullptr;
}

const sim::Button *sim::IOComponent::asButton() const noexcept
{
	return nullptr;
}

sim::Digit *sim::IOComponent::asDigit() noexcept
{
	return nullptr;
}

const sim::Digit *sim::IOComponent::asDigit() const noexcept
{
	return nullptr;
}

sim::Probe *sim::IOComponent::asProbe() noexcept
{
	return nullptr;
}

const sim::Probe *sim::IOComponent::asProbe() const noexcept
{
	return nullptr;
}
