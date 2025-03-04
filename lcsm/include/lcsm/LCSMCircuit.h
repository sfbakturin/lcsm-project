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

#include <map>
#include <memory>
#include <string>

namespace lcsm
{
	class LCSMCircuit;

	class LCSMCircuitView
	{
	  public:
		LCSMCircuitView(LCSMCircuit *circuit) noexcept;

		LCSMCircuitView(const LCSMCircuitView &other) noexcept;
		LCSMCircuitView(LCSMCircuitView &&other) noexcept;

		LCSMCircuitView &operator=(const LCSMCircuitView &other) noexcept;
		LCSMCircuitView &operator=(LCSMCircuitView &&other) noexcept;

		void swap(LCSMCircuitView &other) noexcept;

		Identifier globalId() const noexcept;
		const std::string &name() const noexcept;
		label_t c_name() const noexcept;

		const std::map< Identifier, std::shared_ptr< Circuit > > &components() const noexcept;
		const std::map< Identifier, std::shared_ptr< Circuit > > &inputs() const noexcept;
		const std::map< Identifier, std::shared_ptr< Circuit > > &outputs() const noexcept;

		Circuit *find(Identifier id) noexcept;
		Circuit *find(label_t name) noexcept;
		Circuit *find(const std::string &name) noexcept;

	  private:
		LCSMCircuit *m_circuit;
	};

	class LCSMCircuit
	{
	  public:
		LCSMCircuit() = default;
		LCSMCircuit(label_t name);

		LCSMCircuit(const LCSMCircuit &other) = delete;
		LCSMCircuit(LCSMCircuit &&other) noexcept;

		LCSMCircuit &operator=(const LCSMCircuit &other) = delete;
		LCSMCircuit &operator=(LCSMCircuit &&other) noexcept;

		void swap(LCSMCircuit &other) noexcept;

		LCSMCircuit copy() const;
		Identifier globalId() const noexcept;
		const std::string &name() const noexcept;
		label_t c_name() const noexcept;

		const std::map< Identifier, std::shared_ptr< Circuit > > &components() const noexcept;
		const std::map< Identifier, std::shared_ptr< Circuit > > &inputs() const noexcept;
		const std::map< Identifier, std::shared_ptr< Circuit > > &outputs() const noexcept;

		model::Constant *createConstant(label_t name = "", Width width = Width::Bit1, value_t value = 0x1);
		model::Ground *createGround(label_t name = "", Width width = Width::Bit1);
		model::Power *createPower(label_t name = "", Width width = Width::Bit1);
		model::Pin *createPin(bool output, label_t name = "", Width width = Width::Bit1);
		model::Transistor *createTransistor(label_t name = "", model::Transistor::Type type = model::Transistor::Type::P);
		model::TransmissionGate *createTransmissionGate(label_t name = "");
		model::Tunnel *createTunnel(label_t name = "");
		model::Clock *createClock(label_t name = "", unsigned highDuration = 1, unsigned lowDuration = 1, unsigned phaseOffset = 0);
		model::Button *createButton(label_t name = "", bool activeOnPress = false);
		model::Digit *createDigit(label_t name = "", bool hasDecimalPoint = true);
		model::Probe *createProbe(label_t name = "");
		model::Splitter *createSplitter(label_t name = "", Width widthIn = Width::Bit2, width_t widthOut = 2);

		Circuit *find(Circuit *circuit) noexcept;
		Circuit *find(Identifier id) noexcept;
		Circuit *find(label_t name) noexcept;
		Circuit *find(const std::string &name) noexcept;

		bool remove(Circuit *circuit);
		bool remove(Identifier id);
		bool remove(label_t name);
		bool remove(const std::string &name);

		LCSMCircuitView addCircuit(const LCSMCircuit &other);

		const LCSMCircuit *findCircuit(Identifier id) noexcept;
		const LCSMCircuit *findCircuit(label_t name) noexcept;
		const LCSMCircuit *findCircuit(const std::string &name) noexcept;

		bool removeCircuit(const LCSMCircuit *circuit);
		bool removeCircuit(Identifier id);
		bool removeCircuit(label_t name);
		bool removeCircuit(const std::string &name);

		model::Wire *connect(Circuit *circuit1, portid_t port1, Circuit *circuit2, portid_t port2, label_t name = "");
		model::Wire *connect(Circuit *circuit1, Circuit *circuit2, portid_t port2, label_t name = "");
		model::Wire *connect(Circuit *circuit1, portid_t port1, Circuit *circuit2, label_t name = "");
		model::Wire *connect(Circuit *circuit1, Circuit *circuit2, label_t name = "");

	  private:
		Identifier m_globalId;

		std::map< Identifier, std::shared_ptr< Circuit > > m_components;

		std::map< Identifier, std::shared_ptr< Circuit > > m_inputs;
		std::map< Identifier, std::shared_ptr< Circuit > > m_outputs;

		std::map< Identifier, std::shared_ptr< Circuit > > m_componentWires;
		std::map< Identifier, std::shared_ptr< Circuit > > m_connectorWires;

		std::unordered_map< Identifier, std::shared_ptr< LCSMCircuit > > m_circuits;

		std::string m_name;

	  private:
		Circuit *registerElement(std::shared_ptr< Circuit > &&circuit);

		void copyImpl(LCSMCircuit *newCircuit, const Identifier &entryId) const;
		LCSMCircuit copyImpl(const Identifier &entryId) const;

		std::shared_ptr< model::Wire > createHeadlessWire(label_t name = "");
		std::shared_ptr< model::Wire > createIdentifiedWire(label_t name = "");
		std::shared_ptr< model::Wire > createConnectorWire(label_t name = "");
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMCIRCUIT_H */
