#ifndef LCSM_MODEL_STD_GROUND_H
#define LCSM_MODEL_STD_GROUND_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>

namespace lcsm
{
	namespace model
	{
		class Ground : public Circuit
		{
		  public:
			enum Port : portid_t
			{
				Wiring
			};

		  public:
			Ground(Width width);

			Width width() const noexcept;
			void setWidth(Width width) noexcept;

			const Wire &wire() const noexcept;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
			void connect(const support::PointerView< Circuit > &circuit);

			virtual Circuit *byPort(portid_t portId) override final;

		  private:
			Identifier m_id;

			Width m_width;

			Wire m_wire;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_GROUND_H */
