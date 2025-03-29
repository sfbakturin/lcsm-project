#ifndef LCSM_PHYSICAL_STD_VERILOG_H
#define LCSM_PHYSICAL_STD_VERILOG_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Module.h>
#include <lcsm/lcsmconfig.h>

#include <deque>
#include <utility>
#include <vector>

namespace lcsm
{
	namespace physical
	{
		class LCSM_API VerilogModule : public EvaluatorNode
		{
		  public:
			VerilogModule(const verilog::Module *module, object_type_t objectType);

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;
			virtual std::size_t privateContextSize() const noexcept override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;
			virtual void verifyContext() override final;

			virtual void add(Instruction &&instruction) override final;

			virtual std::vector< Event > invoke(const Timestamp &now) override final;

			void connectInput(const support::PointerView< EvaluatorNode > &input);
			void connectInout(const support::PointerView< EvaluatorNode > &inout);
			void connectOutput(const support::PointerView< EvaluatorNode > &output);

		  private:
			std::deque< std::pair< portid_t, Instruction > > m_inputsInstants;
			std::deque< std::pair< portid_t, Instruction > > m_inoutsInstants;

			std::vector< support::PointerView< EvaluatorNode > > m_inputs;
			std::vector< support::PointerView< EvaluatorNode > > m_inouts;
			std::vector< support::PointerView< EvaluatorNode > > m_outputs;

			support::PointerView< const verilog::Module > m_module;
			support::PointerView< Context > m_context;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_VERILOG_H */
