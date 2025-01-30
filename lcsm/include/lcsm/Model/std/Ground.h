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

			Ground(const Ground &other);
			Ground(Ground &&other);

			Ground &operator=(const Ground &other);
			Ground &operator=(Ground &&other) noexcept;

			void swap(Ground &other) noexcept;

			Width width() const noexcept;
			void setWidth(Width newWidth) noexcept;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
			void connect(const support::PointerView< Circuit > &circuit);

			const Wire &wire() const noexcept;

		  private:
			Identifier m_id;

			Width m_width;

			Wire m_wire;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_GROUND_H */
