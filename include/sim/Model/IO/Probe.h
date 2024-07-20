#ifndef SIM_MODEL_IO_PROBE_H
#define SIM_MODEL_IO_PROBE_H

#include <sim/Component/IOComponent.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
{
	class Probe : public IOComponent
	{
	  public:
		Probe() noexcept = default;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

	  private:
		wire_t m_wireIn;
	};
}	 // namespace sim

#endif /* SIM_MODEL_IO_PROBE_H */
