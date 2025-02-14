#ifndef LCSM_MODEL_STD_PIN_H
#define LCSM_MODEL_STD_PIN_H

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
		class Pin : public Circuit
		{
		  public:
			enum Port : portid_t
			{
				Internal,
				External
			};

		  public:
			Pin(bool output, Width width);

			bool output() const noexcept;
			void setOutput(bool output) noexcept;

			Width width() const noexcept;
			void setWidth(Width width) noexcept;

			const Wire &internal() const noexcept;
			const Wire &external() const noexcept;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual ObjectType objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
			void connectInternal(const support::PointerView< Circuit > &circuit);
			void connectExternal(const support::PointerView< Circuit > &circuit);

			virtual Circuit *byPort(portid_t portId) override final;

		  private:
			Identifier m_id;
			bool m_output;
			Width m_width;
			Wire m_internal;
			Wire m_external;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_PIN_H */
