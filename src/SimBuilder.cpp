#include <sim/Component/CircuitComponent.h>
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
#include <sim/SimContext.h>

#include <utility>

sim::SimBuilder::SimBuilder(sim::SimContext &context) noexcept : m_context(context) {}

sim::SimBuilder::SimBuilder(const sim::SimBuilder &other) noexcept : m_context(other.m_context) {}
sim::SimBuilder::SimBuilder(sim::SimBuilder &&other) noexcept : m_context(other.m_context) {}

sim::SimBuilder &sim::SimBuilder::operator=(const sim::SimBuilder &other) noexcept
{
	if (this != &other)
		sim::SimBuilder(other).Swap(*this);
	return *this;
}
sim::SimBuilder &sim::SimBuilder::operator=(sim::SimBuilder &&other) noexcept
{
	if (this != &other)
		sim::SimBuilder(std::move(other)).Swap(*this);
	return *this;
}

void sim::SimBuilder::Swap(sim::SimBuilder &other) noexcept
{
	std::swap(m_context, other.m_context);
}

sim::Constant *sim::SimBuilder::CreateConstant(sim::Width width, std::uint64_t value)
{
	return m_context.AllocaConstant(width, value);
}
sim::Ground *sim::SimBuilder::CreateGround(sim::Width width)
{
	return m_context.AllocaGround(width);
}
sim::Power *sim::SimBuilder::CreatePower(sim::Width width)
{
	return m_context.AllocaPower(width);
}
sim::Pin *sim::SimBuilder::CreatePin(bool output, sim::Width width, std::uint64_t value)
{
	return m_context.AllocaPin(output, width, value);
}
sim::Splitter *sim::SimBuilder::CreateSplitter(sim::Width widthIn, std::size_t widthOut)
{
	return m_context.AllocaSplitter(widthIn, widthOut);
}
sim::Transistor *sim::SimBuilder::CreateTransistor(sim::TransistorType type)
{
	return m_context.AllocaTransistor(type);
}
sim::TransmissionGate *sim::SimBuilder::CreateTransmissionGate()
{
	return m_context.AllocaTransmissionGate();
}
sim::Clock *sim::SimBuilder::CreateClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset)
{
	return m_context.AllocaClock(highDuration, lowDuration, phaseOffset);
}
sim::Button *sim::SimBuilder::CreateButton(bool activeOnPress)
{
	return m_context.AllocaButton(activeOnPress);
}
sim::Digit *sim::SimBuilder::CreateDigit(bool hasDecimalPoint)
{
	return m_context.AllocaDigit(hasDecimalPoint);
}
sim::Probe *sim::SimBuilder::CreateProbe()
{
	return m_context.AllocaProbe();
}

sim::wire_t sim::SimBuilder::Connect(sim::CircuitComponent *compOut, std::size_t iOut, sim::CircuitComponent *compIn, std::size_t iIn)
{
	sim::Wire *w = m_context.AllocaWire();
	compOut->ConnectOut(w, iOut);
	compIn->ConnectIn(w, iIn);
	w->Connect(compOut);
	w->Connect(compIn);
	return w;
}
sim::wire_t sim::SimBuilder::ConnectConstant(sim::Constant *constantOut, sim::CircuitComponent *compIn, std::size_t iIn)
{
	return Connect(constantOut, 0, compIn, iIn);
}
sim::wire_t sim::SimBuilder::ConnectGround(sim::Ground *groundOut, sim::CircuitComponent *compIn, std::size_t iIn)
{
	return Connect(groundOut, 0, compIn, iIn);
}
sim::wire_t sim::SimBuilder::ConnectPower(sim::Power *powerOut, sim::CircuitComponent *compIn, std::size_t iIn)
{
	return Connect(powerOut, 0, compIn, iIn);
}
sim::wire_t sim::SimBuilder::ConnectPin(sim::Pin *pinOut, sim::CircuitComponent *compIn, std::size_t iIn)
{
	return Connect(pinOut, 0, compIn, iIn);
}
sim::wire_t sim::SimBuilder::ConnectSplitter(sim::Splitter *splitterOut, std::size_t iOut, sim::CircuitComponent *compIn, std::size_t iIn)
{
	return Connect(splitterOut, iOut, compIn, iIn);
}
sim::wire_t
	sim::SimBuilder::ConnectTransistor(sim::Transistor *transistorOut, std::size_t iOut, sim::CircuitComponent *compIn, std::size_t iIn)
{
	return Connect(transistorOut, iOut, compIn, iIn);
}
sim::wire_t
	sim::SimBuilder::ConnectTransmissionGate(sim::TransmissionGate *transmissionGateOut, std::size_t iOut, sim::CircuitComponent *compIn, std::size_t iIn)
{
	return Connect(transmissionGateOut, iOut, compIn, iIn);
}
sim::wire_t sim::SimBuilder::ConnectClock(sim::Clock *clockOut, sim::CircuitComponent *compIn, std::size_t iIn)
{
	return Connect(clockOut, 0, compIn, iIn);
}
sim::wire_t sim::SimBuilder::ConnectButton(sim::Button *, sim::CircuitComponent *, std::size_t)
{
	// TODO: ConnectIO.
	return nullptr;
}
sim::wire_t sim::SimBuilder::ConnectDigit(sim::Digit *, sim::CircuitComponent *, std::size_t)
{
	// TODO: ConnectIO.
	return nullptr;
}
sim::wire_t sim::SimBuilder::ConnectProbe(sim::Probe *, sim::CircuitComponent *, std::size_t)
{
	// TODO: ConnectIO.
	return nullptr;
}
