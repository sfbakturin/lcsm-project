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

	class Pin;

	class Transistor : public CircuitComponent
	{
	  public:
		Transistor(TransistorType type);

		Transistor(const Transistor &other);
		Transistor(Transistor &&other) noexcept;

		Transistor &operator=(const Transistor &other);
		Transistor &operator=(Transistor &&other);

		void Swap(Transistor &other) noexcept;

		virtual unsigned ID() const noexcept override;
		virtual void Identify(unsigned ID) noexcept override;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

		void ConnectBase(wire_t &wire);
		void ConnectSrcA(wire_t &wire);
		void ConnectSrcB(wire_t &wire);

		virtual const Pin *AsPin() const noexcept override;
		virtual Pin *AsPin() noexcept override;

	  private:
		static constexpr std::size_t SRC_N = 2;

		void Connect(wire_t &wire, std::size_t i);

		unsigned m_id;

		TransistorType m_type;
		sim::Wire m_base;
		std::array< sim::Wire, SRC_N > m_srcs;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_TRANSISTOR_H */
