#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Verilog.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Module.h>
#include <lcsm/Verilog/Port.h>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

static lcsm::Component *ByPortImpl(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires, std::size_t &p) noexcept
{
	if (wires.size() != 0)
	{
		if (p >= wires.size())
		{
			p -= wires.size();
		}
		else
		{
			return wires[p].get();
		}
	}
	return nullptr;
}

lcsm::model::VerilogModule::VerilogModule(const std::shared_ptr< lcsm::verilog::Module > &module) :
	lcsm::model::VerilogModule(module, module->identifier().c_str())
{
}

lcsm::model::VerilogModule::VerilogModule(const std::shared_ptr< lcsm::verilog::Module > &module, lcsm::label_t name) :
	lcsm::Component(name), m_module(module)
{
}

lcsm::model::VerilogModule::~VerilogModule() noexcept
{
	disconnectAll();
}

const lcsm::model::Wire *lcsm::model::VerilogModule::wire(lcsm::portid_t portId) const noexcept
{
	lcsm::Component *found = nullptr;
	std::size_t p = static_cast< std::size_t >(portId);

	// Check, if port is input.
	found = ByPortImpl(m_inputs, p);
	if (found != nullptr)
	{
		return static_cast< const lcsm::model::Wire * >(found);
	}

	// Check, if port is inout.
	found = ByPortImpl(m_inouts, p);
	if (found != nullptr)
	{
		return static_cast< const lcsm::model::Wire * >(found);
	}

	// Check, if port is output.
	found = ByPortImpl(m_outputs, p);
	if (found != nullptr)
	{
		return static_cast< const lcsm::model::Wire * >(found);
	}

	return nullptr;
}

const lcsm::model::Wire *lcsm::model::VerilogModule::input(lcsm::portid_t portId) const noexcept
{
	if (static_cast< std::size_t >(portId) < m_inputs.size())
	{
		return m_inputs[portId].get();
	}
	return nullptr;
}

const lcsm::model::Wire *lcsm::model::VerilogModule::inout(lcsm::portid_t portId) const noexcept
{
	if (static_cast< std::size_t >(portId) < m_inouts.size())
	{
		return m_inouts[portId].get();
	}
	return nullptr;
}

const lcsm::model::Wire *lcsm::model::VerilogModule::output(lcsm::portid_t portId) const noexcept
{
	if (static_cast< std::size_t >(portId) < m_outputs.size())
	{
		return m_outputs[portId].get();
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::VerilogModule::indexOfInput(lcsm::portid_t portId) const noexcept
{
	return portId;
}

lcsm::portid_t lcsm::model::VerilogModule::indexOfInout(lcsm::portid_t portId) const noexcept
{
	return numOfInputs() + portId;
}

lcsm::portid_t lcsm::model::VerilogModule::indexOfOutput(lcsm::portid_t portId) const noexcept
{
	return numOfInputs() + numOfInouts() + portId;
}

static lcsm::portid_t FindByLabel(const std::vector< lcsm::verilog::Port > &ports, lcsm::label_t label) noexcept
{
	lcsm::portid_t i = 0;
	for (const lcsm::verilog::Port &port : ports)
	{
		if (port.identifier() == label)
		{
			return i;
		}
		else
		{
			i++;
		}
	}
	return -1;
}

lcsm::portid_t lcsm::model::VerilogModule::indexOfInputByLabel(lcsm::label_t label) const noexcept
{
	return FindByLabel(m_module->inputPorts(), label);
}

lcsm::portid_t lcsm::model::VerilogModule::indexOfInoutByLabel(lcsm::label_t label) const noexcept
{
	const lcsm::portid_t d = FindByLabel(m_module->inoutPorts(), label);
	if (d >= 0)
	{
		return indexOfInout(d);
	}
	else
	{
		return -1;
	}
}

lcsm::portid_t lcsm::model::VerilogModule::indexOfOutputByLabel(lcsm::label_t label) const noexcept
{
	const lcsm::portid_t d = FindByLabel(m_module->outputPorts(), label);
	if (d >= 0)
	{
		return indexOfOutput(d);
	}
	else
	{
		return -1;
	}
}

lcsm::portid_t lcsm::model::VerilogModule::indexOfByLabel(lcsm::label_t label) const noexcept
{
	lcsm::portid_t found = indexOfInputByLabel(label);
	if (found >= 0)
	{
		return found;
	}

	found = indexOfInoutByLabel(label);
	if (found >= 0)
	{
		return found;
	}
	else
	{
		return indexOfOutputByLabel(label);
	}
}

lcsm::portid_t lcsm::model::VerilogModule::numOfInputs() const noexcept
{
	return static_cast< lcsm::portid_t >(m_inputs.size());
}

lcsm::portid_t lcsm::model::VerilogModule::numOfInouts() const noexcept
{
	return static_cast< lcsm::portid_t >(m_inouts.size());
}

lcsm::portid_t lcsm::model::VerilogModule::numOfOutputs() const noexcept
{
	return static_cast< lcsm::portid_t >(m_outputs.size());
}

const lcsm::verilog::Module *lcsm::model::VerilogModule::module() const noexcept
{
	return m_module.get();
}

std::size_t lcsm::model::VerilogModule::numOfWires() const noexcept
{
	const std::size_t nInput = m_module->inputPorts().size();
	const std::size_t nInout = m_module->inoutPorts().size();
	const std::size_t nOutput = m_module->outputPorts().size();
	return nInput + nInout + nOutput;
}

void lcsm::model::VerilogModule::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	// Check sizes.
	if (numOfWires() != wires.size())
	{
		throw std::logic_error("Bad num of provided wires!");
	}

	// Get sizes from Verilog's parsed module.
	const std::size_t nInput = m_module->inputPorts().size();
	const std::size_t nInout = m_module->inoutPorts().size();
	const std::size_t nOutput = m_module->outputPorts().size();

	// Match inouts, inouts, outputs and output regs.
	std::size_t j = 0;
	for (std::size_t i = 0; i < nInput; i++)
	{
		m_inputs.push_back(wires[j++]);
		m_inputs[i]->connectConnect(this);
	}
	for (std::size_t i = 0; i < nInout; i++)
	{
		m_inouts.push_back(wires[j++]);
		m_inouts[i]->connectConnect(this);
	}
	for (std::size_t i = 0; i < nOutput; i++)
	{
		m_outputs.push_back(wires[j++]);
		m_outputs[i]->connectConnect(this);
	}
}

lcsm::Identifier lcsm::model::VerilogModule::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::VerilogModule::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	lcsm::Identifier next = m_id;

	for (std::size_t i = 0; i < m_inputs.size(); i++)
	{
		next = m_inputs[i]->identify(next.next());
	}

	for (std::size_t i = 0; i < m_inouts.size(); i++)
	{
		next = m_inouts[i]->identify(next.next());
	}

	for (std::size_t i = 0; i < m_outputs.size(); i++)
	{
		next = m_outputs[i]->identify(next.next());
	}

	return next;
}

lcsm::object_type_t lcsm::model::VerilogModule::objectType() const noexcept
{
	return lcsm::ObjectType::Internal;
}

lcsm::ComponentType lcsm::model::VerilogModule::componentType() const noexcept
{
	return lcsm::ComponentType::VerilogModule;
}

void lcsm::model::VerilogModule::connect(lcsm::portid_t portId, lcsm::Component *circuit)
{
	lcsm::model::Wire *selected = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (selected == nullptr)
	{
		throw std::logic_error("Bad port!");
	}
	selected->connectToWire(circuit);
}

void lcsm::model::VerilogModule::disconnect(lcsm::Component *) noexcept
{
	// Do nothing.
}

void lcsm::model::VerilogModule::disconnectAll() noexcept
{
	for (std::shared_ptr< lcsm::model::Wire > &input : m_inputs)
	{
		input->disconnectAll();
	}

	for (std::shared_ptr< lcsm::model::Wire > &inout : m_inouts)
	{
		inout->disconnectAll();
	}

	for (std::shared_ptr< lcsm::model::Wire > &output : m_outputs)
	{
		output->disconnectAll();
	}

	m_inputs.clear();
	m_inouts.clear();
	m_outputs.clear();
}

lcsm::Component *lcsm::model::VerilogModule::byPort(lcsm::portid_t portId) noexcept
{
	lcsm::Component *found = nullptr;
	std::size_t p = static_cast< std::size_t >(portId);

	// Check, if port is input.
	found = ByPortImpl(m_inputs, p);
	if (found != nullptr)
	{
		return found;
	}

	// Check, if port is inout.
	found = ByPortImpl(m_inouts, p);
	if (found != nullptr)
	{
		return found;
	}

	// Check, if port is output.
	found = ByPortImpl(m_outputs, p);
	if (found != nullptr)
	{
		return found;
	}

	return nullptr;
}

static inline bool findPortImpl(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires, const lcsm::Component *circuit)
{
	for (auto it = wires.begin(); it != wires.end(); it++)
	{
		if (it->get() == circuit)
		{
			return true;
		}
	}
	return false;
}

lcsm::portid_t lcsm::model::VerilogModule::findPort(const lcsm::Component *circuit) const noexcept
{
	if (findPortImpl(m_inputs, circuit))
	{
		return lcsm::model::VerilogModule::Port::Input;
	}
	else if (findPortImpl(m_inouts, circuit))
	{
		return lcsm::model::VerilogModule::Port::Inout;
	}
	else if (findPortImpl(m_outputs, circuit))
	{
		return lcsm::model::VerilogModule::Port::Output;
	}
	else
	{
		return -1;
	}
}

lcsm::portid_t lcsm::model::VerilogModule::defaultPort() const noexcept
{
	return lcsm::model::VerilogModule::Port::Input;
}

void lcsm::model::VerilogModule::dump(lcsm::model::LCSMFileWriter &writer, lcsm::model::LCSMBuilder &builder) const
{
	// keyvalue source <STRING>;
	writer.writeKeyValueEscapedDeclaration("source", m_module->source());

	// name <STRING>;
	writer.writeNameDeclaration(name());

	for (lcsm::portid_t portId = 0; portId < numOfInputs(); portId++)
	{
		const lcsm::model::Wire *child = input(portId);
		const std::string key = "input" + std::to_string(portId);
		builder.addWires(child, true);
		// keyvalue input<id> <IDENTIFIER>;
		writer.writeKeyValueDeclaration(key.c_str(), child->id());
	}

	for (lcsm::portid_t portId = 0; portId < numOfOutputs(); portId++)
	{
		const lcsm::model::Wire *child = output(portId);
		const std::string key = "output" + std::to_string(portId);
		builder.addWires(child, true);
		// keyvalue output<id> <IDENTIFIER>;
		writer.writeKeyValueDeclaration(key.c_str(), child->id());
	}

	for (lcsm::portid_t portId = 0; portId < numOfInouts(); portId++)
	{
		const lcsm::model::Wire *child = inout(portId);
		const std::string key = "inout" + std::to_string(portId);
		builder.addWires(child, true);
		// keyvalue inout<id> <IDENTIFIER>;
		writer.writeKeyValueDeclaration(key.c_str(), child->id());
	}
}

void lcsm::model::VerilogModule::copy(lcsm::Component *circuit, lcsm::model::LCSMBuilder &builder) const
{
	if (componentType() != circuit->componentType())
	{
		throw std::logic_error("Bad circuit type!");
	}

	lcsm::model::VerilogModule *verilogModule = static_cast< lcsm::model::VerilogModule * >(circuit);
	verilogModule->setName(name());

	builder.oldToNew(id(), verilogModule->id());

	for (std::size_t i = 0; i < numOfWires(); i++)
	{
		const lcsm::portid_t portId = static_cast< lcsm::portid_t >(i);
		const lcsm::model::Wire *oldWire = wire(portId);
		const lcsm::model::Wire *newWire = verilogModule->wire(portId);
		builder.oldToNew(oldWire->id(), newWire->id());
		builder.addWires(oldWire, true);
	}
}

void lcsm::model::VerilogModule::from(lcsm::model::LCSMFileReader &reader, lcsm::model::LCSMBuilder &builder)
{
	// 'source' is already parsed, so we continue to 'endcomponent'

	// name <STRING>;
	setName(reader.exceptName());

	for (lcsm::portid_t portId = 0; portId < numOfInputs(); portId++)
	{
		const std::string key = "input" + std::to_string(portId);
		// keyvalue input<id> <IDENTIFIER>;
		builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue(key.c_str())), input(portId)->id());
	}

	for (lcsm::portid_t portId = 0; portId < numOfOutputs(); portId++)
	{
		const std::string key = "output" + std::to_string(portId);
		// keyvalue output<id> <IDENTIFIER>;
		builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue(key.c_str())), output(portId)->id());
	}

	for (lcsm::portid_t portId = 0; portId < numOfInouts(); portId++)
	{
		const std::string key = "inout" + std::to_string(portId);
		// keyvalue inout<id> <IDENTIFIER>;
		builder.oldToNew(lcsm::Identifier(reader.exceptIntegerKeyValue(key.c_str())), inout(portId)->id());
	}
}
