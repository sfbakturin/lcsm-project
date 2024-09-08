#ifndef SIM_MODEL_CIRCUIT_TRANSMISSIONGATE_H
#define SIM_MODEL_CIRCUIT_TRANSMISSIONGATE_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Identifier.h>
#include <sim/Model/Wiring/Wire.h>

#include <array>

namespace sim
{
	class Pin;

	class TransmissionGate : public CircuitComponent
	{
	  public:
		TransmissionGate() = default;

		TransmissionGate(const TransmissionGate &other);
		TransmissionGate(TransmissionGate &&other) noexcept;

		TransmissionGate &operator=(const TransmissionGate &other) = default;
		TransmissionGate &operator=(TransmissionGate &&other) noexcept = default;

		virtual Identifier ID() const noexcept override final;
		virtual Identifier identify(Identifier ID) noexcept override final;

		Identifier idBase() const noexcept;
		Identifier idSrcA() const noexcept;
		Identifier idSrcB() const noexcept;
		Identifier idSrcC() const noexcept;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

		void ConnectBase(wire_t &wire);
		void ConnectSrcA(wire_t &wire);
		void ConnectSrcB(wire_t &wire);
		void ConnectSrcC(wire_t &wire);

		virtual CircuitComponentType circuitComponentType() const noexcept override;

	  private:
		static constexpr std::size_t SRC_N = 3;

		void Connect(wire_t &wire, std::size_t i);

		Identifier m_id;
		Identifier m_idBase;
		Identifier m_idSrcA;
		Identifier m_idSrcB;
		Identifier m_idSrcC;

		Wire m_base;
		std::array< Wire, SRC_N > m_srcs;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_TRANSMISSIONGATE_H */
