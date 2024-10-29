#ifndef LCSM_MODEL_CIRCUIT_TRANSISTOR_H
#define LCSM_MODEL_CIRCUIT_TRANSISTOR_H

#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/Wiring/Wire.h>

namespace lcsm
{
	namespace model
	{
		enum TransistorType
		{
			TRANSISTOR_TYPE_P,
			TRANSISTOR_TYPE_N
		};

		class Pin;

		class Transistor : public CircuitComponent
		{
		  public:
			enum CompositeIndex : std::size_t
			{
				BASE = 0,
				INOUT_A = 1,
				INOUT_B = 2
			};

			Transistor(TransistorType type);

			Transistor(const Transistor &other);
			Transistor(Transistor &&other) noexcept;

			Transistor &operator=(const Transistor &other);
			Transistor &operator=(Transistor &&other);

			void swap(Transistor &other) noexcept;

			virtual Identifier ID() const noexcept override final;
			virtual Identifier identify(Identifier ID) noexcept override final;

			Identifier idBase() const noexcept;
			Identifier idInoutA() const noexcept;
			Identifier idInoutB() const noexcept;

			virtual void connectIn(wire_t &wire, std::size_t i) override final;
			virtual void connectOut(wire_t &wire, std::size_t i) override final;

			void connectBase(wire_t &wire);
			void connectInoutA(wire_t &wire);
			void connectInoutB(wire_t &wire);

			std::size_t testConnectivity(const Wire *wire) const noexcept;
			bool testConnectivityBase(const Wire *wire) const noexcept;
			bool testConnectivityInoutA(const Wire *wire) const noexcept;
			bool testConnectivityInoutB(const Wire *wire) const noexcept;

			virtual CircuitComponentType circuitComponentType() const noexcept override final;

			virtual Transistor *asTransistor() noexcept override final;
			virtual const Transistor *asTransistor() const noexcept override final;

			Wire &wireBase() noexcept;
			const Wire &wireBase() const noexcept;

			Wire &wireInoutA() noexcept;
			const Wire &wireInoutA() const noexcept;

			Wire &wireInoutB() noexcept;
			const Wire &wireInoutB() const noexcept;

		  private:
			void connect(wire_t &wire, std::size_t i);

			Identifier m_id;
			Identifier m_idBase;
			Identifier m_idInoutA;
			Identifier m_idInoutB;

			TransistorType m_type;

			Wire m_base;
			Wire m_inoutA;
			Wire m_inoutB;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_CIRCUIT_TRANSISTOR_H */
