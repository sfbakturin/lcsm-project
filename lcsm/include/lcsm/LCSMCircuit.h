#ifndef LCSM_LCSMCIRCUIT_H
#define LCSM_LCSMCIRCUIT_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Verilog.h>
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
#include <lcsm/Verilog/Module.h>
#include <lcsm/lcsmconfig.h>
#include <unordered_map>

#include <cstddef>
#include <map>
#include <memory>
#include <string>

namespace lcsm
{
	class LCSM_API LCSMCircuit;

	class LCSM_API LCSMCircuitView
	{
	  public:
		LCSMCircuitView() noexcept;
		LCSMCircuitView(LCSMCircuit *circuit) noexcept;

		LCSMCircuitView(const LCSMCircuitView &other) noexcept;
		LCSMCircuitView(LCSMCircuitView &&other) noexcept;

		LCSMCircuitView &operator=(const LCSMCircuitView &other) noexcept;
		LCSMCircuitView &operator=(LCSMCircuitView &&other) noexcept;

		void swap(LCSMCircuitView &other) noexcept;

		bool present() const noexcept;
		const LCSMCircuit *get() const noexcept;

		Identifier globalId() const noexcept;
		const std::string &name() const noexcept;
		void setName(label_t name);
		void setName(const std::string &name);

		const std::map< Identifier, std::shared_ptr< Circuit > > &components() const noexcept;
		const std::map< Identifier, std::shared_ptr< Circuit > > &inputs() const noexcept;
		const std::map< Identifier, std::shared_ptr< Circuit > > &outputs() const noexcept;

		Circuit *find(Circuit *circuit) noexcept;
		Circuit *find(Identifier id) noexcept;
		Circuit *find(label_t name) noexcept;
		Circuit *find(const std::string &name) noexcept;

		Circuit *findInput(Circuit *circuit) noexcept;
		Circuit *findInput(Identifier id) noexcept;
		Circuit *findInput(label_t name) noexcept;
		Circuit *findInput(const std::string &name) noexcept;

		Circuit *findOutput(Circuit *circuit) noexcept;
		Circuit *findOutput(Identifier id) noexcept;
		Circuit *findOutput(label_t name) noexcept;
		Circuit *findOutput(const std::string &name) noexcept;

	  private:
		LCSMCircuit *m_circuit;
	};

	class LCSM_API LCSMCircuit
	{
	  public:
		LCSMCircuit() = default;
		LCSMCircuit(label_t name);
		LCSMCircuit(const std::string &name);

		LCSMCircuit(const LCSMCircuit &other);
		LCSMCircuit(LCSMCircuit &&other) noexcept;

		LCSMCircuit &operator=(const LCSMCircuit &other);
		LCSMCircuit &operator=(LCSMCircuit &&other) noexcept;

		void swap(LCSMCircuit &other) noexcept;

		LCSMCircuit copy() const;
		Identifier globalId() const noexcept;
		const std::string &name() const noexcept;
		void setName(label_t name);
		void setName(const std::string &name);

		const std::map< Identifier, std::shared_ptr< Circuit > > &components() const noexcept;
		const std::map< Identifier, std::shared_ptr< Circuit > > &inputs() const noexcept;
		const std::map< Identifier, std::shared_ptr< Circuit > > &outputs() const noexcept;

		Circuit *create(Circuit *circuit);
		model::Constant *createConstant(label_t name = "", Width width = Width::Bit1, value_t value = 0x1);
		model::Ground *createGround(label_t name = "", Width width = Width::Bit1);
		model::Power *createPower(label_t name = "", Width width = Width::Bit1);
		model::Pin *createPin(bool output, label_t name = "", Width width = Width::Bit1);
		model::Transistor *createTransistor(label_t name = "", model::Transistor::Type type = model::Transistor::Type::P);
		model::TransmissionGate *createTransmissionGate(label_t name = "");
		model::Tunnel *createTunnel(label_t name = "");
		model::Clock *createClock(label_t name = "", unsigned highDuration = 1, unsigned lowDuration = 1, unsigned phaseOffset = 0);
		model::Button *createButton(label_t name = "", bool activeOnPress = true);
		model::Digit *createDigit(label_t name = "", bool hasDecimalPoint = true);
		model::Probe *createProbe(label_t name = "");
		model::Splitter *createSplitter(label_t name = "", Width widthIn = Width::Bit2, width_t widthOut = 2);

		model::VerilogModule *createVerilogModule(const verilog::Module &module);
		model::VerilogModule *createVerilogModule(verilog::Module &&module);

		Circuit *find(Circuit *circuit) noexcept;
		Circuit *find(Identifier id) noexcept;
		Circuit *find(label_t name) noexcept;
		Circuit *find(const std::string &name) noexcept;

		Circuit *findInput(Circuit *circuit) noexcept;
		Circuit *findInput(Identifier id) noexcept;
		Circuit *findInput(label_t name) noexcept;
		Circuit *findInput(const std::string &name) noexcept;

		Circuit *findOutput(Circuit *circuit) noexcept;
		Circuit *findOutput(Identifier id) noexcept;
		Circuit *findOutput(label_t name) noexcept;
		Circuit *findOutput(const std::string &name) noexcept;

		bool remove(Circuit *circuit);
		bool remove(Identifier id);
		bool remove(label_t name);
		bool remove(const std::string &name);

		LCSMCircuitView addCircuit(const LCSMCircuit &other);

		LCSMCircuitView findCircuit(const LCSMCircuitView &circuit) noexcept;
		LCSMCircuitView findCircuit(Identifier id) noexcept;
		LCSMCircuitView findCircuit(label_t name) noexcept;
		LCSMCircuitView findCircuit(const std::string &name) noexcept;

		bool removeCircuit(const LCSMCircuitView &circuit);
		bool removeCircuit(Identifier id);
		bool removeCircuit(label_t name);
		bool removeCircuit(const std::string &name);

		model::Wire *connect(Circuit *circuit1, portid_t port1, Circuit *circuit2, portid_t port2);
		model::Wire *connect(Circuit *circuit1, Circuit *circuit2, portid_t port2);
		model::Wire *connect(Circuit *circuit1, portid_t port1, Circuit *circuit2);
		model::Wire *connect(Circuit *circuit1, Circuit *circuit2);

		std::string dumpToString() const;
		void dumpToFile(const std::string &filename) const;
		void dumpToFile(const char *filename) const;

		static LCSMCircuit fromString(const std::string &string);
		static LCSMCircuit fromString(const char *string);
		static LCSMCircuit fromFile(const std::string &filename);
		static LCSMCircuit fromFile(const char *filename);

	  private:
		// God, I swear WE NEED THIS.
		// FIXME: Somehow this need to be dead...
		friend class model::LCSMBuilder;

		Identifier m_globalId;

		std::map< Identifier, std::shared_ptr< Circuit > > m_components;

		std::map< Identifier, std::shared_ptr< Circuit > > m_inputs;
		std::map< Identifier, std::shared_ptr< Circuit > > m_outputs;

		std::map< Identifier, std::shared_ptr< Circuit > > m_componentWires;
		std::map< Identifier, std::shared_ptr< Circuit > > m_connectorWires;

		std::unordered_map< Identifier, std::shared_ptr< LCSMCircuit > > m_circuits;
		std::unordered_map< Identifier, std::shared_ptr< verilog::Module > > m_verilogModules;

		std::string m_name;

	  private:
		LCSMCircuitView addCircuit(const LCSMCircuit &other, model::LCSMBuilder &builder, std::size_t depth = 0);

		void copyImpl(LCSMCircuit *newCircuit, model::LCSMBuilder &builder, const Identifier &entryId, std::size_t depth = 0) const;
		LCSMCircuit copyImpl(const Identifier &entryId) const;

		void dumpImpl(model::LCSMFileWriter &writer) const;
		static LCSMCircuit fromImpl(model::LCSMFileReader &reader, model::LCSMBuilder &builder, std::size_t depth = 0);

		std::shared_ptr< model::Wire > createHeadlessWire(label_t name = "");
		std::shared_ptr< model::Wire > createIdentifiedWire(label_t name = "");
		std::shared_ptr< model::Wire > createConnectorWire(label_t name = "");

		Circuit *registerComponent(std::shared_ptr< Circuit > &circuit);

		std::shared_ptr< Circuit > preRegisterComponent(Circuit *circuit);
		std::shared_ptr< Circuit > preRegisterComponent(std::shared_ptr< Circuit > &circuit);

		Circuit *postRegisterComponent(Circuit *circuit);
		Circuit *postRegisterComponent(std::shared_ptr< Circuit > &circuit);

		Circuit *findGloballyComponentWire(Identifier id) noexcept;
	};
}	 // namespace lcsm

#endif /* LCSM_LCSMCIRCUIT_H */
