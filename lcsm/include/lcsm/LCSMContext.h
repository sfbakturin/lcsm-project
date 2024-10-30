#ifndef LCSM_LCSMCONTEXT_H
#define LCSM_LCSMCONTEXT_H

#include <lcsm/Component/Component.h>
#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/WiringComponent.h>
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
#include <vector>

namespace lcsm
{
	class LCSMCircuit;

	class LCSMContext
	{
	  public:
		LCSMContext() = default;

		LCSMContext(const LCSMContext &other) = delete;
		LCSMContext(LCSMContext &&other) noexcept;

		LCSMContext &operator=(const LCSMContext &other) = delete;
		LCSMContext &operator=(LCSMContext &&other) noexcept;

		void Swap(LCSMContext &other) noexcept;

	  private:
		friend class LCSMCircuit;

		void AddCircuit(LCSMCircuit *circuit);

		model::Constant *AllocaConstant(model::Width width, std::uint64_t value);
		model::Ground *AllocaGround(model::Width width);
		model::Power *AllocaPower(model::Width width);
		model::Pin *AllocaPin(bool output, model::Width width);
		model::Splitter *AllocaSplitter(model::Width widthIn, std::size_t widthOut);
		model::Transistor *AllocaTransistor(model::TransistorType type);
		model::TransmissionGate *AllocaTransmissionGate();
		model::Clock *AllocaClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);
		model::Button *AllocaButton(bool activeOnPress);
		model::Digit *AllocaDigit(bool hasDecimalPoint);
		model::Probe *AllocaProbe();

		model::Wire *AllocaWire();
		model::Tunnel *AllocaTunnel();

		std::vector< support::PointerView< LCSMCircuit > > m_circuits;
		std::vector< std::shared_ptr< Component > > m_allocated;
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMCONTEXT_H */
