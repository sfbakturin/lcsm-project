#ifndef LCSM_MODEL_STD_TRANSISTOR_H
#define LCSM_MODEL_STD_TRANSISTOR_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>

namespace lcsm
{
	namespace model
	{
		class Transistor : public Circuit
		{
		  public:
			enum Type : id_t
			{
				P,
				N
			};

			enum Port : portid_t
			{
				Base,
				InoutA,
				InoutB
			};

		  public:
			Transistor(Type type);

			Transistor(const Transistor &other);
			Transistor(Transistor &&other) noexcept;

			Transistor &operator=(const Transistor &other);
			Transistor &operator=(Transistor &&other);

			void swap(Transistor &other) noexcept;

			Identifier idBase() const noexcept;
			Identifier idInoutA() const noexcept;
			Identifier idInoutB() const noexcept;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
			void connectBase(const support::PointerView< Circuit > &circuit);
			void connectInoutA(const support::PointerView< Circuit > &circuit);
			void connectInoutB(const support::PointerView< Circuit > &circuit);

		  private:
			Identifier m_id;

			Identifier m_idBase;
			Identifier m_idInoutA;
			Identifier m_idInoutB;

			Type m_type;

			Wire m_base;
			Wire m_inoutA;
			Wire m_inoutB;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_TRANSISTOR_H */
