#ifndef LCSM_LCSMCIRCUIT_H
#define LCSM_LCSMCIRCUIT_H

#include <lcsm/Component/Component.h>
#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/LCSMContext.h>
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
#include <unordered_map>

namespace lcsm
{
	class LCSMBuilder;

	class LCSMCircuit
	{
	  public:
		LCSMCircuit(LCSMContext &context);

		LCSMCircuit(const LCSMCircuit &other) = delete;
		LCSMCircuit(LCSMCircuit &&other) noexcept = delete;

		LCSMCircuit &operator=(const LCSMCircuit &other) = delete;
		LCSMCircuit &operator=(LCSMCircuit &&other) noexcept = delete;

		const std::unordered_map< Identifier, support::PointerView< Component > > &Pins() const noexcept;

	  private:
		friend class LCSMBuilder;

		model::Constant *RegisterConstant(Width width, std::uint64_t value);
		model::Ground *RegisterGround(Width width);
		model::Power *RegisterPower(Width width);
		model::Pin *RegisterPin(bool output, Width width);
		model::Splitter *RegisterSplitter(Width widthIn, std::size_t widthOut);
		model::Transistor *RegisterTransistor(model::TransistorType type);
		model::TransmissionGate *RegisterTransmissionGate();
		model::Clock *RegisterClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);
		model::Button *RegisterButton(bool activeOnPress);
		model::Digit *RegisterDigit(bool hasDecimalPoint);
		model::Probe *RegisterProbe();

		model::Wire *RegisterWire();
		model::Tunnel *RegisterTunnel();

		LCSMContext &m_context;

		Identifier m_globalId;

		std::unordered_map< Identifier, support::PointerView< Component > > m_pin;
		std::unordered_map< Identifier, support::PointerView< Component > > m_io;
		std::unordered_map< Identifier, support::PointerView< Component > > m_comp;
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMCIRCUIT_H */
