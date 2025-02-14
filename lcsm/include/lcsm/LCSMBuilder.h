#ifndef LCSM_LCSMBUILDER_H
#define LCSM_LCSMBUILDER_H

#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
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

namespace lcsm
{
	class LCSMBuilder
	{
	  public:
		LCSMBuilder(LCSMCircuit &circuit) noexcept;

		LCSMBuilder(const LCSMBuilder &other) = delete;
		LCSMBuilder(LCSMBuilder &&other) noexcept = delete;

		LCSMBuilder &operator=(const LCSMBuilder &other) = delete;
		LCSMBuilder &operator=(LCSMBuilder &&other) noexcept = delete;

		model::Constant *CreateConstant(Width width = Width::Bit1, value_t value = 0x1);
		model::Ground *CreateGround(Width width = Width::Bit1);
		model::Power *CreatePower(Width width = Width::Bit1);
		model::Pin *CreatePin(bool output, Width width = Width::Bit1);
		model::Transistor *CreateTransistor(model::Transistor::Type type = model::Transistor::Type::P);
		model::TransmissionGate *CreateTransmissionGate();
		model::Tunnel *CreateTunnel();
		model::Clock *CreateClock(unsigned highDuration = 1, unsigned lowDuration = 1, unsigned phaseOffset = 0);
		model::Button *CreateButton(bool activeOnPress = false);
		model::Digit *CreateDigit(bool hasDecimalPoint = true);
		model::Probe *CreateProbe();
		model::Splitter *CreateSplitter(Width widthIn = Width::Bit2, width_t widthOut = 2);

		model::Wire *Connect(Circuit *comp1, portid_t port1, Circuit *comp2, portid_t port2);

	  private:
		LCSMCircuit &m_circuit;
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMBUILDER_H */
