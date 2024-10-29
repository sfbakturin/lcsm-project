#ifndef LCSM_MODEL_CIRCUIT_TRANSMISSIONGATE_H
#define LCSM_MODEL_CIRCUIT_TRANSMISSIONGATE_H

#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/Wiring/Wire.h>

namespace lcsm
{
	namespace model
	{
		class Pin;

		class TransmissionGate : public CircuitComponent
		{
		  public:
			enum CompositeIndex : std::size_t
			{
				BASE = 0,
				INOUT_A = 1,
				INOUT_B = 2,
				INOUT_C = 3
			};

			TransmissionGate();

			TransmissionGate(const TransmissionGate &other);
			TransmissionGate(TransmissionGate &&other) noexcept;

			TransmissionGate &operator=(const TransmissionGate &other) = default;
			TransmissionGate &operator=(TransmissionGate &&other) noexcept = default;

			virtual Identifier ID() const noexcept override final;
			virtual Identifier identify(Identifier ID) noexcept override final;

			Identifier idBase() const noexcept;
			Identifier idInoutA() const noexcept;
			Identifier idInoutB() const noexcept;
			Identifier idInoutC() const noexcept;

			virtual void connectIn(wire_t &wire, std::size_t i) override final;
			virtual void connectOut(wire_t &wire, std::size_t i) override final;

			void connectBase(wire_t &wire);
			void connectInoutA(wire_t &wire);
			void connectInoutB(wire_t &wire);
			void connectInoutC(wire_t &wire);

			virtual CircuitComponentType circuitComponentType() const noexcept override final;

		  private:
			void connect(wire_t &wire, std::size_t i);

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

#endif /* LCSM_MODEL_CIRCUIT_TRANSMISSIONGATE_H */
