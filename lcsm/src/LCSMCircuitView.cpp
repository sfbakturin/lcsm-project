#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/Algorithm.hpp>

#include <map>
#include <memory>
#include <string>
#include <utility>

lcsm::LCSMCircuitView::LCSMCircuitView() noexcept : m_circuit(nullptr) {}

lcsm::LCSMCircuitView::LCSMCircuitView(lcsm::LCSMCircuit *circuit) noexcept : m_circuit(circuit) {}

lcsm::LCSMCircuitView::LCSMCircuitView(const lcsm::LCSMCircuitView &other) noexcept : m_circuit(other.m_circuit) {}

lcsm::LCSMCircuitView::LCSMCircuitView(lcsm::LCSMCircuitView &&other) noexcept : m_circuit(other.m_circuit) {}

lcsm::LCSMCircuitView &lcsm::LCSMCircuitView::operator=(const lcsm::LCSMCircuitView &other) noexcept
{
	return lcsm::support::CopyAssign< lcsm::LCSMCircuitView >(this, other);
}

lcsm::LCSMCircuitView &lcsm::LCSMCircuitView::operator=(lcsm::LCSMCircuitView &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::LCSMCircuitView >(this, std::move(other));
}

void lcsm::LCSMCircuitView::swap(lcsm::LCSMCircuitView &other) noexcept
{
	std::swap(m_circuit, other.m_circuit);
}

bool lcsm::LCSMCircuitView::present() const noexcept
{
	return m_circuit != nullptr;
}

const lcsm::LCSMCircuit *lcsm::LCSMCircuitView::get() const noexcept
{
	return m_circuit;
}

lcsm::Identifier lcsm::LCSMCircuitView::id() const noexcept
{
	return m_circuit->id();
}

const std::string &lcsm::LCSMCircuitView::name() const noexcept
{
	return m_circuit->name();
}

void lcsm::LCSMCircuitView::setName(lcsm::label_t name)
{
	m_circuit->setName(name);
}

void lcsm::LCSMCircuitView::setName(const std::string &name)
{
	m_circuit->setName(name);
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Component > > &lcsm::LCSMCircuitView::components() const noexcept
{
	return m_circuit->components();
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Component > > &lcsm::LCSMCircuitView::inputs() const noexcept
{
	return m_circuit->inputs();
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Component > > &lcsm::LCSMCircuitView::outputs() const noexcept
{
	return m_circuit->outputs();
}

lcsm::Component *lcsm::LCSMCircuitView::find(lcsm::Component *circuit) noexcept
{
	return m_circuit->find(circuit);
}

lcsm::Component *lcsm::LCSMCircuitView::find(lcsm::Identifier id) noexcept
{
	return m_circuit->find(id);
}

lcsm::Component *lcsm::LCSMCircuitView::find(lcsm::label_t name) noexcept
{
	return m_circuit->find(name);
}

lcsm::Component *lcsm::LCSMCircuitView::find(const std::string &name) noexcept
{
	return m_circuit->find(name);
}

lcsm::Component *lcsm::LCSMCircuitView::findInput(lcsm::Component *circuit) noexcept
{
	return m_circuit->findInput(circuit);
}

lcsm::Component *lcsm::LCSMCircuitView::findInput(lcsm::Identifier id) noexcept
{
	return m_circuit->findInput(id);
}

lcsm::Component *lcsm::LCSMCircuitView::findInput(lcsm::label_t name) noexcept
{
	return m_circuit->findInput(name);
}

lcsm::Component *lcsm::LCSMCircuitView::findInput(const std::string &name) noexcept
{
	return m_circuit->findInput(name);
}

lcsm::Component *lcsm::LCSMCircuitView::findOutput(lcsm::Component *circuit) noexcept
{
	return m_circuit->findOutput(circuit);
}

lcsm::Component *lcsm::LCSMCircuitView::findOutput(lcsm::Identifier id) noexcept
{
	return m_circuit->findOutput(id);
}

lcsm::Component *lcsm::LCSMCircuitView::findOutput(lcsm::label_t name) noexcept
{
	return m_circuit->findOutput(name);
}

lcsm::Component *lcsm::LCSMCircuitView::findOutput(const std::string &name) noexcept
{
	return m_circuit->findOutput(name);
}
