#ifndef SIM_MODEL_IO_PROBE_H
#define SIM_MODEL_IO_PROBE_H

#include <sim/Component/IOComponent.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
{
	class Probe : public IOComponent
	{
	  public:
		Probe();

		virtual unsigned ID() const noexcept override;
		virtual void Identify(unsigned ID) noexcept override;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

	  private:
		unsigned m_id;

		sim::Wire m_wireIn;
	};
}	 // namespace sim

#endif /* SIM_MODEL_IO_PROBE_H */
