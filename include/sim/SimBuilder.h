#ifndef SIM_SIMBUILDER_H
#define SIM_SIMBUILDER_H

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
#include <sim/SimContext.h>

#include <cstddef>

namespace sim
{
	class SimBuilder
	{
	  public:
		SimBuilder(SimContext &context) noexcept;

		SimBuilder(const SimBuilder &other) noexcept;
		SimBuilder(SimBuilder &&other) noexcept;

		SimBuilder &operator=(const SimBuilder &other) noexcept;
		SimBuilder &operator=(SimBuilder &&other) noexcept;

		void Swap(SimBuilder &other) noexcept;

		Constant *CreateConstant(Width width = Width::W1, std::uint64_t value = 0x1);
		Ground *CreateGround(Width width = Width::W1);
		Power *CreatePower(Width width = Width::W1);
		Pin *CreatePin(bool output, Width width = Width::W1, std::uint64_t value = 0x0);
		Splitter *CreateSplitter(Width widthIn = Width::W2, std::size_t widthOut = 2);
		Transistor *CreateTransistor(TransistorType type = TransistorType::P);
		TransmissionGate *CreateTransmissionGate();
		Clock *CreateClock(unsigned highDuration = 1, unsigned lowDuration = 1, unsigned phaseOffset = 0);
		Button *CreateButton(bool activeOnPress = true);
		Digit *CreateDigit(bool hasDecimalPoint = true);
		Probe *CreateProbe();

		wire_t Connect(CircuitComponent *compOut, std::size_t iOut, CircuitComponent *compIn, std::size_t iIn);

		wire_t ConnectConstant(Constant *constantOut, CircuitComponent *compIn, std::size_t iIn);
		wire_t ConnectGround(Ground *groundOut, CircuitComponent *compIn, std::size_t iIn);
		wire_t ConnectPower(Power *powerOut, CircuitComponent *compIn, std::size_t iIn);
		wire_t ConnectPin(Pin *pinOut, CircuitComponent *compIn, std::size_t iIn);
		wire_t ConnectSplitter(Splitter *splitterOut, std::size_t iOut, CircuitComponent *compIn, std::size_t iIn);
		wire_t ConnectTransistor(Transistor *transistorOut, std::size_t iOut, CircuitComponent *compIn, std::size_t iIn);
		wire_t ConnectTransmissionGate(TransmissionGate *transmissionGateOut, std::size_t iOut, CircuitComponent *compIn, std::size_t iIn);
		wire_t ConnectClock(Clock *clockOut, CircuitComponent *compIn, std::size_t iIn);
		wire_t ConnectButton(Button *buttonOut, CircuitComponent *compIn, std::size_t iIn);
		wire_t ConnectDigit(Digit *digitOut, CircuitComponent *compIn, std::size_t iIn);
		wire_t ConnectProbe(Probe *probeOut, CircuitComponent *compIn, std::size_t iIn);

	  private:
		SimContext &m_context;
	};
}	 // namespace sim

#endif /* SIM_SIMBUILDER_H */
