#ifndef SIM_MODEL_CIRCUIT_TRANSMISSIONGATE_H
#define SIM_MODEL_CIRCUIT_TRANSMISSIONGATE_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Model/Wiring/Wire.h>

#include <array>

namespace sim
{
	class Pin;

	class TransmissionGate : public CircuitComponent
	{
	  public:
		TransmissionGate();

		TransmissionGate(const TransmissionGate &other);
		TransmissionGate(TransmissionGate &&other) noexcept;

		TransmissionGate &operator=(const TransmissionGate &other) = default;
		TransmissionGate &operator=(TransmissionGate &&other) noexcept = default;

		virtual unsigned ID() const noexcept override;
		virtual void Identify(unsigned ID) noexcept override;

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

		unsigned m_id;

		sim::Wire m_base;
		std::array< sim::Wire, SRC_N > m_srcs;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_TRANSMISSIONGATE_H */
