#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/Algorithm.hpp>

#include <map>
#include <memory>
#include <string>
#include <utility>

lcsm::LCSMCircuitView::LCSMCircuitView() noexcept : lcsm::LCSMCircuitView(nullptr) {}

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

lcsm::Identifier lcsm::LCSMCircuitView::globalId() const noexcept
{
	return m_circuit->globalId();
}

const std::string &lcsm::LCSMCircuitView::name() const noexcept
{
	return m_circuit->name();
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuitView::components() const noexcept
{
	return m_circuit->components();
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuitView::inputs() const noexcept
{
	return m_circuit->inputs();
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuitView::outputs() const noexcept
{
	return m_circuit->outputs();
}

lcsm::Circuit *lcsm::LCSMCircuitView::find(lcsm::Circuit *circuit) noexcept
{
	return m_circuit->find(circuit);
}

lcsm::Circuit *lcsm::LCSMCircuitView::find(lcsm::Identifier id) noexcept
{
	return m_circuit->find(id);
}

lcsm::Circuit *lcsm::LCSMCircuitView::find(lcsm::label_t name) noexcept
{
	return m_circuit->find(name);
}

lcsm::Circuit *lcsm::LCSMCircuitView::find(const std::string &name) noexcept
{
	return m_circuit->find(name);
}

lcsm::Circuit *lcsm::LCSMCircuitView::findInput(lcsm::Circuit *circuit) noexcept
{
	return m_circuit->findInput(circuit);
}

lcsm::Circuit *lcsm::LCSMCircuitView::findInput(lcsm::Identifier id) noexcept
{
	return m_circuit->findInput(id);
}

lcsm::Circuit *lcsm::LCSMCircuitView::findInput(lcsm::label_t name) noexcept
{
	return m_circuit->findInput(name);
}

lcsm::Circuit *lcsm::LCSMCircuitView::findInput(const std::string &name) noexcept
{
	return m_circuit->findInput(name);
}

lcsm::Circuit *lcsm::LCSMCircuitView::findOutput(lcsm::Circuit *circuit) noexcept
{
	return m_circuit->findOutput(circuit);
}

lcsm::Circuit *lcsm::LCSMCircuitView::findOutput(lcsm::Identifier id) noexcept
{
	return m_circuit->findOutput(id);
}

lcsm::Circuit *lcsm::LCSMCircuitView::findOutput(lcsm::label_t name) noexcept
{
	return m_circuit->findOutput(name);
}

lcsm::Circuit *lcsm::LCSMCircuitView::findOutput(const std::string &name) noexcept
{
	return m_circuit->findOutput(name);
}
