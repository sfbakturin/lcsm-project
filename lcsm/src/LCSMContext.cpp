#include <lcsm/Component/Component.h>
#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/WiringComponent.h>
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

#include <cstdint>
#include <memory>
#include <utility>

lcsm::LCSMContext::LCSMContext(lcsm::LCSMContext &&other) noexcept : m_allocated(std::move(other.m_allocated)) {}

lcsm::LCSMContext &lcsm::LCSMContext::operator=(lcsm::LCSMContext &&other) noexcept
{
	if (this != &other)
		lcsm::LCSMContext(std::move(other)).Swap(*this);
	return *this;
}

void lcsm::LCSMContext::Swap(lcsm::LCSMContext &other) noexcept
{
	std::swap(m_allocated, other.m_allocated);
}

void lcsm::LCSMContext::AddCircuit(lcsm::LCSMCircuit *circuit)
{
	m_circuits.push_back(lcsm::support::PointerView< lcsm::LCSMCircuit >(circuit));
}

lcsm::model::Constant *lcsm::LCSMContext::AllocaConstant(lcsm::Width width, std::uint64_t value)
{
	std::shared_ptr< lcsm::model::Constant > constant = std::make_shared< lcsm::model::Constant >(width, value);
	m_allocated.push_back(constant);
	return constant.get();
}

lcsm::model::Ground *lcsm::LCSMContext::AllocaGround(lcsm::Width width)
{
	std::shared_ptr< lcsm::model::Ground > ground = std::make_shared< lcsm::model::Ground >(width);
	m_allocated.push_back(ground);
	return ground.get();
}

lcsm::model::Power *lcsm::LCSMContext::AllocaPower(lcsm::Width width)
{
	std::shared_ptr< lcsm::model::Power > power = std::make_shared< lcsm::model::Power >(width);
	m_allocated.push_back(power);
	return power.get();
}

lcsm::model::Pin *lcsm::LCSMContext::AllocaPin(bool output, lcsm::Width width)
{
	std::shared_ptr< lcsm::model::Pin > pin = std::make_shared< lcsm::model::Pin >(output, width);
	m_allocated.push_back(pin);
	return pin.get();
}
lcsm::model::Splitter *lcsm::LCSMContext::AllocaSplitter(lcsm::Width widthIn, std::size_t widthOut)
{
	std::shared_ptr< lcsm::model::Splitter > splitter = std::make_shared< lcsm::model::Splitter >(widthIn, widthOut);
	m_allocated.push_back(splitter);
	return splitter.get();
}

lcsm::model::Transistor *lcsm::LCSMContext::AllocaTransistor(lcsm::model::TransistorType type)
{
	std::shared_ptr< lcsm::model::Transistor > transistor = std::make_shared< lcsm::model::Transistor >(type);
	m_allocated.push_back(transistor);
	return transistor.get();
}

lcsm::model::TransmissionGate *lcsm::LCSMContext::AllocaTransmissionGate()
{
	std::shared_ptr< lcsm::model::TransmissionGate > transmissionGate = std::make_shared< lcsm::model::TransmissionGate >();
	m_allocated.push_back(transmissionGate);
	return transmissionGate.get();
}

lcsm::model::Clock *lcsm::LCSMContext::AllocaClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	std::shared_ptr< lcsm::model::Clock > clock = std::make_shared< lcsm::model::Clock >(highDuration, lowDuration, phaseOffset);
	m_allocated.push_back(clock);
	return clock.get();
}

lcsm::model::Button *lcsm::LCSMContext::AllocaButton(bool activeOnPress)
{
	std::shared_ptr< lcsm::model::Button > button = std::make_shared< lcsm::model::Button >(activeOnPress);
	m_allocated.push_back(button);
	return button.get();
}

lcsm::model::Digit *lcsm::LCSMContext::AllocaDigit(bool hasDecimalPoint)
{
	std::shared_ptr< lcsm::model::Digit > digit = std::make_shared< lcsm::model::Digit >(hasDecimalPoint);
	m_allocated.push_back(digit);
	return digit.get();
}

lcsm::model::Probe *lcsm::LCSMContext::AllocaProbe()
{
	std::shared_ptr< lcsm::model::Probe > probe = std::make_shared< lcsm::model::Probe >();
	m_allocated.push_back(probe);
	return probe.get();
}

lcsm::model::Wire *lcsm::LCSMContext::AllocaWire()
{
	std::shared_ptr< lcsm::model::Wire > wire = std::make_shared< lcsm::model::Wire >();
	m_allocated.push_back(wire);
	return wire.get();
}

lcsm::model::Tunnel *lcsm::LCSMContext::AllocaTunnel()
{
	std::shared_ptr< lcsm::model::Tunnel > tunnel = std::make_shared< lcsm::model::Tunnel >();
	m_allocated.push_back(tunnel);
	return tunnel.get();
}
