#include <lcsm/LCSM.h>
#include <lcsm/Model/Component.h>

#include <string>
#include <utility>

lcsm::Component::Component(lcsm::label_t name) : m_name(name) {}

void lcsm::Component::setName(const std::string &name)
{
	m_name = name;
}

void lcsm::Component::setName(std::string &&name) noexcept
{
	m_name = std::move(name);
}

void lcsm::Component::setName(lcsm::label_t name)
{
	m_name = name;
}

const std::string &lcsm::Component::name() const noexcept
{
	return m_name;
}
