#ifndef LCSM_MODEL_STD_BUTTON_H
#define LCSM_MODEL_STD_BUTTON_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/lcsmconfig.h>

#include <cstddef>
#include <memory>
#include <vector>

namespace lcsm
{
	namespace model
	{
		class LCSM_API Button : public Circuit
		{
		  public:
			enum Port : portid_t
			{
				Wiring
			};

		  public:
			Button() = default;
			Button(bool activeOnPress);
			Button(label_t name, bool activeOnPress);
			~Button() noexcept;

			bool activeOnPress() const noexcept;
			void setActiveOnPress(bool activeOnPress) noexcept;

			const Wire *wire() const noexcept;

			virtual std::size_t numOfWires() const noexcept override final;
			virtual void provideWires(const std::vector< std::shared_ptr< Wire > > &wires) override final;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual object_type_t objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, Circuit *circuit) override final;

			virtual void disconnect(Circuit *circuit) noexcept override final;
			virtual void disconnectAll() noexcept override final;

			void connect(Circuit *circuit);

			virtual Circuit *byPort(portid_t portId) noexcept override final;
			virtual portid_t findPort(const Circuit *circuit) const noexcept override final;

			virtual portid_t defaultPort() const noexcept override final;

			virtual void dump(LCSMFileWriter &writer, LCSMBuilder &builder) const override final;
			virtual void copy(Circuit *circuit, LCSMBuilder &builder) const override final;
			virtual void from(LCSMFileReader &reader, LCSMBuilder &builder) override final;

		  private:
			Identifier m_id;
			bool m_activeOnPress;
			std::shared_ptr< Wire > m_wire;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_BUTTON_H */
