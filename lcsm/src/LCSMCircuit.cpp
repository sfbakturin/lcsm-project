#include <lcsm/Component/Component.h>
#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/LCSMCircuit.h>
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

#include <memory>
#include <utility>

lcsm::LCSMCircuit::LCSMCircuit(lcsm::LCSMCircuit &&other) noexcept :
	m_globalId(std::move(other.m_globalId)), m_components(std::move(other.m_components))
{
}

lcsm::LCSMCircuit &lcsm::LCSMCircuit::operator=(lcsm::LCSMCircuit &&other) noexcept
{
	if (this != std::addressof(other))
		lcsm::LCSMCircuit(std::move(other)).swap(*this);
	return *this;
}

void lcsm::LCSMCircuit::swap(lcsm::LCSMCircuit &other) noexcept
{
	std::swap(m_globalId, other.m_globalId);
	std::swap(m_components, other.m_components);
}

const std::unordered_map< lcsm::Identifier, std::shared_ptr< lcsm::Component > > &lcsm::LCSMCircuit::components() const noexcept
{
	return m_components;
}

lcsm::model::Constant *lcsm::LCSMCircuit::RegisterConstant(lcsm::model::Width width, std::uint64_t value)
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Constant >(width, value);
	lcsm::model::Constant *constant = allocated->asCircuit()->asConstant();

	m_globalId = constant->identify(prev);
	m_components[prev] = std::move(allocated);

	return constant;
}

lcsm::model::Ground *lcsm::LCSMCircuit::RegisterGround(lcsm::model::Width width)
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Ground >(width);
	lcsm::model::Ground *ground = allocated->asCircuit()->asGround();

	m_globalId = ground->identify(prev);
	m_components[prev] = std::move(allocated);

	return ground;
}

lcsm::model::Power *lcsm::LCSMCircuit::RegisterPower(lcsm::model::Width width)
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Power >(width);
	lcsm::model::Power *power = allocated->asCircuit()->asPower();

	m_globalId = power->identify(prev);
	m_components[prev] = std::move(allocated);

	return power;
}

lcsm::model::Pin *lcsm::LCSMCircuit::RegisterPin(bool output, lcsm::model::Width width)
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Pin >(output, width);
	lcsm::model::Pin *pin = allocated->asCircuit()->asPin();

	m_globalId = pin->identify(prev);
	m_components[prev] = std::move(allocated);

	return pin;
}

lcsm::model::Splitter *lcsm::LCSMCircuit::RegisterSplitter(lcsm::model::Width widthIn, std::size_t widthOut)
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Splitter >(widthIn, widthOut);
	lcsm::model::Splitter *splitter = allocated->asCircuit()->asSplitter();

	m_globalId = splitter->identify(prev);
	m_components[prev] = std::move(allocated);

	return splitter;
}

lcsm::model::Transistor *lcsm::LCSMCircuit::RegisterTransistor(lcsm::model::TransistorType type)
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Transistor >(type);
	lcsm::model::Transistor *transistor = allocated->asCircuit()->asTransistor();

	m_globalId = transistor->identify(prev);
	m_components[prev] = std::move(allocated);

	return transistor;
}

lcsm::model::TransmissionGate *lcsm::LCSMCircuit::RegisterTransmissionGate()
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::TransmissionGate >();
	lcsm::model::TransmissionGate *transmissionGate = allocated->asCircuit()->asTransmissionGate();

	m_globalId = transmissionGate->identify(prev);
	m_components[prev] = std::move(allocated);

	return transmissionGate;
}

lcsm::model::Clock *lcsm::LCSMCircuit::RegisterClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Clock >(highDuration, lowDuration, phaseOffset);
	lcsm::model::Clock *clock = allocated->asCircuit()->asClock();

	m_globalId = clock->identify(prev);
	m_components[prev] = std::move(allocated);

	return clock;
}

lcsm::model::Button *lcsm::LCSMCircuit::RegisterButton(bool activeOnPress)
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Button >(activeOnPress);
	lcsm::model::Button *button = allocated->asIO()->asButton();

	m_globalId = button->identify(prev);
	m_components[prev] = std::move(allocated);

	return button;
}

lcsm::model::Digit *lcsm::LCSMCircuit::RegisterDigit(bool hasDecimalPoint)
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Digit >(hasDecimalPoint);
	lcsm::model::Digit *digit = allocated->asIO()->asDigit();

	m_globalId = digit->identify(prev);
	m_components[prev] = std::move(allocated);

	return digit;
}

lcsm::model::Probe *lcsm::LCSMCircuit::RegisterProbe()
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Probe >();
	lcsm::model::Probe *probe = allocated->asIO()->asProbe();

	m_globalId = probe->identify(prev);
	m_components[prev] = std::move(allocated);

	return probe;
}

lcsm::model::Wire *lcsm::LCSMCircuit::RegisterWire()
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Wire >();
	lcsm::model::Wire *wire = allocated->asWiring()->asWire();

	m_globalId = wire->identify(prev);
	m_components[prev] = std::move(allocated);

	return wire;
}

lcsm::model::Tunnel *lcsm::LCSMCircuit::RegisterTunnel()
{
	const lcsm::Identifier prev = m_globalId;

	std::shared_ptr< lcsm::Component > allocated = std::make_shared< lcsm::model::Tunnel >();
	lcsm::model::Tunnel *tunnel = allocated->asWiring()->asTunnel();

	m_globalId = tunnel->identify(prev);
	m_components[prev] = std::move(allocated);

	return tunnel;
}
