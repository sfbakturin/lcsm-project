#ifndef LCSM_MODEL_STD_CLOCK_H
#define LCSM_MODEL_STD_CLOCK_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>

#include <memory>
#include <vector>

namespace lcsm
{
	namespace model
	{
		class Clock : public Circuit
		{
		  public:
			enum Port : portid_t
			{
				Wiring
			};

		  public:
			Clock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);

			unsigned highDuration() const noexcept;
			void setHighDuration(unsigned highDuration) noexcept;

			unsigned lowDuration() const noexcept;
			void setLowDuration(unsigned lowDuration) noexcept;

			unsigned phaseOffset() const noexcept;
			void setPhaseOffset(unsigned phaseOffset) noexcept;

			const Wire *wire() const noexcept;

			virtual std::size_t numOfWires() const noexcept override final;
			virtual void provideWires(const std::vector< std::shared_ptr< model::Wire > > &wires) override final;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual object_type_t objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, Circuit *circuit) override final;
			virtual void disconnect(Circuit *circuit) override final;
			virtual void disconnectAll() override final;

			void connect(Circuit *circuit);

			virtual Circuit *byPort(portid_t portId) noexcept override final;

		  private:
			Identifier m_id;

			unsigned m_highDuration;
			unsigned m_lowDuration;
			unsigned m_phaseOffset;

			std::shared_ptr< Wire > m_wire;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_CLOCK_H */
