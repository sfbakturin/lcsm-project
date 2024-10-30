#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Component.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/LCSMBuilder.h>
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

lcsm::LCSMBuilder::LCSMBuilder(lcsm::LCSMCircuit &circuit) noexcept : m_circuit(circuit) {}

lcsm::model::Constant *lcsm::LCSMBuilder::CreateConstant(lcsm::Width width, std::uint64_t value)
{
	return m_circuit.RegisterConstant(width, value);
}

lcsm::model::Ground *lcsm::LCSMBuilder::CreateGround(lcsm::Width width)
{
	return m_circuit.RegisterGround(width);
}

lcsm::model::Power *lcsm::LCSMBuilder::CreatePower(lcsm::Width width)
{
	return m_circuit.RegisterPower(width);
}

lcsm::model::Pin *lcsm::LCSMBuilder::CreatePin(bool output, lcsm::Width width)
{
	return m_circuit.RegisterPin(output, width);
}

lcsm::model::Splitter *lcsm::LCSMBuilder::CreateSplitter(lcsm::Width widthIn, std::size_t widthOut)
{
	return m_circuit.RegisterSplitter(widthIn, widthOut);
}

lcsm::model::Transistor *lcsm::LCSMBuilder::CreateTransistor(lcsm::model::TransistorType type)
{
	return m_circuit.RegisterTransistor(type);
}

lcsm::model::TransmissionGate *lcsm::LCSMBuilder::CreateTransmissionGate()
{
	return m_circuit.RegisterTransmissionGate();
}

lcsm::model::Clock *lcsm::LCSMBuilder::CreateClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	return m_circuit.RegisterClock(highDuration, lowDuration, phaseOffset);
}

lcsm::model::Button *lcsm::LCSMBuilder::CreateButton(bool activeOnPress)
{
	return m_circuit.RegisterButton(activeOnPress);
}

lcsm::model::Digit *lcsm::LCSMBuilder::CreateDigit(bool hasDecimalPoint)
{
	return m_circuit.RegisterDigit(hasDecimalPoint);
}

lcsm::model::Probe *lcsm::LCSMBuilder::CreateProbe()
{
	return m_circuit.RegisterProbe();
}

lcsm::model::Wire *lcsm::LCSMBuilder::Connect(lcsm::Component *compOut, std::size_t iOut, lcsm::Component *compIn, std::size_t iIn)
{
	lcsm::model::Wire *w = m_circuit.RegisterWire();
	lcsm::wire_t wpv = lcsm::wire_t(w);
	compOut->connectOut(wpv, iOut);
	compIn->connectIn(wpv, iIn);
	return w;
}

lcsm::model::Wire *lcsm::LCSMBuilder::ConnectConstant(lcsm::model::Constant *constantOut, lcsm::Component *compIn, std::size_t iIn)
{
	return Connect(constantOut, 0, compIn, iIn);
}

lcsm::model::Wire *lcsm::LCSMBuilder::ConnectGround(lcsm::model::Ground *groundOut, lcsm::Component *compIn, std::size_t iIn)
{
	return Connect(groundOut, 0, compIn, iIn);
}

lcsm::model::Wire *lcsm::LCSMBuilder::ConnectPower(lcsm::model::Power *powerOut, lcsm::Component *compIn, std::size_t iIn)
{
	return Connect(powerOut, 0, compIn, iIn);
}

lcsm::model::Wire *lcsm::LCSMBuilder::ConnectPin(lcsm::model::Pin *pinOut, lcsm::Component *compIn, std::size_t iIn)
{
	return Connect(pinOut, 0, compIn, iIn);
}

lcsm::model::Wire *
	lcsm::LCSMBuilder::ConnectSplitter(lcsm::model::Splitter *splitterOut, std::size_t iOut, lcsm::Component *compIn, std::size_t iIn)
{
	return Connect(splitterOut, iOut, compIn, iIn);
}

lcsm::model::Wire *
	lcsm::LCSMBuilder::ConnectTransistor(lcsm::model::Transistor *transistorOut, std::size_t iOut, lcsm::Component *compIn, std::size_t iIn)
{
	return Connect(transistorOut, iOut, compIn, iIn);
}

lcsm::model::Wire *
	lcsm::LCSMBuilder::ConnectTransmissionGate(lcsm::model::TransmissionGate *transmissionGateOut, std::size_t iOut, lcsm::Component *compIn, std::size_t iIn)
{
	return Connect(transmissionGateOut, iOut, compIn, iIn);
}

lcsm::model::Wire *lcsm::LCSMBuilder::ConnectClock(lcsm::model::Clock *clockOut, lcsm::Component *compIn, std::size_t iIn)
{
	return Connect(clockOut, 0, compIn, iIn);
}

lcsm::model::Wire *lcsm::LCSMBuilder::ConnectButton(lcsm::model::Button *buttonOut, lcsm::Component *compIn, std::size_t iIn)
{
	return Connect(buttonOut, 0, compIn, iIn);
}

lcsm::model::Wire *lcsm::LCSMBuilder::ConnectDigit(lcsm::model::Digit *digitOut, lcsm::Component *compIn, std::size_t iIn)
{
	return Connect(digitOut, 0, compIn, iIn);
}

lcsm::model::Wire *lcsm::LCSMBuilder::ConnectProbe(lcsm::model::Probe *probeOut, lcsm::Component *compIn, std::size_t iIn)
{
	return Connect(probeOut, 0, compIn, iIn);
}
