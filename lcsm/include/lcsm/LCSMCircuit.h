#ifndef LCSM_LCSMCIRCUIT_H
#define LCSM_LCSMCIRCUIT_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Button.h>
#include <lcsm/Model/std/Clock.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Digit.h>
#include <lcsm/Model/std/Ground.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Power.h>
#include <lcsm/Model/std/Probe.h>
#include <lcsm/Model/std/Splitter.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Model/std/TransmissionGate.h>
#include <lcsm/Model/std/Tunnel.h>
#include <lcsm/Support/PointerView.hpp>
#include <unordered_map>

#include <memory>

namespace lcsm
{
	class LCSMCircuit
	{
	  public:
		LCSMCircuit() = default;

		LCSMCircuit(const LCSMCircuit &other);
		LCSMCircuit(LCSMCircuit &&other) noexcept;

		LCSMCircuit &operator=(const LCSMCircuit &other);
		LCSMCircuit &operator=(LCSMCircuit &&other) noexcept;

		void swap(LCSMCircuit &other) noexcept;

		const std::unordered_map< Identifier, std::shared_ptr< Circuit > > &components() const noexcept;
		const std::unordered_map< Identifier, std::shared_ptr< Circuit > > &inputs() const noexcept;
		const std::unordered_map< Identifier, std::shared_ptr< Circuit > > &outputs() const noexcept;

		model::Constant *createConstant(Width width = Width::Bit1, value_t value = 0x1);
		model::Ground *createGround(Width width = Width::Bit1);
		model::Power *createPower(Width width = Width::Bit1);
		model::Pin *createPin(bool output, Width width = Width::Bit1);
		model::Transistor *createTransistor(model::Transistor::Type type = model::Transistor::Type::P);
		model::TransmissionGate *createTransmissionGate();
		model::Tunnel *createTunnel();
		model::Clock *createClock(unsigned highDuration = 1, unsigned lowDuration = 1, unsigned phaseOffset = 0);
		model::Button *createButton(bool activeOnPress = false);
		model::Digit *createDigit(bool hasDecimalPoint = true);
		model::Probe *createProbe();
		model::Splitter *createSplitter(Width widthIn = Width::Bit2, width_t widthOut = 2);

		model::Wire *connect(Circuit *circuit1, portid_t port1, Circuit *circuit2, portid_t port2);
		void remove(Circuit *circuit);

	  private:
		Identifier m_globalId;

		std::unordered_map< Identifier, std::shared_ptr< Circuit > > m_components;
		std::unordered_map< Identifier, std::shared_ptr< Circuit > > m_inputs;
		std::unordered_map< Identifier, std::shared_ptr< Circuit > > m_outputs;
		std::unordered_map< Identifier, std::shared_ptr< Circuit > > m_wires;

	  private:
		Circuit *registerElement(std::shared_ptr< Circuit > &&circuit);
		std::shared_ptr< model::Wire > createWire();
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMCIRCUIT_H */
