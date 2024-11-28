#ifndef LCSM_LCSMCIRCUIT_H
#define LCSM_LCSMCIRCUIT_H

#include <lcsm/Component/Component.h>
#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/Identifier.h>
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
#include <unordered_map>

#include <memory>

namespace lcsm
{
	class LCSMBuilder;

	class LCSMCircuit
	{
	  public:
		LCSMCircuit() = default;

		LCSMCircuit(const LCSMCircuit &other) = delete;
		LCSMCircuit(LCSMCircuit &&other) noexcept;

		LCSMCircuit &operator=(const LCSMCircuit &other) = delete;
		LCSMCircuit &operator=(LCSMCircuit &&other) noexcept;

		void swap(LCSMCircuit &other) noexcept;

		const std::unordered_map< Identifier, std::shared_ptr< Component > > &components() const noexcept;

	  private:
		friend class LCSMBuilder;

		model::Constant *RegisterConstant(model::Width width, std::uint64_t value);
		model::Ground *RegisterGround(model::Width width);
		model::Power *RegisterPower(model::Width width);
		model::Pin *RegisterPin(bool output, model::Width width);
		model::Splitter *RegisterSplitter(model::Width widthIn, std::size_t widthOut);
		model::Transistor *RegisterTransistor(model::TransistorType type);
		model::TransmissionGate *RegisterTransmissionGate();
		model::Clock *RegisterClock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);
		model::Button *RegisterButton(bool activeOnPress);
		model::Digit *RegisterDigit(bool hasDecimalPoint);
		model::Probe *RegisterProbe();

		model::Wire *RegisterWire();
		model::Tunnel *RegisterTunnel();

		Identifier m_globalId;

		std::unordered_map< Identifier, std::shared_ptr< Component > > m_components;
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMCIRCUIT_H */
