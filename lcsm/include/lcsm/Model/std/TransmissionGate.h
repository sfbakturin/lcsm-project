#ifndef LCSM_MODEL_STD_TRANSMISSIONGATE_H
#define LCSM_MODEL_STD_TRANSMISSIONGATE_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>

namespace lcsm
{
	namespace model
	{
		class TransmissionGate : public Circuit
		{
		  public:
			enum Port : portid_t
			{
				Base,
				SrcA,
				SrcB,
				SrcC
			};

		  public:
			TransmissionGate();

			const Wire &wireBase() const noexcept;
			const Wire &wireSrcA() const noexcept;
			const Wire &wireSrcB() const noexcept;
			const Wire &wireSrcC() const noexcept;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
			void connectBase(const support::PointerView< Circuit > &circuit);
			void connectSrcA(const support::PointerView< Circuit > &circuit);
			void connectSrcB(const support::PointerView< Circuit > &circuit);
			void connectSrcC(const support::PointerView< Circuit > &circuit);

			virtual Circuit *byPort(portid_t portId) override final;

		  private:
			Identifier m_id;
			Wire m_base;
			Wire m_srcA;
			Wire m_srcB;
			Wire m_srcC;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_TRANSMISSIONGATE_H */
