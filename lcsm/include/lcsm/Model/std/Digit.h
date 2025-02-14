#ifndef LCSM_MODEL_STD_DIGIT_H
#define LCSM_MODEL_STD_DIGIT_H

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
		class Digit : public Circuit
		{
		  public:
			enum Port : portid_t
			{
				WiringData,
				WiringDecimalPoint
			};

		  public:
			Digit(bool hasDecimalPoint);

			bool hasDecimalPoint() const noexcept;
			void setHasDecimalPoint(bool hasDecimalPoint) noexcept;

			const Wire &wireData() const noexcept;
			const Wire &wireDecimalPoint() const noexcept;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
			void connectData(const support::PointerView< Circuit > &circuit);
			void connectDecimalPoint(const support::PointerView< Circuit > &circuit);

			virtual Circuit *byPort(portid_t portId) override final;

		  private:
			Identifier m_id;
			bool m_hasDecimalPoint;
			Wire m_wireData;
			Wire m_wireDecimalPoint;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_DIGIT_H */
