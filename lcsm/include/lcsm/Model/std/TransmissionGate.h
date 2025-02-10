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
				Base = 0,
				InoutA = 1,
				InoutB = 2,
				InoutC = 3
			};

		  public:
			TransmissionGate();

			TransmissionGate(const TransmissionGate &other) = default;
			TransmissionGate(TransmissionGate &&other) noexcept = default;

			TransmissionGate &operator=(const TransmissionGate &other);
			TransmissionGate &operator=(TransmissionGate &&other) noexcept;

			void swap(TransmissionGate &other) noexcept;

			Identifier idBase() const noexcept;
			Identifier idInoutA() const noexcept;
			Identifier idInoutB() const noexcept;
			Identifier idInoutC() const noexcept;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
			void connectBase(const support::PointerView< Circuit > &circuit);
			void connectInoutA(const support::PointerView< Circuit > &circuit);
			void connectInoutB(const support::PointerView< Circuit > &circuit);
			void connectInoutC(const support::PointerView< Circuit > &circuit);

			virtual Circuit *byPort(portid_t portId) override final;

		  private:
			Identifier m_id;

			Identifier m_idBase;
			Identifier m_idInoutA;
			Identifier m_idInoutB;
			Identifier m_idInoutC;

			Wire m_base;
			Wire m_inoutA;
			Wire m_inoutB;
			Wire m_inoutC;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_TRANSMISSIONGATE_H */
