#include <lcsm/Component/Component.h>
#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMContext.h>
#include <lcsm/Model/Circuit/Clock.h>
#include <lcsm/Model/Circuit/Constant.h>
#include <lcsm/Model/Circuit/Ground.h>
#include <lcsm/Model/Circuit/Pin.h>
#include <lcsm/Model/Circuit/Power.h>
#include <lcsm/Model/Circuit/Splitter.h>
#include <lcsm/Model/Circuit/Transistor.h>
#include <lcsm/Model/Circuit/TransmissionGate.h>
#include <lcsm/Model/IO/Button.h>
#include <lcsm/Model/IO/Digit.h>
#include <lcsm/Model/IO/Probe.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wiring/Tunnel.h>
#include <lcsm/Model/Wiring/Wire.h>
#include <lcsm/Support/PointerView.hpp>

lcsm::LCSMCircuit::LCSMCircuit(lcsm::LCSMContext &context) : m_context(context)
{
	m_context.AddCircuit(this);
}

const std::unordered_map< lcsm::Identifier, lcsm::support::PointerView< lcsm::Component > > &lcsm::LCSMCircuit::Pins() const noexcept
{
	return m_pin;
}

lcsm::model::Constant *lcsm::LCSMCircuit::RegisterConstant(lcsm::model::Width width, std::uint64_t value)
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Constant *constant = m_context.AllocaConstant(width, value);

	m_globalId = constant->identify(m_globalId);
	m_comp[prev] = { constant };

	return constant;
}

lcsm::model::Ground *lcsm::LCSMCircuit::RegisterGround(lcsm::model::Width width)
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Ground *ground = m_context.AllocaGround(width);

	m_globalId = ground->identify(m_globalId);
	m_comp[prev] = { ground };

	return ground;
}

lcsm::model::Power *lcsm::LCSMCircuit::RegisterPower(lcsm::model::Width width)
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Power *power = m_context.AllocaPower(width);

	m_globalId = power->identify(prev);
	m_comp[prev] = { power };

	return power;
}

lcsm::model::Pin *lcsm::LCSMCircuit::RegisterPin(bool output, lcsm::model::Width width)
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Pin *pin = m_context.AllocaPin(output, width);

	m_globalId = pin->identify(prev);
	m_pin[prev] = lcsm::support::PointerView< lcsm::Component >(pin);

	return pin;
}

lcsm::model::Splitter *lcsm::LCSMCircuit::RegisterSplitter(lcsm::model::Width widthIn, std::size_t widthOut)
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Splitter *splitter = m_context.AllocaSplitter(widthIn, widthOut);

	m_globalId = splitter->identify(prev);
	m_comp[prev] = lcsm::support::PointerView< lcsm::Component >(splitter);

	return splitter;
}

lcsm::model::Transistor *lcsm::LCSMCircuit::RegisterTransistor(lcsm::model::TransistorType type)
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Transistor *transistor = m_context.AllocaTransistor(type);

	m_globalId = transistor->identify(prev);
	m_comp[prev] = lcsm::support::PointerView< lcsm::Component >(transistor);

	return transistor;
}

lcsm::model::TransmissionGate *lcsm::LCSMCircuit::RegisterTransmissionGate()
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::TransmissionGate *transmissionGate = m_context.AllocaTransmissionGate();

	m_globalId = transmissionGate->identify(prev);
	m_comp[prev] = lcsm::support::PointerView< lcsm::Component >(transmissionGate);

	return transmissionGate;
}

lcsm::model::Clock *lcsm::LCSMCircuit::RegisterClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Clock *clock = m_context.AllocaClock(highDuration, lowDuration, phaseOffset);

	m_globalId = clock->identify(prev);
	m_comp[prev] = lcsm::support::PointerView< lcsm::Component >(clock);

	return clock;
}

lcsm::model::Button *lcsm::LCSMCircuit::RegisterButton(bool activeOnPress)
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Button *button = m_context.AllocaButton(activeOnPress);

	m_globalId = button->identify(prev);
	m_io[prev] = lcsm::support::PointerView< lcsm::Component >(button);

	return button;
}

lcsm::model::Digit *lcsm::LCSMCircuit::RegisterDigit(bool hasDecimalPoint)
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Digit *digit = m_context.AllocaDigit(hasDecimalPoint);

	m_globalId = digit->identify(prev);
	m_io[prev] = lcsm::support::PointerView< lcsm::Component >(digit);

	return digit;
}

lcsm::model::Probe *lcsm::LCSMCircuit::RegisterProbe()
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Probe *probe = m_context.AllocaProbe();

	m_globalId = probe->identify(prev);
	m_io[prev] = lcsm::support::PointerView< lcsm::Component >(probe);

	return probe;
}

lcsm::model::Wire *lcsm::LCSMCircuit::RegisterWire()
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Wire *wire = m_context.AllocaWire();

	m_globalId = wire->identify(prev);
	m_comp[prev] = lcsm::support::PointerView< lcsm::Component >(wire);

	return wire;
}

lcsm::model::Tunnel *lcsm::LCSMCircuit::RegisterTunnel()
{
	const lcsm::Identifier prev = m_globalId;
	lcsm::model::Tunnel *tunnel = m_context.AllocaTunnel();

	m_globalId = tunnel->identify(prev);
	m_comp[prev] = lcsm::support::PointerView< lcsm::Component >(tunnel);

	return tunnel;
}
