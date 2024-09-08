#include <sim/Component/Component.h>
#include <sim/Component/IOComponent.h>
#include <sim/Component/Identifier.h>
#include <sim/Component/WiringComponent.h>
#include <sim/IR/Width.h>
#include <sim/Model/Circuit/Clock.h>
#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Circuit/Ground.h>
#include <sim/Model/Circuit/Pin.h>
#include <sim/Model/Circuit/Power.h>
#include <sim/Model/Circuit/Splitter.h>
#include <sim/Model/Circuit/Transistor.h>
#include <sim/Model/Circuit/TransmissionGate.h>
#include <sim/Model/IO/Button.h>
#include <sim/Model/IO/Digit.h>
#include <sim/Model/IO/Probe.h>
#include <sim/Model/Wiring/Tunnel.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/SimCircuit.h>
#include <sim/SimContext.h>
#include <sim/Support/PointerView.hpp>

sim::SimCircuit::SimCircuit(sim::SimContext &context) : m_context(context)
{
	m_context.AddCircuit(this);
}

const std::unordered_map< sim::Identifier, sim::support::PointerView< sim::Component > > &
	sim::SimCircuit::Pins() const noexcept
{
	return m_pin;
}

sim::Constant *sim::SimCircuit::RegisterConstant(sim::Width width, std::uint64_t value)
{
	const sim::Identifier prev = m_globalId;
	sim::Constant *constant = m_context.AllocaConstant(width, value);

	m_globalId = constant->identify(m_globalId);
	m_comp[prev] = { constant };

	return constant;
}

sim::Ground *sim::SimCircuit::RegisterGround(sim::Width width)
{
	const sim::Identifier prev = m_globalId;
	sim::Ground *ground = m_context.AllocaGround(width);

	m_globalId = ground->identify(m_globalId);
	m_comp[prev] = { ground };

	return ground;
}

sim::Power *sim::SimCircuit::RegisterPower(sim::Width width)
{
	const sim::Identifier prev = m_globalId;
	sim::Power *power = m_context.AllocaPower(width);

	m_globalId = power->identify(prev);
	m_comp[prev] = { power };

	return power;
}

sim::Pin *sim::SimCircuit::RegisterPin(bool output, sim::Width width)
{
	const sim::Identifier prev = m_globalId;
	sim::Pin *pin = m_context.AllocaPin(output, width);

	m_globalId = pin->identify(prev);
	m_pin[prev] = sim::support::PointerView< sim::Component >(pin);

	return pin;
}

sim::Splitter *sim::SimCircuit::RegisterSplitter(sim::Width widthIn, std::size_t widthOut)
{
	const sim::Identifier prev = m_globalId;
	sim::Splitter *splitter = m_context.AllocaSplitter(widthIn, widthOut);

	m_globalId = splitter->identify(prev);
	m_comp[prev] = sim::support::PointerView< sim::Component >(splitter);

	return splitter;
}

sim::Transistor *sim::SimCircuit::RegisterTransistor(sim::TransistorType type)
{
	const sim::Identifier prev = m_globalId;
	sim::Transistor *transistor = m_context.AllocaTransistor(type);

	transistor->identify(prev);
	m_comp[prev] = sim::support::PointerView< sim::Component >(transistor);

	return transistor;
}

sim::TransmissionGate *sim::SimCircuit::RegisterTransmissionGate()
{
	const sim::Identifier prev = m_globalId;
	sim::TransmissionGate *transmissionGate = m_context.AllocaTransmissionGate();

	transmissionGate->identify(prev);
	m_comp[prev] = sim::support::PointerView< sim::Component >(transmissionGate);

	return transmissionGate;
}

sim::Clock *sim::SimCircuit::RegisterClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	const sim::Identifier prev = m_globalId;
	sim::Clock *clock = m_context.AllocaClock(highDuration, lowDuration, phaseOffset);

	m_globalId = clock->identify(prev);
	m_comp[prev] = sim::support::PointerView< sim::Component >(clock);

	return clock;
}

sim::Button *sim::SimCircuit::RegisterButton(bool activeOnPress)
{
	const sim::Identifier prev = m_globalId;
	sim::Button *button = m_context.AllocaButton(activeOnPress);

	m_globalId = button->identify(prev);
	m_io[prev] = sim::support::PointerView< sim::Component >(button);

	return button;
}

sim::Digit *sim::SimCircuit::RegisterDigit(bool hasDecimalPoint)
{
	const sim::Identifier prev = m_globalId;
	sim::Digit *digit = m_context.AllocaDigit(hasDecimalPoint);

	m_globalId = digit->identify(prev);
	m_io[prev] = sim::support::PointerView< sim::Component >(digit);

	return digit;
}

sim::Probe *sim::SimCircuit::RegisterProbe()
{
	const sim::Identifier prev = m_globalId;
	sim::Probe *probe = m_context.AllocaProbe();

	m_globalId = probe->identify(prev);
	m_io[prev] = sim::support::PointerView< sim::Component >(probe);

	return probe;
}

sim::Wire *sim::SimCircuit::RegisterWire()
{
	const sim::Identifier prev = m_globalId;
	sim::Wire *wire = m_context.AllocaWire();

	m_globalId = wire->identify(prev);
	m_comp[prev] = sim::support::PointerView< sim::Component >(wire);

	return wire;
}

sim::Tunnel *sim::SimCircuit::RegisterTunnel()
{
	const sim::Identifier prev = m_globalId;
	sim::Tunnel *tunnel = m_context.AllocaTunnel();

	m_globalId = tunnel->identify(prev);
	m_comp[prev] = sim::support::PointerView< sim::Component >(tunnel);

	return tunnel;
}
