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
		Pin(bool output = false, Width width = Width::W1, std::uint64_t value = 0);

		Pin(const Pin &other);
		Pin(Pin &&other) noexcept;

		Pin &operator=(const Pin &other);
		Pin &operator=(Pin &&other) noexcept;

		void Swap(Pin &other) noexcept;

	  private:
		bool m_output;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_PIN_H */
