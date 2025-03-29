#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Verilog.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Button.h>
#include <lcsm/Model/std/Clock.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Digit.h>
#include <lcsm/Model/std/Ground.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Power.h>
#include <lcsm/Model/std/Probe.h>
#include <lcsm/Model/std/Splitter.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Model/std/TransmissionGate.h>
#include <lcsm/Model/std/Tunnel.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/IO/CStringReader.h>
#include <lcsm/Support/IO/FileReader.h>
#include <lcsm/Support/IO/FileWriter.h>
#include <lcsm/Support/IO/Reader.h>
#include <lcsm/Support/IO/StringWriter.h>
#include <lcsm/Support/IO/Writer.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Module.h>
#include <unordered_map>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

lcsm::LCSMCircuit::LCSMCircuit(lcsm::label_t name) : m_name(name) {}

lcsm::LCSMCircuit::LCSMCircuit(const std::string &name) : m_name(name) {}

lcsm::LCSMCircuit::LCSMCircuit(const lcsm::LCSMCircuit &other) : m_name(other.m_name)
{
	static const lcsm::Identifier entryId;
	lcsm::model::LCSMBuilder builder;
	other.copyImpl(this, builder, entryId);
}

lcsm::LCSMCircuit::LCSMCircuit(lcsm::LCSMCircuit &&other) noexcept :
	m_globalId(std::move(other.m_globalId)), m_components(std::move(other.m_components)), m_inputs(std::move(other.m_inputs)),
	m_outputs(std::move(other.m_outputs)), m_componentWires(std::move(other.m_componentWires)),
	m_connectorWires(std::move(other.m_connectorWires)), m_circuits(std::move(other.m_circuits)),
	m_verilogModules(std::move(other.m_verilogModules)), m_name(std::move(other.m_name))
{
}

lcsm::LCSMCircuit &lcsm::LCSMCircuit::operator=(const lcsm::LCSMCircuit &other)
{
	return lcsm::support::CopyAssign< lcsm::LCSMCircuit >(this, other);
}

lcsm::LCSMCircuit &lcsm::LCSMCircuit::operator=(lcsm::LCSMCircuit &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::LCSMCircuit >(this, std::move(other));
}

void lcsm::LCSMCircuit::swap(lcsm::LCSMCircuit &other) noexcept
{
	std::swap(m_globalId, other.m_globalId);
	std::swap(m_components, other.m_components);
	std::swap(m_inputs, other.m_inputs);
	std::swap(m_outputs, other.m_outputs);
	std::swap(m_componentWires, other.m_componentWires);
	std::swap(m_connectorWires, other.m_connectorWires);
	std::swap(m_circuits, other.m_circuits);
	std::swap(m_verilogModules, other.m_verilogModules);
	std::swap(m_name, other.m_name);
}

lcsm::LCSMCircuit lcsm::LCSMCircuit::copy() const
{
	static const lcsm::Identifier entryId;
	return copyImpl(entryId);
}

lcsm::Identifier lcsm::LCSMCircuit::globalId() const noexcept
{
	return m_globalId;
}

const std::string &lcsm::LCSMCircuit::name() const noexcept
{
	return m_name;
}

void lcsm::LCSMCircuit::setName(lcsm::label_t name)
{
	m_name = name;
}

void lcsm::LCSMCircuit::setName(const std::string &name)
{
	m_name = name;
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::components() const noexcept
{
	return m_components;
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::inputs() const noexcept
{
	return m_inputs;
}

const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &lcsm::LCSMCircuit::outputs() const noexcept
{
	return m_outputs;
}

lcsm::Circuit *lcsm::LCSMCircuit::create(lcsm::Circuit *circuit)
{
	std::shared_ptr< lcsm::Circuit > element(circuit);
	return registerComponent(element);
}

lcsm::model::Constant *lcsm::LCSMCircuit::createConstant(lcsm::label_t name, lcsm::Width width, lcsm::value_t value)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Constant >(name, width, value);
	lcsm::model::Constant *circuit = static_cast< lcsm::model::Constant * >(registerComponent(element));
	return circuit;
}

lcsm::model::Ground *lcsm::LCSMCircuit::createGround(lcsm::label_t name, lcsm::Width width)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Ground >(name, width);
	lcsm::model::Ground *circuit = static_cast< lcsm::model::Ground * >(registerComponent(element));
	return circuit;
}

lcsm::model::Power *lcsm::LCSMCircuit::createPower(lcsm::label_t name, lcsm::Width width)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Power >(name, width);
	lcsm::model::Power *circuit = static_cast< lcsm::model::Power * >(registerComponent(element));
	return circuit;
}

lcsm::model::Pin *lcsm::LCSMCircuit::createPin(bool output, lcsm::label_t name, lcsm::Width width)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Pin >(name, output, width);
	lcsm::model::Pin *circuit = static_cast< lcsm::model::Pin * >(registerComponent(element));
	return circuit;
}

lcsm::model::Transistor *lcsm::LCSMCircuit::createTransistor(lcsm::label_t name, lcsm::model::Transistor::Type type)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Transistor >(name, type);
	lcsm::model::Transistor *circuit = static_cast< lcsm::model::Transistor * >(registerComponent(element));
	return circuit;
}

lcsm::model::TransmissionGate *lcsm::LCSMCircuit::createTransmissionGate(lcsm::label_t name)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::TransmissionGate >(name);
	lcsm::model::TransmissionGate *circuit = static_cast< lcsm::model::TransmissionGate * >(registerComponent(element));
	return circuit;
}

lcsm::model::Tunnel *lcsm::LCSMCircuit::createTunnel(lcsm::label_t name)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Tunnel >(name);
	lcsm::model::Tunnel *circuit = static_cast< lcsm::model::Tunnel * >(registerComponent(element));
	return circuit;
}

lcsm::model::Clock *lcsm::LCSMCircuit::createClock(lcsm::label_t name, unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Clock >(name, highDuration, lowDuration, phaseOffset);
	lcsm::model::Clock *circuit = static_cast< lcsm::model::Clock * >(registerComponent(element));
	return circuit;
}

lcsm::model::Button *lcsm::LCSMCircuit::createButton(lcsm::label_t name, bool activeOnPress)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Button >(name, activeOnPress);
	lcsm::model::Button *circuit = static_cast< lcsm::model::Button * >(registerComponent(element));
	return circuit;
}

lcsm::model::Digit *lcsm::LCSMCircuit::createDigit(lcsm::label_t name, bool hasDecimalPoint)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Digit >(name, hasDecimalPoint);
	lcsm::model::Digit *circuit = static_cast< lcsm::model::Digit * >(registerComponent(element));
	return circuit;
}

lcsm::model::Probe *lcsm::LCSMCircuit::createProbe(lcsm::label_t name)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Probe >(name);
	lcsm::model::Probe *circuit = static_cast< lcsm::model::Probe * >(registerComponent(element));
	return circuit;
}

lcsm::model::Splitter *lcsm::LCSMCircuit::createSplitter(lcsm::label_t name, lcsm::Width widthIn, lcsm::width_t widthOut)
{
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::Splitter >(name, widthIn, widthOut);
	lcsm::model::Splitter *circuit = static_cast< lcsm::model::Splitter * >(registerComponent(element));
	return circuit;
}

lcsm::model::VerilogModule *lcsm::LCSMCircuit::createVerilogModule(const lcsm::verilog::Module &module)
{
	// Indexes.
	const lcsm::Identifier moduleId = m_globalId;

	// Create a copy of Verilog's abstract module to this circuit.
	std::shared_ptr< lcsm::verilog::Module > circuitModule = std::make_shared< lcsm::verilog::Module >(module);
	m_verilogModules[moduleId] = circuitModule;

	// Create element.
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::VerilogModule >(circuitModule);
	lcsm::model::VerilogModule *circuit = static_cast< lcsm::model::VerilogModule * >(registerComponent(element));

	return circuit;
}

lcsm::model::VerilogModule *lcsm::LCSMCircuit::createVerilogModule(lcsm::verilog::Module &&module)
{
	// Indexes.
	const lcsm::Identifier moduleId = m_globalId;

	// Move Verilog's abstract module to this circuit.
	std::shared_ptr< lcsm::verilog::Module > circuitModule = std::make_shared< lcsm::verilog::Module >(std::move(module));
	m_verilogModules[moduleId] = circuitModule;

	// Create element.
	std::shared_ptr< lcsm::Circuit > element = std::make_shared< lcsm::model::VerilogModule >(circuitModule);
	lcsm::model::VerilogModule *circuit = static_cast< lcsm::model::VerilogModule * >(registerComponent(element));

	return circuit;
}

static lcsm::Circuit *FindCircuit(const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &m, lcsm::Circuit *c)
{
	const auto found = std::find_if(
		m.begin(),
		m.end(),
		[c](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &i) { return i.second.get() == c; });
	if (found == m.cend())
	{
		return nullptr;
	}
	{
		return found->second.get();
	}
}

lcsm::model::Wire *lcsm::LCSMCircuit::connect(lcsm::Circuit *circuit1, lcsm::portid_t port1, lcsm::Circuit *circuit2, lcsm::portid_t port2)
{
	// If circuit1 or circuit2 is nullptrs, then return nullptr - as not succeeded.
	if (circuit1 == nullptr || circuit2 == nullptr)
		return nullptr;

	// Ensure, that circuit1 and circuit2 is in this circuit.
	const lcsm::Circuit *foundAsComponent1 = FindCircuit(m_components, circuit1);
	const lcsm::Circuit *foundAsConnectorWire1 = FindCircuit(m_connectorWires, circuit1);

	lcsm::Circuit *foundAsInputInCircuit1 = nullptr;
	for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &lcsmCircuit : m_circuits)
	{
		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &inputsInLcsmCircuit = lcsmCircuit.second->inputs();
		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &outputsInLcsmCircuit = lcsmCircuit.second->outputs();

		lcsm::Circuit *foundInInputs = FindCircuit(inputsInLcsmCircuit, circuit1);
		lcsm::Circuit *foundInOutputs = FindCircuit(outputsInLcsmCircuit, circuit1);

		if (foundInInputs != nullptr)
		{
			foundAsInputInCircuit1 = foundInInputs;
			break;
		}

		if (foundInOutputs != nullptr)
		{
			foundAsInputInCircuit1 = foundInOutputs;
			break;
		}
	}

	const lcsm::Circuit *foundAsComponent2 = FindCircuit(m_components, circuit2);
	const lcsm::Circuit *foundAsConnectorWire2 = FindCircuit(m_connectorWires, circuit2);

	lcsm::Circuit *foundAsInputInCircuit2 = nullptr;
	for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &lcsmCircuit : m_circuits)
	{
		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &inputsInLcsmCircuit = lcsmCircuit.second->inputs();
		const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &outputsInLcsmCircuit = lcsmCircuit.second->outputs();

		lcsm::Circuit *foundInInputs = FindCircuit(inputsInLcsmCircuit, circuit2);
		lcsm::Circuit *foundInOutputs = FindCircuit(outputsInLcsmCircuit, circuit2);

		if (foundInInputs != nullptr)
		{
			foundAsInputInCircuit2 = foundInInputs;
			break;
		}

		if (foundInOutputs != nullptr)
		{
			foundAsInputInCircuit2 = foundInOutputs;
			break;
		}
	}

	if ((foundAsComponent1 == nullptr && foundAsConnectorWire1 == nullptr && foundAsInputInCircuit1 == nullptr) ||
		(foundAsComponent2 == nullptr && foundAsConnectorWire2 == nullptr && foundAsInputInCircuit2 == nullptr))
		throw std::logic_error("One of two components is not found in this circuit");

	// Make connect via new wire.
	std::shared_ptr< lcsm::model::Wire > wire = createConnectorWire();
	circuit1->connect(port1, wire.get());
	circuit2->connect(port2, wire.get());
	lcsm::Circuit *wire1 = circuit1->byPort(port1);
	lcsm::Circuit *wire2 = circuit2->byPort(port2);
	wire->connectToWire(wire1);
	wire->connectToWire(wire2);
	return wire.get();
}

lcsm::model::Wire *lcsm::LCSMCircuit::connect(lcsm::Circuit *circuit1, lcsm::Circuit *circuit2, lcsm::portid_t port2)
{
	// Use first circuit's default port, when `circuit1` is not nullptr.
	if (circuit1 == nullptr)
	{
		return nullptr;
	}
	return connect(circuit1, circuit1->defaultPort(), circuit2, port2);
}

lcsm::model::Wire *lcsm::LCSMCircuit::connect(lcsm::Circuit *circuit1, lcsm::portid_t port1, lcsm::Circuit *circuit2)
{
	// Use second circuit's default port, when `circuit2` is not nullptr.
	if (circuit2 == nullptr)
	{
		return nullptr;
	}
	return connect(circuit1, port1, circuit2, circuit2->defaultPort());
}

lcsm::model::Wire *lcsm::LCSMCircuit::connect(lcsm::Circuit *circuit1, lcsm::Circuit *circuit2)
{
	// Use default ports, when `circuit1` and `circuit2` are not nullptr.
	if (circuit1 == nullptr || circuit2 == nullptr)
	{
		return nullptr;
	}
	return connect(circuit1, circuit1->defaultPort(), circuit2, circuit2->defaultPort());
}

lcsm::Circuit *lcsm::LCSMCircuit::find(lcsm::Circuit *circuit) noexcept
{
	std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator found = std::find_if(
		m_components.begin(),
		m_components.end(),
		[circuit](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &i)
		{ return i.second.get() == circuit; });

	if (found != m_components.end())
	{
		return found->second.get();
	}

	found = std::find_if(
		m_connectorWires.begin(),
		m_connectorWires.end(),
		[circuit](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &i)
		{ return i.second.get() == circuit; });

	if (found != m_connectorWires.end())
	{
		return found->second.get();
	}

	for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &inner : m_circuits)
	{
		lcsm::Circuit *foundInner = inner.second->findInput(circuit);
		if (foundInner != nullptr)
		{
			return foundInner;
		}

		foundInner = inner.second->findOutput(circuit);
		if (foundInner != nullptr)
		{
			return foundInner;
		}
	}

	return nullptr;
}

lcsm::Circuit *lcsm::LCSMCircuit::find(lcsm::Identifier id) noexcept
{
	std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::iterator found = m_components.find(id);

	if (found != m_components.end())
	{
		return found->second.get();
	}

	found = m_connectorWires.find(id);

	if (found != m_connectorWires.end())
	{
		return found->second.get();
	}

	for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &inner : m_circuits)
	{
		lcsm::Circuit *foundInner = inner.second->findInput(id);
		if (foundInner != nullptr)
		{
			return foundInner;
		}

		foundInner = inner.second->findOutput(id);
		if (foundInner != nullptr)
		{
			return foundInner;
		}
	}

	return nullptr;
}

lcsm::Circuit *lcsm::LCSMCircuit::find(lcsm::label_t name) noexcept
{
	std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator found = std::find_if(
		m_components.begin(),
		m_components.end(),
		[name](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &i)
		{ return i.second->name() == name; });

	if (found != m_components.end())
	{
		return found->second.get();
	}

	found = std::find_if(
		m_connectorWires.begin(),
		m_connectorWires.end(),
		[name](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &i)
		{ return i.second->name() == name; });

	if (found != m_connectorWires.end())
	{
		return found->second.get();
	}

	for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &inner : m_circuits)
	{
		lcsm::Circuit *foundInner = inner.second->findInput(name);
		if (foundInner != nullptr)
		{
			return foundInner;
		}

		foundInner = inner.second->findOutput(name);
		if (foundInner != nullptr)
		{
			return foundInner;
		}
	}

	return nullptr;
}

lcsm::Circuit *lcsm::LCSMCircuit::find(const std::string &name) noexcept
{
	return find(name.c_str());
}

static inline std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator
	FindByPtr(const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &m, const lcsm::Circuit *circuit)
{
	return std::find_if(
		m.begin(),
		m.end(),
		[circuit](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &i)
		{ return i.second.get() == circuit; });
}

static inline std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator
	FindByName(const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &m, lcsm::label_t name)
{
	return std::find_if(
		m.begin(),
		m.end(),
		[name](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &i)
		{ return i.second->name() == name; });
}

lcsm::Circuit *lcsm::LCSMCircuit::findInput(lcsm::Circuit *circuit) noexcept
{
	// Find this circuit, if not found - nullptr.
	const auto found = FindByPtr(m_inputs, circuit);
	if (found == m_inputs.end())
	{
		return nullptr;
	}
	return found->second.get();
}

lcsm::Circuit *lcsm::LCSMCircuit::findInput(lcsm::Identifier id) noexcept
{
	// Find this circuit, if not found - nullptr.
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator found = m_inputs.find(id);
	if (found == m_inputs.end())
	{
		return nullptr;
	}
	return found->second.get();
}

lcsm::Circuit *lcsm::LCSMCircuit::findInput(lcsm::label_t name) noexcept
{
	// Find this circuit, if not found - nullptr.
	const auto found = FindByName(m_inputs, name);
	if (found == m_inputs.end())
	{
		return nullptr;
	}
	return found->second.get();
}

lcsm::Circuit *lcsm::LCSMCircuit::findInput(const std::string &name) noexcept
{
	return findInput(name.c_str());
}

lcsm::Circuit *lcsm::LCSMCircuit::findOutput(lcsm::Circuit *circuit) noexcept
{
	// Find this circuit, if not found - nullptr.
	const auto found = FindByPtr(m_outputs, circuit);
	if (found == m_outputs.end())
	{
		return nullptr;
	}
	return found->second.get();
}

lcsm::Circuit *lcsm::LCSMCircuit::findOutput(lcsm::Identifier id) noexcept
{
	// Find this circuit, if not found - nullptr.
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator found = m_outputs.find(id);
	if (found == m_outputs.end())
	{
		return nullptr;
	}
	return found->second.get();
}

lcsm::Circuit *lcsm::LCSMCircuit::findOutput(lcsm::label_t name) noexcept
{
	// Find this circuit, if not found - nullptr.
	const auto found = FindByName(m_outputs, name);
	if (found == m_outputs.end())
	{
		return nullptr;
	}
	return found->second.get();
}

lcsm::Circuit *lcsm::LCSMCircuit::findOutput(const std::string &name) noexcept
{
	return findOutput(name.c_str());
}

bool lcsm::LCSMCircuit::remove(lcsm::Circuit *circuit)
{
	// If `circuit` is nullptr, do nothing.
	if (circuit == nullptr)
	{
		return false;
	}

	// Find this circuit, if not found - exit.
	if (find(circuit) == nullptr)
	{
		return false;
	}

	// Remove by identifier.
	const lcsm::Identifier id = circuit->id();
	m_inputs.erase(id);
	m_outputs.erase(id);
	m_components.erase(id);
	m_connectorWires.erase(id);
	m_componentWires.erase(id);

	// As it was naturally erased, so then it's now really erased from circuit.
	return true;
}

bool lcsm::LCSMCircuit::remove(lcsm::Identifier id)
{
	return remove(find(id));
}

bool lcsm::LCSMCircuit::remove(lcsm::label_t name)
{
	return remove(find(name));
}

bool lcsm::LCSMCircuit::remove(const std::string &name)
{
	return remove(name.c_str());
}

lcsm::LCSMCircuitView lcsm::LCSMCircuit::addCircuit(const lcsm::LCSMCircuit &other)
{
	lcsm::model::LCSMBuilder builder;
	return addCircuit(other, builder);
}

lcsm::LCSMCircuitView lcsm::LCSMCircuit::findCircuit(const lcsm::LCSMCircuitView &circuit) noexcept
{
	// Find this circuit, if not found - exit.
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > >::const_iterator found = std::find_if(
		m_circuits.begin(),
		m_circuits.end(),
		[circuit](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &i)
		{ return i.second.get() == circuit.get(); });
	if (found == m_circuits.end())
	{
		return {};
	}
	return circuit;
}

lcsm::LCSMCircuitView lcsm::LCSMCircuit::findCircuit(lcsm::Identifier circuitId) noexcept
{
	std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > >::iterator found = m_circuits.find(circuitId);
	if (found == m_circuits.end())
	{
		return {};
	}
	return { found->second.get() };
}

lcsm::LCSMCircuitView lcsm::LCSMCircuit::findCircuit(lcsm::label_t name) noexcept
{
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > >::const_iterator found = std::find_if(
		m_circuits.begin(),
		m_circuits.end(),
		[name](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &i)
		{ return i.second->name() == name; });
	if (found == m_circuits.end())
	{
		return {};
	}
	return { found->second.get() };
}

lcsm::LCSMCircuitView lcsm::LCSMCircuit::findCircuit(const std::string &name) noexcept
{
	return findCircuit(name.c_str());
}

bool lcsm::LCSMCircuit::removeCircuit(const lcsm::LCSMCircuitView &circuit)
{
	// If circuit is not presented, no need to waste your time.
	if (!circuit.present())
	{
		return false;
	}

	// Find this circuit, if not found - exit.
	const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > >::const_iterator found = std::find_if(
		m_circuits.begin(),
		m_circuits.end(),
		[circuit](const std::pair< lcsm::Identifier, std::shared_ptr< lcsm::LCSMCircuit > > &i)
		{ return i.second.get() == circuit.get(); });
	if (found == m_circuits.end())
	{
		return false;
	}

	// Erase this circuit and as it's done, return `true`.
	m_circuits.erase(found);
	return true;
}

bool lcsm::LCSMCircuit::removeCircuit(lcsm::Identifier id)
{
	const lcsm::LCSMCircuitView circuit = findCircuit(id);
	return removeCircuit(circuit);
}

bool lcsm::LCSMCircuit::removeCircuit(lcsm::label_t name)
{
	const lcsm::LCSMCircuitView circuit = findCircuit(name);
	return removeCircuit(circuit);
}

bool lcsm::LCSMCircuit::removeCircuit(const std::string &name)
{
	const lcsm::LCSMCircuitView circuit = findCircuit(name);
	return removeCircuit(circuit);
}

lcsm::LCSMCircuitView lcsm::LCSMCircuit::addCircuit(const lcsm::LCSMCircuit &other, lcsm::model::LCSMBuilder &builder, std::size_t depth)
{
	std::shared_ptr< lcsm::LCSMCircuit > circuit = std::make_shared< lcsm::LCSMCircuit >(other.m_name);
	const lcsm::Identifier circuitId = m_globalId;
	other.copyImpl(circuit.get(), builder, m_globalId, depth);
	m_circuits[circuitId] = circuit;
	m_globalId = circuit->globalId().next();
	return { circuit.get() };
}

std::string lcsm::LCSMCircuit::dumpToString() const
{
	std::shared_ptr< lcsm::support::Writer > w = std::make_shared< lcsm::support::StringWriter >();
	lcsm::model::LCSMFileWriter writer(w);
	dumpImpl(writer);
	return static_cast< lcsm::support::StringWriter * >(w.get())->toString();
}

void lcsm::LCSMCircuit::dumpToFile(const std::string &filename) const
{
	dumpToFile(filename.c_str());
}

void lcsm::LCSMCircuit::dumpToFile(const char *filename) const
{
	std::shared_ptr< lcsm::support::Writer > w = std::make_shared< lcsm::support::FileWriter >(filename);
	lcsm::model::LCSMFileWriter writer(w);
	dumpImpl(writer);
}

lcsm::LCSMCircuit lcsm::LCSMCircuit::fromString(const std::string &string)
{
	return lcsm::LCSMCircuit::fromString(string.c_str());
}

lcsm::LCSMCircuit lcsm::LCSMCircuit::fromString(const char *string)
{
	std::shared_ptr< lcsm::support::Reader > r = std::make_shared< lcsm::support::CStringReader >(string);
	lcsm::model::LCSMFileReader reader(r);
	lcsm::model::LCSMBuilder builder;
	return lcsm::LCSMCircuit::fromImpl(reader, builder);
}

lcsm::LCSMCircuit lcsm::LCSMCircuit::fromFile(const std::string &filename)
{
	return lcsm::LCSMCircuit::fromFile(filename.c_str());
}

lcsm::LCSMCircuit lcsm::LCSMCircuit::fromFile(const char *filename)
{
	std::shared_ptr< lcsm::support::Reader > r = std::make_shared< lcsm::support::FileReader >(filename);
	lcsm::model::LCSMFileReader reader(r);
	lcsm::model::LCSMBuilder builder;
	return lcsm::LCSMCircuit::fromImpl(reader, builder);
}

void lcsm::LCSMCircuit::copyImpl(lcsm::LCSMCircuit *newCircuit, lcsm::model::LCSMBuilder &builder, const Identifier &entryId, std::size_t depth) const
{
	// Re-entry global Id.
	newCircuit->m_globalId = entryId;

	// Stage 1.1. Re-create all needed components, remember old-to-new-ids (as entryId might be not as global id).
	for (const auto &component : m_components)
	{
		const std::shared_ptr< lcsm::Circuit > &circuit = component.second;
		switch (circuit->circuitType())
		{
		case lcsm::CircuitType::Wire:
		{
			throw std::logic_error("Wire in components? The fuck you have done?!");
		}
		case lcsm::CircuitType::Tunnel:
		{
			circuit->copy(newCircuit->createTunnel(), builder);
			break;
		}
		case lcsm::CircuitType::Pin:
		{
			const lcsm::model::Pin *oldPin = static_cast< const lcsm::model::Pin * >(circuit.get());
			circuit->copy(newCircuit->createPin(oldPin->output()), builder);
			break;
		}
		case lcsm::CircuitType::Constant:
		{
			circuit->copy(newCircuit->createConstant(), builder);
			break;
		}
		case lcsm::CircuitType::Power:
		{
			circuit->copy(newCircuit->createPower(), builder);
			break;
		}
		case lcsm::CircuitType::Ground:
		{
			circuit->copy(newCircuit->createGround(), builder);
			break;
		}
		case lcsm::CircuitType::Clock:
		{
			circuit->copy(newCircuit->createClock(), builder);
			break;
		}
		case lcsm::CircuitType::Transistor:
		{
			circuit->copy(newCircuit->createTransistor(), builder);
			break;
		}
		case lcsm::CircuitType::TransmissionGate:
		{
			circuit->copy(newCircuit->createTransmissionGate(), builder);
			break;
		}
		case lcsm::CircuitType::Button:
		{
			circuit->copy(newCircuit->createButton(), builder);
			break;
		}
		case lcsm::CircuitType::Digit:
		{
			circuit->copy(newCircuit->createDigit(), builder);
			break;
		}
		case lcsm::CircuitType::Probe:
		{
			circuit->copy(newCircuit->createProbe(), builder);
			break;
		}
		case lcsm::CircuitType::Splitter:
		{
			circuit->copy(newCircuit->createSplitter(), builder);
			break;
		}
		case lcsm::CircuitType::VerilogModule:
		{
			const lcsm::model::VerilogModule *oldVerilogModule =
				static_cast< const lcsm::model::VerilogModule * >(circuit.get());
			const std::shared_ptr< lcsm::verilog::Module > &oldModule = m_verilogModules.at(oldVerilogModule->id());
			lcsm::model::VerilogModule *newVerilogModule = newCircuit->createVerilogModule(*oldModule);
			circuit->copy(newVerilogModule, builder);
			break;
		}
		case lcsm::CircuitType::LastCircuitType:
		{
			throw std::logic_error("Found unexpected circuit type in copy-function!");
		}
		}
	}

	// Stage 1.2. Re-create all wires as 1.1.
	for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::Circuit > > &connector : m_connectorWires)
	{
		const lcsm::model::Wire *oldWire = static_cast< const lcsm::model::Wire * >(connector.second.get());
		std::shared_ptr< lcsm::Circuit > wire = newCircuit->createConnectorWire(oldWire->c_name());
		builder.oldToNew(oldWire->id(), wire->id());
		builder.addWires(oldWire, false);
	}

	// Stage 2. Copy in within circuits.
	for (const auto &circuit : m_circuits)
	{
		newCircuit->addCircuit(*circuit.second, builder, depth + 1);
	}

	// Stage 3-4. Make connections and tunnel's connections, only if depth is zero.
	builder.finalize(newCircuit);
	if (depth == 0)
	{
		builder.check();
	}
}

lcsm::LCSMCircuit lcsm::LCSMCircuit::copyImpl(const lcsm::Identifier &entryId) const
{
	lcsm::LCSMCircuit newCircuit(name());
	lcsm::model::LCSMBuilder builder;
	copyImpl(std::addressof(newCircuit), builder, entryId);
	return newCircuit;
}

void lcsm::LCSMCircuit::dumpImpl(lcsm::model::LCSMFileWriter &writer) const
{
	// Builder helper.
	lcsm::model::LCSMBuilder builder;

	// 'begincircuit'
	writer.writeBeginCircuit();

	// 'id <ID>;'
	writer.writeIdDeclaration(m_globalId);

	// 'name <NAME>;'
	writer.writeNameDeclaration(m_name);

	// Dump all components.
	for (auto it = m_components.begin(); it != m_components.end(); it++)
	{
		it->second->dump(writer, builder);
	}

	// Dump all connectors.
	for (auto it = m_connectorWires.begin(); it != m_connectorWires.end(); it++)
	{
		it->second->dump(writer, builder);
	}

	// Dump all circuits.
	for (auto it = m_circuits.begin(); it != m_circuits.end(); it++)
	{
		it->second->dumpImpl(writer);
	}

	// Dump all connections and tunnels.
	builder.dump(writer);

	// 'endcircuit'
	writer.writeEndCircuit();
}

lcsm::LCSMCircuit lcsm::LCSMCircuit::fromImpl(lcsm::model::LCSMFileReader &reader, lcsm::model::LCSMBuilder &builder, std::size_t depth)
{
	lcsm::LCSMCircuit circuit;

	// 'begincircuit'
	reader.parseBeginCircuit();

	// 'id <ID>;'
	const lcsm::Identifier id = reader.exceptIdentifier();
	if (depth != 0)
	{
		circuit.m_globalId = id;
	}

	// 'name <NAME>;'
	std::string name = reader.exceptName();
	circuit.m_name = std::move(name);

	// Try to parse 'component' over and over again.
	while (reader.tryParseBeginComponent())
	{
		// 'circuittype <INTEGER>;' <-- must be the first.
		const lcsm::CircuitType circuitType = static_cast< lcsm::CircuitType >(reader.exceptCircuitType());

		// Choose.
		switch (circuitType)
		{
		case lcsm::CircuitType::Wire:
		{
			// Create Wire component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > wire = circuit.preRegisterComponent(new lcsm::model::Wire);
			// Fill the component and builder from reader.
			wire->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(wire);
			break;
		}
		case lcsm::CircuitType::Tunnel:
		{
			// Create Tunnel component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > tunnel = circuit.preRegisterComponent(new lcsm::model::Tunnel);
			// Fill the component and builder from reader.
			tunnel->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(tunnel);
			break;
		}
		case lcsm::CircuitType::Pin:
		{
			// Create Pin component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > pin = circuit.preRegisterComponent(new lcsm::model::Pin);
			// Fill the component and builder from reader.
			pin->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(pin);
			break;
		}
		case lcsm::CircuitType::Constant:
		{
			// Create Constant component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > constant = circuit.preRegisterComponent(new lcsm::model::Constant);
			// Fill the component and builder from reader.
			constant->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(constant);
			break;
		}
		case lcsm::CircuitType::Power:
		{
			// Create Power component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > power = circuit.preRegisterComponent(new lcsm::model::Power);
			// Fill the component and builder from reader.
			power->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(power);
			break;
		}
		case lcsm::CircuitType::Ground:
		{
			// Create Ground component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > ground = circuit.preRegisterComponent(new lcsm::model::Ground);
			// Fill the component and builder from reader.
			ground->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(ground);
			break;
		}
		case lcsm::CircuitType::Clock:
		{
			// Create Clock component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > clock = circuit.preRegisterComponent(new lcsm::model::Clock);
			// Fill the component and builder from reader.
			clock->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(clock);
			break;
		}
		case lcsm::CircuitType::Transistor:
		{
			// Create Transistor component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > transistor = circuit.preRegisterComponent(new lcsm::model::Transistor);
			// Fill the component and builder from reader.
			transistor->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(transistor);
			break;
		}
		case lcsm::CircuitType::TransmissionGate:
		{
			// Create TransmissionGate component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > transistor = circuit.preRegisterComponent(new lcsm::model::TransmissionGate);
			// Fill the component and builder from reader.
			transistor->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(transistor);
			break;
		}
		case lcsm::CircuitType::Button:
		{
			// Create Button component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > button = circuit.preRegisterComponent(new lcsm::model::Button);
			// Fill the component and builder from reader.
			button->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(button);
			break;
		}
		case lcsm::CircuitType::Digit:
		{
			// Create Digit component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > digit = circuit.preRegisterComponent(new lcsm::model::Digit);
			// Fill the component and builder from reader.
			digit->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(digit);
			break;
		}
		case lcsm::CircuitType::Probe:
		{
			// Create Probe component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > probe = circuit.preRegisterComponent(new lcsm::model::Probe);
			// Fill the component and builder from reader.
			probe->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(probe);
			break;
		}
		case lcsm::CircuitType::Splitter:
		{
			// Create Splitter component and PRE-register it to circuit.
			std::shared_ptr< lcsm::Circuit > splitter = circuit.preRegisterComponent(new lcsm::model::Splitter);
			// Fill the component and builder from reader.
			splitter->from(reader, builder);
			// Perform POST-registration.
			circuit.postRegisterComponent(splitter);
			break;
		}
		case lcsm::CircuitType::VerilogModule:
		{
			// Create Verilog's Module from parsed source.
			lcsm::verilog::Module module = lcsm::verilog::Module::fromString(reader.exceptEscapedKeyValue("source"));

			// Create Verilog's Module component.
			lcsm::model::VerilogModule *verilogModule = circuit.createVerilogModule(std::move(module));

			// Fill the component and builder from reader.
			verilogModule->from(reader, builder);

			// No need to perform POST-registration.
			break;
		}
		default:
		{
			throw std::logic_error("Found unknown circuit type!");
		}
		}

		// 'endcomponent'
		reader.parseEndComponent();
	}

	// Parse inner circuits, if presented.
	while (reader.tryParseBeginCircuit())
	{
		// Back 'begincircuit' to lexer for reading inner circuit.
		reader.back();

		// Parse inner circuit and add to circuits.
		std::shared_ptr< lcsm::LCSMCircuit > inner =
			std::make_shared< lcsm::LCSMCircuit >(lcsm::LCSMCircuit::fromImpl(reader, builder, depth + 1));
		circuit.m_circuits[inner->globalId()] = inner;
	}

	// 'beginconnections'
	reader.parseBeginConnections();

	while (reader.tryParseConnect())
	{
		// Back 'connect' to lexer for reading connect.
		reader.back();

		// Parse connection.
		builder.addWire(reader.exceptConnect());
	}

	// 'endconnections'
	reader.parseEndConnections();

	// 'begintunnels'
	reader.parseBeginTunnels();

	while (reader.tryParseTunnel())
	{
		// Back 'connect' to lexer for reading connect.
		reader.back();

		// Parse connection.
		builder.addTunnel(reader.exceptTunnel());
	}

	// 'endtunnels'
	reader.parseEndTunnels();

	// 'endcircuit'
	reader.parseEndCircuit();

	// Finalize.
	builder.finalize(std::addressof(circuit));
	if (depth == 0)
	{
		builder.check();
	}

	// Return dumped.
	return circuit;
}

std::shared_ptr< lcsm::model::Wire > lcsm::LCSMCircuit::createHeadlessWire(lcsm::label_t name)
{
	return std::make_shared< lcsm::model::Wire >(name);
}

std::shared_ptr< lcsm::model::Wire > lcsm::LCSMCircuit::createIdentifiedWire(lcsm::label_t name)
{
	std::shared_ptr< lcsm::model::Wire > wire = createHeadlessWire(name);
	m_globalId = wire->identify(m_globalId);
	return wire;
}

std::shared_ptr< lcsm::model::Wire > lcsm::LCSMCircuit::createConnectorWire(lcsm::label_t name)
{
	std::shared_ptr< lcsm::model::Wire > wire = createIdentifiedWire(name);
	m_connectorWires[wire->id()] = wire;
	return wire;
}

lcsm::Circuit *lcsm::LCSMCircuit::registerComponent(std::shared_ptr< lcsm::Circuit > &circuit)
{
	preRegisterComponent(circuit);
	return postRegisterComponent(circuit);
}

std::shared_ptr< lcsm::Circuit > lcsm::LCSMCircuit::preRegisterComponent(lcsm::Circuit *circuit)
{
	std::shared_ptr< lcsm::Circuit > ptr(circuit);
	return preRegisterComponent(ptr);
}

std::shared_ptr< lcsm::Circuit > lcsm::LCSMCircuit::preRegisterComponent(std::shared_ptr< lcsm::Circuit > &circuit)
{
	const std::size_t numOfWires = circuit->numOfWires();
	std::vector< std::shared_ptr< lcsm::model::Wire > > wires;
	for (std::size_t i = 0; i < numOfWires; i++)
	{
		wires.push_back(std::make_shared< lcsm::model::Wire >());
	}
	circuit->provideWires(wires);
	const lcsm::Identifier prev = m_globalId;
	m_globalId = circuit->identify(prev);
	m_components[prev] = circuit;
	for (std::size_t i = 0; i < numOfWires; i++)
	{
		const lcsm::Identifier id = wires[i]->id();
		m_componentWires[id] = wires[i];
	}
	return circuit;
}

lcsm::Circuit *lcsm::LCSMCircuit::postRegisterComponent(lcsm::Circuit *circuit)
{
	return postRegisterComponent(m_components.at(circuit->id()));
}

lcsm::Circuit *lcsm::LCSMCircuit::postRegisterComponent(std::shared_ptr< lcsm::Circuit > &circuit)
{
	const lcsm::object_type_t objectType = circuit->objectType();
	const lcsm::Identifier id = circuit->id();
	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Input))
	{
		m_inputs[id] = circuit;
	}
	if (lcsm::TestObjectType(objectType, lcsm::ObjectType::Output))
	{
		m_outputs[id] = circuit;
	}
	return circuit.get();
}

lcsm::Circuit *lcsm::LCSMCircuit::findGloballyComponentWire(lcsm::Identifier id) noexcept
{
	const std::map< lcsm::Identifier, std::shared_ptr< lcsm::Circuit > >::const_iterator found = m_componentWires.find(id);
	if (found != m_componentWires.end())
	{
		return found->second.get();
	}

	for (const auto &inner : m_circuits)
	{
		lcsm::Circuit *foundInner = inner.second->findGloballyComponentWire(id);
		if (foundInner != nullptr)
		{
			return foundInner;
		}
	}

	return nullptr;
}
