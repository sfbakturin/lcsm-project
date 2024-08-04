#ifndef SIM_SIMCONTEXT_H
#define SIM_SIMCONTEXT_H

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

		Constant *AllocaConstant(unsigned ID, Width width, std::uint64_t value);
		Ground *AllocaGround(unsigned ID, Width width);
		Power *AllocaPower(unsigned ID, Width width);
		Pin *AllocaPin(unsigned ID, bool output, Width width);
		Splitter *AllocaSplitter(unsigned ID, Width widthIn, std::size_t widthOut);
		Transistor *AllocaTransistor(unsigned ID, TransistorType type);
		TransmissionGate *AllocaTransmissionGate(unsigned ID);
		Clock *AllocaClock(unsigned ID, unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);
		Button *AllocaButton(unsigned ID, bool activeOnPress);
		Digit *AllocaDigit(unsigned ID, bool hasDecimalPoint);
		Probe *AllocaProbe(unsigned ID);

		Wire *AllocaWire(unsigned ID);
		Tunnel *AllocaTunnel(unsigned ID);

		std::vector< support::Ref< SimCircuit > > m_circuits;
		std::vector< std::shared_ptr< Component > > m_allocated;
	};
}	 // namespace sim

#endif /* SIM_SIMCONTEXT_H */
