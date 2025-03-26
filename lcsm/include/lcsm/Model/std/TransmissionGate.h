#ifndef LCSM_MODEL_STD_TRANSMISSIONGATE_H
#define LCSM_MODEL_STD_TRANSMISSIONGATE_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
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
		class LCSM_API TransmissionGate : public Circuit
		{
		  public:
			enum Port : portid_t
			{
				Base,
				SrcA,
				SrcB,
				SrcC
			};

		  public:
			TransmissionGate() = default;
			TransmissionGate(label_t name);
			~TransmissionGate() noexcept;

			const Wire *wireBase() const noexcept;
			const Wire *wireSrcA() const noexcept;
			const Wire *wireSrcB() const noexcept;
			const Wire *wireSrcC() const noexcept;

			virtual std::size_t numOfWires() const noexcept override final;
			virtual void provideWires(const std::vector< std::shared_ptr< model::Wire > > &wires) override final;

			virtual Identifier id() const noexcept override final;
			virtual Identifier identify(Identifier id) noexcept override final;

			virtual object_type_t objectType() const noexcept override final;
			virtual CircuitType circuitType() const noexcept override final;

			virtual void connect(portid_t portId, Circuit *circuit) override final;

			virtual void disconnect(Circuit *circuit) noexcept override final;
			virtual void disconnectAll() noexcept override final;

			void connectBase(Circuit *circuit);
			void connectSrcA(Circuit *circuit);
			void connectSrcB(Circuit *circuit);
			void connectSrcC(Circuit *circuit);

			virtual Circuit *byPort(portid_t portId) noexcept override final;
			virtual portid_t findPort(const Circuit *circuit) const noexcept override final;

			virtual portid_t defaultPort() const noexcept override final;

			virtual void dump(LCSMFileWriter &writer, LCSMBuilder &builder) const override final;
			virtual void copy(Circuit *circuit, LCSMBuilder &builder) const override final;
			virtual void from(LCSMFileReader &reader, LCSMBuilder &builder) override final;

		  private:
			Identifier m_id;
			std::shared_ptr< Wire > m_base;
			std::shared_ptr< Wire > m_srcA;
			std::shared_ptr< Wire > m_srcB;
			std::shared_ptr< Wire > m_srcC;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_TRANSMISSIONGATE_H */
