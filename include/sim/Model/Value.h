#ifndef SIM_VALUE_H
#define SIM_VALUE_H

namespace sim
{
	class Value
	{
	  public:
		enum class State: unsigned
		{
			UNDEFINED,
			TRUE,
			FALSE,
			INCOMPATIBLE_WIDTHS,
			ERROR
		};

		Value() noexcept;
		Value(const Value &other) noexcept;
		Value(Value &&other) noexcept;

	  private:
		State m_state;
	};
}

#endif /* SIM_VALUE_H */
