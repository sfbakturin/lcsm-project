#ifndef LCSM_MODEL_STD_DIGIT_H
#define LCSM_MODEL_STD_DIGIT_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/lcsmconfig.h>

#include <memory>

namespace lcsm
{
	namespace model
	{
		class LCSM_API Digit : public Component
		{
		  public:
			enum Port : portid_t
			{
				WiringData,
				WiringDecimalPoint
			};

		  public:
			Digit() = default;
			Digit(bool hasDecimalPoint);
			Digit(label_t name, bool hasDecimalPoint);
			~Digit() noexcept;

			bool hasDecimalPoint() const noexcept;
			void setHasDecimalPoint(bool hasDecimalPoint) noexcept;

			const Wire *wireData() const noexcept;
			const Wire *wireDecimalPoint() const noexcept;

			virtual std::size_t numOfWires() const noexcept override final;
			virtual void provideWires(const std::vector< std::shared_ptr< model::Wire > > &wires) override final;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual object_type_t objectType() const noexcept override final;
			virtual ComponentType componentType() const noexcept override final;

			virtual void connect(portid_t portId, Component *circuit) override final;

			virtual void disconnect(Component *circuit) noexcept override final;
			virtual void disconnectAll() noexcept override final;

			void connectData(Component *circuit);
			void connectDecimalPoint(Component *circuit);

			virtual Component *byPort(portid_t portId) noexcept override final;
			virtual portid_t findPort(const Component *circuit) const noexcept override final;

			virtual portid_t defaultPort() const noexcept override final;

			virtual void dump(LCSMFileWriter &writer, LCSMBuilder &builder) const override final;
			virtual void copy(Component *circuit, LCSMBuilder &builder) const override final;
			virtual void from(LCSMFileReader &reader, LCSMBuilder &builder) override final;

		  private:
			Identifier m_id;
			bool m_hasDecimalPoint;
			std::shared_ptr< Wire > m_wireData;
			std::shared_ptr< Wire > m_wireDecimalPoint;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_DIGIT_H */
