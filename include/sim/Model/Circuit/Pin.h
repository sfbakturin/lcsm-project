#ifndef SIM_MODEL_CIRCUIT_PIN_H
#define SIM_MODEL_CIRCUIT_PIN_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Model/Circuit/Constant.h>
#include <sim/Model/Width.h>
#include <sim/Model/Wiring/Wire.h>

namespace sim
{
	class Pin : public Constant
	{
	  public:
		Pin() noexcept;
		Pin(bool output) noexcept;
		Pin(bool output, Width width) noexcept;
		Pin(bool output, Width width, std::uint64_t value) noexcept;

		Pin(const Pin &other) noexcept;
		Pin(Pin &&other) noexcept;

		Pin &operator=(const Pin &other) noexcept;
		Pin &operator=(Pin &&other) noexcept;

		void Swap(Pin &other) noexcept;

	  private:
		bool m_output;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_PIN_H */
