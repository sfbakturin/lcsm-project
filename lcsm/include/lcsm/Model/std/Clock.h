#ifndef LCSM_MODEL_STD_CLOCK_H
#define LCSM_MODEL_STD_CLOCK_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/lcsmconfig.h>

#include <memory>
#include <vector>

namespace lcsm
{
	namespace model
	{
		class LCSM_API Clock : public Component
		{
		  public:
			enum Port : portid_t
			{
				Wiring
			};

		  public:
			Clock() = default;
			Clock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);
			Clock(label_t name, unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);
			~Clock() noexcept;

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
			virtual ComponentType componentType() const noexcept override final;

			virtual void connect(portid_t portId, Component *circuit) override final;

			virtual void disconnect(Component *circuit) noexcept override final;
			virtual void disconnectAll() noexcept override final;

			void connect(Component *circuit);

			virtual Component *byPort(portid_t portId) noexcept override final;
			virtual portid_t findPort(const Component *circuit) const noexcept override final;

			virtual portid_t defaultPort() const noexcept override final;

			virtual void dump(LCSMFileWriter &writer, LCSMBuilder &builder) const override final;
			virtual void copy(Component *circuit, LCSMBuilder &builder) const override final;
			virtual void from(LCSMFileReader &reader, LCSMBuilder &builder) override final;

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
