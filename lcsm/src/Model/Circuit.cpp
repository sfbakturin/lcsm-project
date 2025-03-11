#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>

#include <string>
#include <utility>

lcsm::Circuit::Circuit(lcsm::label_t name) : m_name(name) {}

void lcsm::Circuit::setName(const std::string &name)
{
	m_name = name;
}

void lcsm::Circuit::setName(std::string &&name) noexcept
{
	m_name = std::move(name);
}

void lcsm::Circuit::setName(lcsm::label_t name)
{
	m_name = name;
}

const std::string &lcsm::Circuit::name() const noexcept
{
	return m_name;
}

lcsm::label_t lcsm::Circuit::c_name() const noexcept
{
	return m_name.c_str();
}
