#include <lcsm/Component/Component.h>
#include <lcsm/Component/IOComponent.h>

lcsm::ComponentType lcsm::IOComponent::componentType() const noexcept
{
	return lcsm::ComponentType::COMP_IO;
}

lcsm::IOComponent *lcsm::IOComponent::asIO() noexcept
{
	return this;
}

const lcsm::IOComponent *lcsm::IOComponent::asIO() const noexcept
{
	return this;
}

bool lcsm::IOComponent::isButton() const noexcept
{
	return ioComponentType() == lcsm::IOComponentType::IO_COMP_BUTTON;
}

bool lcsm::IOComponent::isDigit() const noexcept
{
	return ioComponentType() == lcsm::IOComponentType::IO_COMP_DIGIT;
}

bool lcsm::IOComponent::isProbe() const noexcept
{
	return ioComponentType() == lcsm::IOComponentType::IO_COMP_PROBE;
}

lcsm::model::Button *lcsm::IOComponent::asButton() noexcept
{
	return nullptr;
}

const lcsm::model::Button *lcsm::IOComponent::asButton() const noexcept
{
	return nullptr;
}

lcsm::model::Digit *lcsm::IOComponent::asDigit() noexcept
{
	return nullptr;
}

const lcsm::model::Digit *lcsm::IOComponent::asDigit() const noexcept
{
	return nullptr;
}

lcsm::model::Probe *lcsm::IOComponent::asProbe() noexcept
{
	return nullptr;
}

const lcsm::model::Probe *lcsm::IOComponent::asProbe() const noexcept
{
	return nullptr;
}
