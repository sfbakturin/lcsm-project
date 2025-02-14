#ifndef LCSM_MODEL_STD_TRANSISTOR_H
#define LCSM_MODEL_STD_TRANSISTOR_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>

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

			const Wire &wireBase() const noexcept;
			const Wire &wireSrcA() const noexcept;
			const Wire &wireSrcB() const noexcept;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
			void connectBase(const support::PointerView< Circuit > &circuit);
			void connectSrcA(const support::PointerView< Circuit > &circuit);
			void connectSrcB(const support::PointerView< Circuit > &circuit);

			virtual Circuit *byPort(portid_t portId) override final;

		  private:
			Identifier m_id;

			Type m_type;

			Wire m_base;
			Wire m_srcA;
			Wire m_srcB;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_TRANSISTOR_H */
