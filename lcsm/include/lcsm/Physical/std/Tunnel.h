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
#include <lcsm/lcsmconfig.h>

#include <deque>
#include <vector>

namespace lcsm
{
	namespace physical
	{
		class LCSM_API Tunnel : public EvaluatorNode
		{
		  public:
			Tunnel(object_type_t objectType);

			virtual NodeType nodeType() const noexcept override final;

			virtual std::size_t contextSize() const noexcept override final;
			virtual std::size_t privateContextSize() const noexcept override final;

			virtual void setContext(const support::PointerView< Context > &context) override final;
			virtual void resetContext() noexcept override final;
			virtual void verifyContext() override final;

			virtual void add(Instruction &&instruction) override final;

			virtual void invoke(const Timestamp &now, std::deque< Event > &events) override final;

			void connectWiring(const support::PointerView< EvaluatorNode > &node) noexcept;
			void connectTunnel(const support::PointerView< EvaluatorNode > &node);

		  private:
			support::PointerView< Context > m_context;
			support::PointerView< EvaluatorNode > m_wiring;
			std::vector< support::PointerView< EvaluatorNode > > m_tunnels;
			std::deque< Instruction > m_writeValueWiring;
			std::deque< Instruction > m_writeValueTunnel;
			std::deque< Instruction > m_polluteValueWiring;
			std::deque< Instruction > m_polluteValueTunnel;
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_TUNNEL_H */
