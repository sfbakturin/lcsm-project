#ifndef SIM_SIMBUILDER_H
#define SIM_SIMBUILDER_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Component.h>
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
#include <sim/SimCircuit.h>

#include <cstddef>

namespace sim
{
	class SimBuilder
	{
	  public:
		SimBuilder(SimCircuit &circuit) noexcept;

		SimBuilder(const SimBuilder &other) = delete;
		SimBuilder(SimBuilder &&other) noexcept = delete;

		SimBuilder &operator=(const SimBuilder &other) = delete;
		SimBuilder &operator=(SimBuilder &&other) noexcept = delete;

		Constant *CreateConstant(Width width = Width::W1, std::uint64_t value = 0x1);
		Ground *CreateGround(Width width = Width::W1);
		Power *CreatePower(Width width = Width::W1);
		Pin *CreatePin(bool output, Width width = Width::W1);
		Splitter *CreateSplitter(Width widthIn = Width::W2, std::size_t widthOut = 2);
		Transistor *CreateTransistor(TransistorType type = TransistorType::P);
		TransmissionGate *CreateTransmissionGate();
		Clock *CreateClock(unsigned highDuration = 1, unsigned lowDuration = 1, unsigned phaseOffset = 0);
		Button *CreateButton(bool activeOnPress = true);
		Digit *CreateDigit(bool hasDecimalPoint = true);
		Probe *CreateProbe();

		Wire *Connect(Component *compOut, std::size_t iOut, Component *compIn, std::size_t iIn);

		Wire *ConnectConstant(Constant *constantOut, Component *compIn, std::size_t iIn);
		Wire *ConnectGround(Ground *groundOut, Component *compIn, std::size_t iIn);
		Wire *ConnectPower(Power *powerOut, Component *compIn, std::size_t iIn);
		Wire *ConnectPin(Pin *pinOut, Component *compIn, std::size_t iIn);
		Wire *ConnectSplitter(Splitter *splitterOut, std::size_t iOut, Component *compIn, std::size_t iIn);
		Wire *ConnectTransistor(Transistor *transistorOut, std::size_t iOut, Component *compIn, std::size_t iIn);
		Wire *ConnectTransmissionGate(TransmissionGate *transmissionGateOut, std::size_t iOut, Component *compIn, std::size_t iIn);
		Wire *ConnectClock(Clock *clockOut, Component *compIn, std::size_t iIn);
		Wire *ConnectButton(Button *buttonOut, Component *compIn, std::size_t iIn);
		Wire *ConnectDigit(Digit *digitOut, Component *compIn, std::size_t iIn);
		Wire *ConnectProbe(Probe *probeOut, Component *compIn, std::size_t iIn);

	  private:
		SimCircuit &m_circuit;
	};
}	 // namespace sim

#endif /* SIM_SIMBUILDER_H */
