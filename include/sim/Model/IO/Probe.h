#ifndef SIM_MODEL_IO_PROBE_H
#define SIM_MODEL_IO_PROBE_H

#include <sim/Component/IOComponent.h>
#include <sim/Component/Identifier.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
{
	class Probe : public IOComponent
	{
	  public:
		Probe() = default;

		virtual Identifier ID() const noexcept override final;
		virtual Identifier identify(Identifier ID) noexcept override final;

		virtual void connectIn(wire_t &wire, std::size_t i) override final;
		virtual void connectOut(wire_t &wire, std::size_t i) override final;

	  private:
		Identifier m_id;

		Wire m_wireIn;
	};
}	 // namespace sim

#endif /* SIM_MODEL_IO_PROBE_H */
