#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Verilog.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Module.h>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

static lcsm::Circuit *ByPortImpl(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires, std::size_t &p) noexcept
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
	lcsm::Circuit(name), m_module(module)
{
}

lcsm::model::VerilogModule::~VerilogModule() noexcept
{
	disconnectAll();
}

const lcsm::model::Wire *lcsm::model::VerilogModule::wire(lcsm::portid_t portId) const noexcept
{
	lcsm::Circuit *found = nullptr;
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

	// Check, if port is output reg.
	found = ByPortImpl(m_outputRegs, p);
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

const lcsm::model::Wire *lcsm::model::VerilogModule::outputReg(lcsm::portid_t portId) const noexcept
{
	if (static_cast< std::size_t >(portId) < m_outputRegs.size())
	{
		return m_outputRegs[portId].get();
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

lcsm::portid_t lcsm::model::VerilogModule::indexOfOutputReg(lcsm::portid_t portId) const noexcept
{
	return numOfInputs() + numOfInouts() + numOfOutputs() + portId;
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

lcsm::portid_t lcsm::model::VerilogModule::numOfOutputRegs() const noexcept
{
	return static_cast< lcsm::portid_t >(m_outputRegs.size());
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
	const std::size_t nOutputReg = m_module->outputRegPorts().size();
	return nInput + nInout + nOutput + nOutputReg;
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
	const std::size_t nOutputReg = m_module->outputRegPorts().size();

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
	for (std::size_t i = 0; i < nOutputReg; i++)
	{
		m_outputRegs.push_back(wires[j++]);
		m_outputRegs[i]->connectConnect(this);
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

	for (std::size_t i = 0; i < m_outputRegs.size(); i++)
	{
		next = m_outputRegs[i]->identify(next.next());
	}

	return next;
}

lcsm::object_type_t lcsm::model::VerilogModule::objectType() const noexcept
{
	return lcsm::ObjectType::Internal;
}

lcsm::CircuitType lcsm::model::VerilogModule::circuitType() const noexcept
{
	return lcsm::CircuitType::VerilogModule;
}

void lcsm::model::VerilogModule::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *selected = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (selected == nullptr)
	{
		throw std::logic_error("Bad port!");
	}
	selected->connectToWire(circuit);
}

void lcsm::model::VerilogModule::disconnect(lcsm::Circuit *) noexcept
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

	for (std::shared_ptr< lcsm::model::Wire > &outputReg : m_outputRegs)
	{
		outputReg->disconnectAll();
	}

	m_inputs.clear();
	m_inouts.clear();
	m_outputs.clear();
	m_outputRegs.clear();
}

lcsm::Circuit *lcsm::model::VerilogModule::byPort(lcsm::portid_t portId) noexcept
{
	lcsm::Circuit *found = nullptr;
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

	// Check, if port is output reg.
	found = ByPortImpl(m_outputRegs, p);
	if (found != nullptr)
	{
		return found;
	}

	return nullptr;
}

static inline bool findPortImpl(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires, const lcsm::Circuit *circuit)
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

lcsm::portid_t lcsm::model::VerilogModule::findPort(const lcsm::Circuit *circuit) const noexcept
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
	else if (findPortImpl(m_outputRegs, circuit))
	{
		return lcsm::model::VerilogModule::Port::OutputReg;
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
