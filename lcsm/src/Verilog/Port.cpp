#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Verilog/Port.h>

#include <string>
#include <utility>

lcsm::verilog::Port::Port(const lcsm::verilog::PortType &portType) : m_portType(portType) {}

lcsm::verilog::Port::Port(lcsm::verilog::PortType &&portType) : m_portType(portType) {}

lcsm::verilog::Port::Port(const lcsm::verilog::PortType &portType, const std::string &identifier) :
	m_portType(portType), m_identifier(identifier)
{
}

lcsm::verilog::Port::Port(const lcsm::verilog::PortType &portType, std::string &&identifier) noexcept :
	m_portType(portType), m_identifier(std::move(identifier))
{
}

lcsm::verilog::Port::Port(lcsm::verilog::PortType &&portType, const std::string &identifier) :
	m_portType(std::move(portType)), m_identifier(identifier)
{
}

lcsm::verilog::Port::Port(lcsm::verilog::PortType &&portType, std::string &&identifier) noexcept :
	m_portType(std::move(portType)), m_identifier(std::move(identifier))
{
}

lcsm::verilog::Port::Port(const Port &other) : m_portType(other.m_portType), m_identifier(other.m_identifier) {}

lcsm::verilog::Port::Port(Port &&other) noexcept :
	m_portType(std::move(other.portType())), m_identifier(std::move(other.m_identifier))
{
}

lcsm::verilog::Port &lcsm::verilog::Port::operator=(const Port &other)
{
	return lcsm::support::CopyAssign< lcsm::verilog::Port >(this, other);
}

lcsm::verilog::Port &lcsm::verilog::Port::operator=(Port &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::verilog::Port >(this, std::move(other));
}

void lcsm::verilog::Port::swap(lcsm::verilog::Port &other) noexcept
{
	std::swap(m_portType, other.m_portType);
	std::swap(m_identifier, other.m_identifier);
}

const lcsm::verilog::PortType &lcsm::verilog::Port::portType() const noexcept
{
	return m_portType;
}

const std::string &lcsm::verilog::Port::identifier() const noexcept
{
	return m_identifier;
}

void lcsm::verilog::Port::setIdentifier(const std::string &identifier)
{
	m_identifier = identifier;
}

void lcsm::verilog::Port::setIdentifier(std::string &&identifier) noexcept
{
	m_identifier = std::move(identifier);
}
