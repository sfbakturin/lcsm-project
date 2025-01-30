#include <lcsm/LCSMBuilder.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Circuit.h>
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
#include <lcsm/Support/PointerView.hpp>

#include <memory>

lcsm::LCSMBuilder::LCSMBuilder(lcsm::LCSMCircuit &circuit) noexcept : m_circuit(circuit) {}

lcsm::model::Constant *lcsm::LCSMBuilder::CreateConstant(lcsm::Width width, lcsm::value_t value)
{
	return static_cast< lcsm::model::Constant * >(m_circuit.registerElement(std::make_shared< lcsm::model::Constant >(width, value)));
}

lcsm::model::Ground *lcsm::LCSMBuilder::CreateGround(lcsm::Width width)
{
	return static_cast< lcsm::model::Ground * >(m_circuit.registerElement(std::make_shared< lcsm::model::Ground >(width)));
}

lcsm::model::Power *lcsm::LCSMBuilder::CreatePower(lcsm::Width width)
{
	return static_cast< lcsm::model::Power * >(m_circuit.registerElement(std::make_shared< lcsm::model::Power >(width)));
}

lcsm::model::Pin *lcsm::LCSMBuilder::CreatePin(bool output, lcsm::Width width)
{
	return static_cast< lcsm::model::Pin * >(m_circuit.registerElement(std::make_shared< lcsm::model::Pin >(output, width)));
}

lcsm::model::Transistor *lcsm::LCSMBuilder::CreateTransistor(lcsm::model::Transistor::Type type)
{
	return static_cast< lcsm::model::Transistor * >(m_circuit.registerElement(std::make_shared< lcsm::model::Transistor >(type)));
}

lcsm::model::TransmissionGate *lcsm::LCSMBuilder::CreateTransmissionGate()
{
	return static_cast< lcsm::model::TransmissionGate * >(
		m_circuit.registerElement(std::make_shared< lcsm::model::TransmissionGate >()));
}

lcsm::model::Clock *lcsm::LCSMBuilder::CreateClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	return static_cast< lcsm::model::Clock * >(
		m_circuit.registerElement(std::make_shared< lcsm::model::Clock >(highDuration, lowDuration, phaseOffset)));
}

lcsm::model::Wire *
	lcsm::LCSMBuilder::ConnectInternally(lcsm::Circuit *comp1, lcsm::portid_t port1, lcsm::Circuit *comp2, lcsm::portid_t port2)
{
	lcsm::model::Wire *w = static_cast< lcsm::model::Wire * >(m_circuit.registerElement(std::make_shared< lcsm::model::Wire >()));
	const lcsm::support::PointerView< lcsm::Circuit > circuit = w;
	comp1->connect(port1, circuit);
	comp2->connect(port2, circuit);
	return w;
}
