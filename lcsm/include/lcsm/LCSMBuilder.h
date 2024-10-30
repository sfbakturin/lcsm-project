#ifndef LCSM_LCSMBUILDER_H
#define LCSM_LCSMBUILDER_H

#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Component.h>
#include <lcsm/Component/WiringComponent.h>
#include <lcsm/LCSMCircuit.h>
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

#include <cstddef>

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

		model::Constant *CreateConstant(model::Width width = model::Width::Bit1, std::uint64_t value = 0x1);
		model::Ground *CreateGround(model::Width width = model::Width::Bit1);
		model::Power *CreatePower(model::Width width = model::Width::Bit1);
		model::Pin *CreatePin(bool output, model::Width width = model::Width::Bit1);
		model::Splitter *CreateSplitter(model::Width widthIn = model::Width::Bit2, std::size_t widthOut = 2);
		model::Transistor *CreateTransistor(model::TransistorType type = model::TransistorType::TRANSISTOR_TYPE_P);
		model::TransmissionGate *CreateTransmissionGate();
		model::Clock *CreateClock(unsigned highDuration = 1, unsigned lowDuration = 1, unsigned phaseOffset = 0);
		model::Button *CreateButton(bool activeOnPress = true);
		model::Digit *CreateDigit(bool hasDecimalPoint = true);
		model::Probe *CreateProbe();

		model::Wire *Connect(Component *compOut, std::size_t iOut, Component *compIn, std::size_t iIn);

		model::Wire *ConnectConstant(model::Constant *constantOut, Component *compIn, std::size_t iIn);
		model::Wire *ConnectGround(model::Ground *groundOut, Component *compIn, std::size_t iIn);
		model::Wire *ConnectPower(model::Power *powerOut, Component *compIn, std::size_t iIn);
		model::Wire *ConnectPin(model::Pin *pinOut, Component *compIn, std::size_t iIn);
		model::Wire *ConnectSplitter(model::Splitter *splitterOut, std::size_t iOut, Component *compIn, std::size_t iIn);
		model::Wire *ConnectTransistor(model::Transistor *transistorOut, std::size_t iOut, Component *compIn, std::size_t iIn);
		model::Wire *
			ConnectTransmissionGate(model::TransmissionGate *transmissionGateOut, std::size_t iOut, Component *compIn, std::size_t iIn);
		model::Wire *ConnectClock(model::Clock *clockOut, Component *compIn, std::size_t iIn);
		model::Wire *ConnectButton(model::Button *buttonOut, Component *compIn, std::size_t iIn);
		model::Wire *ConnectDigit(model::Digit *digitOut, Component *compIn, std::size_t iIn);
		model::Wire *ConnectProbe(model::Probe *probeOut, Component *compIn, std::size_t iIn);

	  private:
		LCSMCircuit &m_circuit;
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMBUILDER_H */
