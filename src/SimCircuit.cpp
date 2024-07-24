#include <sim/Component/Component.h>
#include <sim/Component/IOComponent.h>
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
#include <sim/Support/Ref.hpp>

sim::SimCircuit::SimCircuit(sim::SimContext &context) : m_context(context)
{
	m_context.AddCircuit(this);
}

sim::Constant *sim::SimCircuit::RegisterConstant(sim::Width width, std::uint64_t value)
{
	sim::Constant *constant = m_context.AllocaConstant(width, value);
	m_comp.push_back(sim::support::Ref< sim::Component >(constant));
	return constant;
}
sim::Ground *sim::SimCircuit::RegisterGround(sim::Width width)
{
	sim::Ground *ground = m_context.AllocaGround(width);
	m_comp.push_back(sim::support::Ref< sim::Component >(ground));
	return ground;
}
sim::Power *sim::SimCircuit::RegisterPower(sim::Width width)
{
	sim::Power *power = m_context.AllocaPower(width);
	m_comp.push_back(sim::support::Ref< sim::Component >(power));
	return power;
}
sim::Pin *sim::SimCircuit::RegisterPin(bool output, sim::Width width, std::uint64_t value)
{
	sim::Pin *pin = m_context.AllocaPin(output, width, value);
	m_pin.push_back(sim::support::Ref< sim::Component >(pin));
	return pin;
}
sim::Splitter *sim::SimCircuit::RegisterSplitter(sim::Width widthIn, std::size_t widthOut)
{
	sim::Splitter *splitter = m_context.AllocaSplitter(widthIn, widthOut);
	m_comp.push_back(sim::support::Ref< sim::Component >(splitter));
	return splitter;
}
sim::Transistor *sim::SimCircuit::RegisterTransistor(sim::TransistorType type)
{
	sim::Transistor *transistor = m_context.AllocaTransistor(type);
	m_comp.push_back(sim::support::Ref< sim::Component >(transistor));
	return transistor;
}
sim::TransmissionGate *sim::SimCircuit::RegisterTransmissionGate()
{
	sim::TransmissionGate *transmissionGate = m_context.AllocaTransmissionGate();
	m_comp.push_back(sim::support::Ref< sim::Component >(transmissionGate));
	return transmissionGate;
}
sim::Clock *sim::SimCircuit::RegisterClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	sim::Clock *clock = m_context.AllocaClock(highDuration, lowDuration, phaseOffset);
	m_comp.push_back(sim::support::Ref< sim::Component >(clock));
	return clock;
}
sim::Button *sim::SimCircuit::RegisterButton(bool activeOnPress)
{
	sim::Button *button = m_context.AllocaButton(activeOnPress);
	m_io.push_back(sim::support::Ref< sim::Component >(button));
	return button;
}
sim::Digit *sim::SimCircuit::RegisterDigit(bool hasDecimalPoint)
{
	sim::Digit *digit = m_context.AllocaDigit(hasDecimalPoint);
	m_io.push_back(sim::support::Ref< sim::Component >(digit));
	return digit;
}
sim::Probe *sim::SimCircuit::RegisterProbe()
{
	sim::Probe *probe = m_context.AllocaProbe();
	m_io.push_back(sim::support::Ref< sim::Component >(probe));
	return probe;
}

sim::Wire *sim::SimCircuit::RegisterWire()
{
	sim::Wire *wire = m_context.AllocaWire();
	m_comp.push_back(sim::support::Ref< sim::Component >(wire));
	return wire;
}

sim::Tunnel *sim::SimCircuit::RegisterTunnel()
{
	sim::Tunnel *tunnel = m_context.AllocaTunnel();
	m_comp.push_back(sim::support::Ref< sim::Component >(tunnel));
	return tunnel;
}
