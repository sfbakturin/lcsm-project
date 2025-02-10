#ifndef LCSM_MODEL_STD_CONSTANT_H
#define LCSM_MODEL_STD_CONSTANT_H

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
		class Constant : public Circuit
		{
		  public:
			enum Port : portid_t
			{
				Wiring
			};

		  public:
			Constant(Width width, value_t value);

			Constant(const Constant &other);
			Constant(Constant &&other) noexcept;

			Constant &operator=(const Constant &other);
			Constant &operator=(Constant &&other) noexcept;

			void swap(Constant &other) noexcept;

			Width width() const noexcept;
			void setWidth(Width newWidth) noexcept;

			value_t value() const noexcept;
			void setValue(value_t newValue) noexcept;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
			void connect(const support::PointerView< Circuit > &circuit);

			virtual Circuit *byPort(portid_t portId) override final;

			const Wire &wire() const noexcept;

		  private:
			Identifier m_id;
			Width m_width;
			value_t m_value;
			Wire m_wire;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_CONSTANT_H */
