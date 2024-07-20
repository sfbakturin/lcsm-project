#ifndef SIM_MODEL_CIRCUIT_TRANSMISSIONGATE_H
#define SIM_MODEL_CIRCUIT_TRANSMISSIONGATE_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Model/Wiring/Wire.h>

#include <array>

namespace sim
{
	class TransmissionGate : public CircuitComponent
	{
	  public:
		TransmissionGate() = default;

		TransmissionGate(const TransmissionGate &other);
		TransmissionGate(TransmissionGate &&other) noexcept;

		TransmissionGate &operator=(const TransmissionGate &other) = default;
		TransmissionGate &operator=(TransmissionGate &&other) noexcept = default;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

		void ConnectBase(const wire_t &wire);
		void ConnectSrcA(const wire_t &wire);
		void ConnectSrcB(const wire_t &wire);
		void ConnectSrcC(const wire_t &wire);

	  private:
		static constexpr std::size_t SRC_N = 3;

		void Connect(const wire_t &wire, std::size_t i);

		sim::Wire m_base;
		std::array< sim::Wire, SRC_N > m_srcs;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_TRANSMISSIONGATE_H */
