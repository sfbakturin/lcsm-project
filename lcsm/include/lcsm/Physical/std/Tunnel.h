#ifndef LCSM_PHYSICAL_STD_TUNNEL_H
#define LCSM_PHYSICAL_STD_TUNNEL_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Transistor.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Support/PointerView.hpp>

#include <deque>
#include <vector>

namespace lcsm
{
	namespace physical
	{
		class Tunnel : public EvaluatorNode
		{
		  public:
			Tunnel(object_type_t objectType);

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;
			virtual std::size_t privateContextSize() const noexcept override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;
			virtual void verifyContext() override final;

			virtual void addInstant(const Instruction &instruction) override final;
			virtual void addInstant(Instruction &&instruction) override final;

			virtual std::vector< Event > invokeInstants(const Timestamp &now) override final;

			void connectWiring(const support::PointerView< EvaluatorNode > &node);
			void connectTunnel(const support::PointerView< EvaluatorNode > &node);

		  private:
			support::PointerView< Context > m_context;
			support::PointerView< EvaluatorNode > m_wiring;
			std::vector< support::PointerView< EvaluatorNode > > m_tunnels;
			std::deque< Instruction > m_instantsWiring;
			std::deque< Instruction > m_instantsTunnel;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_TUNNEL_H */
