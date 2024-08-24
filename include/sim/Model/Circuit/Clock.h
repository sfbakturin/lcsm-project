#ifndef SIM_MODEL_CIRCUIT_CLOCK_H
#define SIM_MODEL_CIRCUIT_CLOCK_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Model/Wiring/Wire.h>

#include <cstddef>

namespace sim
{
	class Pin;

	class Clock : public CircuitComponent
	{
	  public:
		Clock(unsigned highDuration, unsigned lowDuration, unsigned phaseOffset);

		Clock(const Clock &other);
		Clock(Clock &&other) noexcept;

		Clock &operator=(const Clock &other);
		Clock &operator=(Clock &&other) noexcept;

		void Swap(Clock &other) noexcept;

		virtual unsigned ID() const noexcept override;
		virtual void Identify(unsigned ID) noexcept override;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

		unsigned GetHighDuration() const noexcept;
		unsigned GetLowDuration() const noexcept;
		unsigned GetPhaseOffset() const noexcept;

		void SetHighDuration(unsigned newHighDuration) noexcept;
		void GetLowDuration(unsigned newLowDuration) noexcept;
		void GetPhaseOffset(unsigned newPhaseOffset) noexcept;

		void Connect(wire_t &wire);

		virtual CircuitComponentType circuitComponentType() const noexcept override;

		virtual const Pin *AsPin() const noexcept override;
		virtual Pin *AsPin() noexcept override;

	  private:
		unsigned m_id;

		unsigned m_highDuration;
		unsigned m_lowDuration;
		unsigned m_phaseOffset;
		sim::Wire m_wireOut;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_CLOCK_H */
