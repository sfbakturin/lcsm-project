#ifndef LCSM_MODEL_VERILOG_H
#define LCSM_MODEL_VERILOG_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Module.h>

#include <cstddef>
#include <memory>
#include <vector>

namespace lcsm
{
	namespace model
	{
		class VerilogModule : public Circuit
		{
		  public:
			enum Port : portid_t
			{
				Input,
				Inout,
				Output,
				OutputReg
			};

		  public:
			VerilogModule(const std::shared_ptr< verilog::Module > &module);
			VerilogModule(const std::shared_ptr< verilog::Module > &module, label_t name);
			~VerilogModule() noexcept;

			const Wire *wire(portid_t portId) const noexcept;

			const Wire *input(portid_t portId) const noexcept;
			const Wire *inout(portid_t portId) const noexcept;
			const Wire *output(portid_t portId) const noexcept;
			const Wire *outputReg(portid_t portId) const noexcept;

			std::size_t numOfInputs() const noexcept;
			std::size_t numOfInouts() const noexcept;
			std::size_t numOfOutputs() const noexcept;
			std::size_t numOfOutputRegs() const noexcept;

			const verilog::Module *module() const noexcept;

			virtual std::size_t numOfWires() const noexcept override final;
			virtual void provideWires(const std::vector< std::shared_ptr< model::Wire > > &wires) override final;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual object_type_t objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, Circuit *circuit) override final;

			virtual void disconnect(Circuit *circuit) noexcept override final;
			virtual void disconnectAll() noexcept override final;

			virtual Circuit *byPort(portid_t portId) noexcept override final;
			virtual portid_t findPort(const Circuit *circuit) const noexcept override final;

			virtual portid_t defaultPort() const noexcept override final;

		  private:
			Identifier m_id;

			std::vector< std::shared_ptr< Wire > > m_inputs;
			std::vector< std::shared_ptr< Wire > > m_inouts;
			std::vector< std::shared_ptr< Wire > > m_outputs;
			std::vector< std::shared_ptr< Wire > > m_outputRegs;

			std::shared_ptr< verilog::Module > m_module;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_VERILOG_H */
