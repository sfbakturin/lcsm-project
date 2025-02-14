#ifndef LCSM_MODEL_STD_SPLITTER_H
#define LCSM_MODEL_STD_SPLITTER_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/StaticArray.hpp>

namespace lcsm
{
	namespace model
	{
		class Splitter : public Circuit
		{
		  public:
			enum Port
			{
				Out0,
				Out1,
				Out2,
				Out3,
				Out4,
				Out5,
				Out6,
				Out7,
				Out8,
				Out9,
				Out10,
				Out11,
				Out12,
				Out13,
				Out14,
				Out15,
				Out16,
				Out17,
				Out18,
				Out19,
				Out20,
				Out21,
				Out22,
				Out23,
				Out24,
				Out25,
				Out26,
				Out27,
				Out28,
				Out29,
				Out30,
				Out31,
				Out32,
				Out33,
				Out34,
				Out35,
				Out36,
				Out37,
				Out38,
				Out39,
				Out40,
				Out41,
				Out42,
				Out43,
				Out44,
				Out45,
				Out46,
				Out47,
				Out48,
				Out49,
				Out50,
				Out51,
				Out52,
				Out53,
				Out54,
				Out55,
				Out56,
				Out57,
				Out58,
				Out59,
				Out60,
				Out61,
				Out62,
				Out63,
				Input
			};

		  public:
			Splitter(Width widthIn, width_t widthOut);

			Width widthIn() const noexcept;
			void setWidthIn(Width widthIn) noexcept;

			width_t widthOut() const noexcept;
			void setWidthOut(width_t widthOut);

			width_bitmask_t bitsOut(portid_t portId) const noexcept;

			const Wire &wireIn() const noexcept;
			const Wire &wireOut(portid_t portId) const noexcept;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;

			virtual Circuit *byPort(portid_t portId) override final;

		  private:
			Identifier m_id;
			Width m_widthIn;
			width_t m_widthOut;
			Wire m_wireIn;
			support::StaticArray< Wire, Width::LastWidth > m_wireOut;
			support::StaticArray< width_bitmask_t, Width::LastWidth > m_bitsOut;

		  private:
			void resetBitmasks() noexcept;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_SPLITTER_H */
