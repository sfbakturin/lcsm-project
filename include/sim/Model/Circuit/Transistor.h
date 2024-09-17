#ifndef SIM_MODEL_CIRCUIT_TRANSISTOR_H
#define SIM_MODEL_CIRCUIT_TRANSISTOR_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Identifier.h>
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

		virtual Identifier ID() const noexcept override final;
		virtual Identifier identify(Identifier ID) noexcept override final;

		Identifier idBase() const noexcept;
		Identifier idSrcA() const noexcept;
		Identifier idSrcB() const noexcept;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

		void ConnectBase(wire_t &wire);
		void ConnectSrcA(wire_t &wire);
		void ConnectSrcB(wire_t &wire);

		bool testConnectivityBase(const Wire *wire) const noexcept;
		bool testConnectivitySrcA(const Wire *wire) const noexcept;
		bool testConnectivitySrcB(const Wire *wire) const noexcept;

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

		Identifier m_id;
		Identifier m_idBase;
		Identifier m_idSrcA;
		Identifier m_idSrcB;

		TransistorType m_type;
		Wire m_base;
		std::array< Wire, SRC_N > m_srcs;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_TRANSISTOR_H */
