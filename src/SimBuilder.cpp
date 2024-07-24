#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Component.h>
#include <sim/Component/WiringComponent.h>
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
#include <sim/Model/Width.h>
#include <sim/SimBuilder.h>
#include <sim/SimCircuit.h>

sim::SimBuilder::SimBuilder(sim::SimCircuit &circuit) noexcept : m_circuit(circuit) {}

sim::Constant *sim::SimBuilder::CreateConstant(sim::Width width, std::uint64_t value)
{
	return m_circuit.RegisterConstant(width, value);
}
sim::Ground *sim::SimBuilder::CreateGround(sim::Width width)
{
	return m_circuit.RegisterGround(width);
}
sim::Power *sim::SimBuilder::CreatePower(sim::Width width)
{
	return m_circuit.RegisterPower(width);
}
sim::Pin *sim::SimBuilder::CreatePin(bool output, sim::Width width, std::uint64_t value)
{
	return m_circuit.RegisterPin(output, width, value);
}
sim::Splitter *sim::SimBuilder::CreateSplitter(sim::Width widthIn, std::size_t widthOut)
{
	return m_circuit.RegisterSplitter(widthIn, widthOut);
}
sim::Transistor *sim::SimBuilder::CreateTransistor(sim::TransistorType type)
{
	return m_circuit.RegisterTransistor(type);
}
sim::TransmissionGate *sim::SimBuilder::CreateTransmissionGate()
{
	return m_circuit.RegisterTransmissionGate();
}
sim::Clock *sim::SimBuilder::CreateClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	return m_circuit.RegisterClock(highDuration, lowDuration, phaseOffset);
}
sim::Button *sim::SimBuilder::CreateButton(bool activeOnPress)
{
	return m_circuit.RegisterButton(activeOnPress);
}
sim::Digit *sim::SimBuilder::CreateDigit(bool hasDecimalPoint)
{
	return m_circuit.RegisterDigit(hasDecimalPoint);
}
sim::Probe *sim::SimBuilder::CreateProbe()
{
	return m_circuit.RegisterProbe();
}

sim::Wire *sim::SimBuilder::Connect(sim::Component *compOut, std::size_t iOut, sim::Component *compIn, std::size_t iIn)
{
	sim::Wire *w = m_circuit.RegisterWire();
	compOut->ConnectOut(w, iOut);
	compIn->ConnectIn(w, iIn);
	w->Connect(compOut);
	w->Connect(compIn);
	return w;
}
sim::Wire *sim::SimBuilder::ConnectConstant(sim::Constant *constantOut, sim::Component *compIn, std::size_t iIn)
{
	return Connect(constantOut, 0, compIn, iIn);
}
sim::Wire *sim::SimBuilder::ConnectGround(sim::Ground *groundOut, sim::Component *compIn, std::size_t iIn)
{
	return Connect(groundOut, 0, compIn, iIn);
}
sim::Wire *sim::SimBuilder::ConnectPower(sim::Power *powerOut, sim::Component *compIn, std::size_t iIn)
{
	return Connect(powerOut, 0, compIn, iIn);
}
sim::Wire *sim::SimBuilder::ConnectPin(sim::Pin *pinOut, sim::Component *compIn, std::size_t iIn)
{
	return Connect(pinOut, 0, compIn, iIn);
}
sim::Wire *sim::SimBuilder::ConnectSplitter(sim::Splitter *splitterOut, std::size_t iOut, sim::Component *compIn, std::size_t iIn)
{
	return Connect(splitterOut, iOut, compIn, iIn);
}
sim::Wire *sim::SimBuilder::ConnectTransistor(sim::Transistor *transistorOut, std::size_t iOut, sim::Component *compIn, std::size_t iIn)
{
	return Connect(transistorOut, iOut, compIn, iIn);
}
sim::Wire *
	sim::SimBuilder::ConnectTransmissionGate(sim::TransmissionGate *transmissionGateOut, std::size_t iOut, sim::Component *compIn, std::size_t iIn)
{
	return Connect(transmissionGateOut, iOut, compIn, iIn);
}
sim::Wire *sim::SimBuilder::ConnectClock(sim::Clock *clockOut, sim::Component *compIn, std::size_t iIn)
{
	return Connect(clockOut, 0, compIn, iIn);
}
sim::Wire *sim::SimBuilder::ConnectButton(sim::Button *buttonOut, sim::Component *compIn, std::size_t iIn)
{
	return Connect(buttonOut, 0, compIn, iIn);
}
sim::Wire *sim::SimBuilder::ConnectDigit(sim::Digit *digitOut, sim::Component *compIn, std::size_t iIn)
{
	return Connect(digitOut, 0, compIn, iIn);
}
sim::Wire *sim::SimBuilder::ConnectProbe(sim::Probe *probeOut, sim::Component *compIn, std::size_t iIn)
{
	return Connect(probeOut, 0, compIn, iIn);
}
