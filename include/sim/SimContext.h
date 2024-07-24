#ifndef SIM_SIMCONTEXT_H
#define SIM_SIMCONTEXT_H

#include <sim/Component/Component.h>
#include <sim/Component/IOComponent.h>
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
#include <sim/Model/Wiring/Tunnel.h>
#include <sim/Model/Wiring/Wire.h>
#include <sim/Support/Ref.hpp>

#include <cstdint>
#include <memory>
#include <vector>

namespace sim
{
	class SimCircuit;

	class SimContext
	{
	  public:
		SimContext() = default;

		SimContext(const SimContext &other) = delete;
		SimContext(SimContext &&other) noexcept;

		SimContext &operator=(const SimContext &other) = delete;
		SimContext &operator=(SimContext &&other) noexcept;

		void Swap(SimContext &other) noexcept;

	  private:
		friend class SimCircuit;

		void AddCircuit(SimCircuit *circuit);

		Constant *AllocaConstant(Width width, std::uint64_t value);
		Ground *AllocaGround(Width width);
		Power *AllocaPower(Width width);
		Pin *AllocaPin(bool output, Width width, std::uint64_t value);
		Splitter *AllocaSplitter(Width widthIn, std::size_t widthOut);
		Transistor *AllocaTransistor(TransistorType type);
		TransmissionGate *AllocaTransmissionGate();
		Clock *AllocaClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);
		Button *AllocaButton(bool activeOnPress);
		Digit *AllocaDigit(bool hasDecimalPoint);
		Probe *AllocaProbe();

		Wire *AllocaWire();
		Tunnel *AllocaTunnel();

		std::vector< support::Ref< SimCircuit > > m_circuits;
		std::vector< std::shared_ptr< Component > > m_allocated;
	};
}	 // namespace sim

#endif /* SIM_SIMCONTEXT_H */
