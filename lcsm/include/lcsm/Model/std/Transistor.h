#ifndef LCSM_MODEL_STD_TRANSISTOR_H
#define LCSM_MODEL_STD_TRANSISTOR_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <vector>

namespace lcsm
{
	namespace model
	{
		class Transistor : public Circuit
		{
		  public:
			enum Type : unsigned
			{
				P,
				N
			};

			enum Port : portid_t
			{
				Base,
				SrcA,
				SrcB
			};

		  public:
			Transistor(Type type);

			Type type() const noexcept;
			void setType(Type type) noexcept;

			const Wire *wireBase() const noexcept;
			const Wire *wireSrcA() const noexcept;
			const Wire *wireSrcB() const noexcept;

			virtual std::size_t numOfWires() const noexcept override final;
			virtual void provideWires(const std::vector< std::shared_ptr< model::Wire > > &wires) override final;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual object_type_t objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, Circuit *circuit) override final;
			virtual void disconnect(Circuit *circuit) override final;
			virtual void disconnectAll() override final;

			void connectBase(Circuit *circuit);
			void connectSrcA(Circuit *circuit);
			void connectSrcB(Circuit *circuit);

			virtual Circuit *byPort(portid_t portId) noexcept override final;

		  private:
			Identifier m_id;

			Type m_type;

			std::shared_ptr< Wire > m_base;
			std::shared_ptr< Wire > m_srcA;
			std::shared_ptr< Wire > m_srcB;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_TRANSISTOR_H */
