#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Verilog/ModuleDeclare/Context.h>
#include <lcsm/Verilog/Port.h>

#include <utility>

lcsm::verilog::ModuleDeclareContext::ModuleDeclareContext(const ModuleDeclareContext &other) :
	m_moduleIdentifier(other.m_moduleIdentifier), m_modulePortDeclarations(other.m_modulePortDeclarations)
{
}

lcsm::verilog::ModuleDeclareContext::ModuleDeclareContext(ModuleDeclareContext &&other) noexcept :
	m_moduleIdentifier(std::move(other.m_moduleIdentifier)), m_modulePortDeclarations(std::move(other.m_modulePortDeclarations))
{
}

lcsm::verilog::ModuleDeclareContext &lcsm::verilog::ModuleDeclareContext::operator=(const lcsm::verilog::ModuleDeclareContext &other)
{
	return lcsm::support::CopyAssign< lcsm::verilog::ModuleDeclareContext >(this, other);
}

lcsm::verilog::ModuleDeclareContext &lcsm::verilog::ModuleDeclareContext::operator=(lcsm::verilog::ModuleDeclareContext &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::verilog::ModuleDeclareContext >(this, std::move(other));
}

void lcsm::verilog::ModuleDeclareContext::swap(lcsm::verilog::ModuleDeclareContext &other) noexcept
{
	std::swap(m_moduleIdentifier, other.m_moduleIdentifier);
	std::swap(m_modulePortDeclarations, other.m_modulePortDeclarations);
}

const std::string &lcsm::verilog::ModuleDeclareContext::moduleIdentifier() const noexcept
{
	return m_moduleIdentifier;
}

void lcsm::verilog::ModuleDeclareContext::setModuleIdentifier(const std::string &moduleIdentifier)
{
	m_moduleIdentifier = moduleIdentifier;
}

void lcsm::verilog::ModuleDeclareContext::setModuleIdentifier(std::string &&moduleIdentifier) noexcept
{
	m_moduleIdentifier = std::move(moduleIdentifier);
}

void lcsm::verilog::ModuleDeclareContext::addPortDeclaration(lcsm::verilog::PortType portType, const std::vector< std::string > &identifiers)
{
	m_modulePortDeclarations.emplace_back(portType, identifiers);
}

void lcsm::verilog::ModuleDeclareContext::addPortDeclaration(lcsm::verilog::PortType portType, std::vector< std::string > &&identifiers)
{
	m_modulePortDeclarations.emplace_back(portType, std::move(identifiers));
}

const std::vector< std::pair< lcsm::verilog::PortType, std::vector< std::string > > > &
	lcsm::verilog::ModuleDeclareContext::modulePortDeclarations() const noexcept
{
	return m_modulePortDeclarations;
}

std::vector< std::pair< lcsm::verilog::PortType, std::vector< std::string > > > &
	lcsm::verilog::ModuleDeclareContext::modulePortDeclarations() noexcept
{
	return m_modulePortDeclarations;
}
