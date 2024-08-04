#ifndef SIM_SIMCIRCUIT_H
#define SIM_SIMCIRCUIT_H

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
#include <sim/SimContext.h>
#include <sim/Support/Ref.hpp>
#include <unordered_map>

namespace sim
{
	class SimBuilder;

	class SimCircuit
	{
	  public:
		SimCircuit(SimContext &context);

		SimCircuit(const SimCircuit &other) = delete;
		SimCircuit(SimCircuit &&other) noexcept = delete;

		SimCircuit &operator=(const SimCircuit &other) = delete;
		SimCircuit &operator=(SimCircuit &&other) noexcept = delete;

		const std::unordered_map< unsigned, support::Ref< Component > > &Pins() const noexcept;

	  private:
		friend class SimBuilder;

		Constant *RegisterConstant(Width width, std::uint64_t value);
		Ground *RegisterGround(Width width);
		Power *RegisterPower(Width width);
		Pin *RegisterPin(bool output, Width width);
		Splitter *RegisterSplitter(Width widthIn, std::size_t widthOut);
		Transistor *RegisterTransistor(TransistorType type);
		TransmissionGate *RegisterTransmissionGate();
		Clock *RegisterClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);
		Button *RegisterButton(bool activeOnPress);
		Digit *RegisterDigit(bool hasDecimalPoint);
		Probe *RegisterProbe();

		Wire *RegisterWire();
		Tunnel *RegisterTunnel();

		SimContext &m_context;

		unsigned m_globalId;

		std::unordered_map< unsigned, support::Ref< Component > > m_pin;
		std::unordered_map< unsigned, support::Ref< Component > > m_io;
		std::unordered_map< unsigned, support::Ref< Component > > m_comp;
	};
}	 // namespace sim

#endif
