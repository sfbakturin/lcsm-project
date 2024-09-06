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

		virtual CircuitComponentType circuitComponentType() const noexcept override;

		virtual Transistor *asTransistor() noexcept override final;
		virtual const Transistor *asTransistor() const noexcept override final;

		Wire &wireBase() noexcept;
		const Wire &wireBase() const noexcept;

		Wire &wireSrcA() noexcept;
		const Wire &wireSrcA() const noexcept;

		Wire &wireSrcB() noexcept;
		const Wire &wireSrcB() const noexcept;

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
