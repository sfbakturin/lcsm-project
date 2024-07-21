#ifndef SIM_MODEL_CIRCUIT_CLOCK_H
#define SIM_MODEL_CIRCUIT_CLOCK_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Model/Wiring/Wire.h>

#include <cstddef>

namespace sim
{
	class Clock : public CircuitComponent
	{
	  public:
		Clock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);

		Clock(const Clock &other);
		Clock(Clock &&other) noexcept;

		Clock &operator=(const Clock &other);
		Clock &operator=(Clock &&other) noexcept;

		void Swap(Clock &other) noexcept;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

		unsigned GetHighDuration() const noexcept;
		unsigned GetLowDuration() const noexcept;
		unsigned GetPhaseOffset() const noexcept;

		void SetHighDuration(unsigned newHighDuration) noexcept;
		void GetLowDuration(unsigned newLowDuration) noexcept;
		void GetPhaseOffset(unsigned newPhaseOffset) noexcept;

		void Connect(const wire_t &wire);

	  private:
		unsigned m_highDuration;
		unsigned m_lowDuration;
		unsigned m_phaseOffset;
		sim::Wire m_wireOut;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_CLOCK_H */
