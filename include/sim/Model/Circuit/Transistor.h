#ifndef SIM_MODEL_CIRCUIT_TRANSISTOR_H
#define SIM_MODEL_CIRCUIT_TRANSISTOR_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Model/Wiring/Wire.h>

#include <array>

namespace sim
{
	enum TransistorType
	{
		P,
		N
	};

	class Transistor : public CircuitComponent
	{
	  public:
		Transistor() noexcept;
		Transistor(TransistorType type) noexcept;

		Transistor(const Transistor &other) noexcept;
		Transistor(Transistor &&other) noexcept;

		Transistor &operator=(const Transistor &other) noexcept;
		Transistor &operator=(Transistor &&other);

		void Swap(Transistor &other) noexcept;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

		void ConnectBase(const wire_t &wire);
		void ConnectSrcA(const wire_t &wire);
		void ConnectSrcB(const wire_t &wire);

	  private:
		static constexpr std::size_t SRC_N = 2;

		void Connect(const wire_t &wire, std::size_t i);

		TransistorType m_type;
		sim::Wire m_base;
		std::array< sim::Wire, SRC_N > m_srcs;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_TRANSISTOR_H */
