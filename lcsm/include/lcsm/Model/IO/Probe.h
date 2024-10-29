#ifndef LCSM_MODEL_IO_PROBE_H
#define LCSM_MODEL_IO_PROBE_H

#include <lcsm/Component/IOComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/Wiring/Wire.h>

namespace lcsm
{
	namespace model
	{
		class Probe : public IOComponent
		{
		  public:
			Probe();

			virtual Identifier ID() const noexcept override final;
			virtual Identifier identify(Identifier ID) noexcept override final;

			virtual void connectIn(wire_t &wire, std::size_t i) override final;
			virtual void connectOut(wire_t &wire, std::size_t i) override final;

			virtual IOComponentType ioComponentType() const noexcept final;

		  private:
			Identifier m_id;

			Wire m_wireIn;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_IO_PROBE_H */
