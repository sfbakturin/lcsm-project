#ifndef LCSM_LCSM_H
#define LCSM_LCSM_H

#include <lcsm/lcsmconfig.h>

#include <cstdint>

namespace lcsm
{
	using value_t = std::uint64_t;
	using id_t = std::uint64_t;
	using target_t = std::int8_t;
	using object_type_t = std::uint32_t;
	using label_t = const char *;

	/// Represents all implemented circuit elements in standard library.
	enum CircuitType : target_t
	{
		Wire,				 ///< Wire
		Tunnel,				 ///< Tunnel
		Pin,				 ///< Pin
		Constant,			 ///< Constant
		Power,				 ///< Power
		Ground,				 ///< Ground
		Clock,				 ///< Clock
		Transistor,			 ///< Transistor
		TransmissionGate,	 ///< Transmission gate
		Button,				 ///< Button
		Digit,				 ///< Digit
		Probe,				 ///< Probe
		Splitter,			 ///< Splitter
		VerilogModule,		 ///< Verilog's module
		LastCircuitType = VerilogModule + 1
	};

	LCSM_API const char *CircuitTypePretty(CircuitType circuitType) noexcept;

	/// Represents ordering of invoking and generating physical node's events.
	enum NodeType : target_t
	{
		Fast,	   ///< Fast nodes. Conceptually, these are those whose actual speed is "instantaneous" (for example,
				   ///< wires).
		Static,	   ///< Slow nodes. Conceptually, these are those whose speed in real life is slightly "slowed down" due
				   ///< to the effect of internal resistance.
		Dynamic,	///< Super slow nodes. Conceptually, they are those that in real life have the property of "opening
					///< up".
		FirstNodeType = Fast,
		LastNodeType = Dynamic + 1
	};

	/// Represents object properties in circuit for the flags. They can be combined using the bitwise OR operator.
	enum ObjectType : object_type_t
	{
		Wiring = 1 << 0,	  ///< Conceptually, it is an object that performs wiring between several objects.
		Root = 1 << 1,		  ///< Conceptually, it represents the object from which the passage of the current value
							  ///< begins.
		Internal = 1 << 2,	  ///< Indicates that the object can be found within the given circuit.
		External = 1 << 3,	  ///< Indicates that the object can be found as an element of third-party circuit.
		Input = 1 << 4,		  ///< Indicates that the object can be an input port to the circuit (can take on a value).
		Output = 1 << 5		  ///< Indicates that the object can be an output port from the circuit.
	};

	/**
	 * @brief Checks if object type has property.
	 *
	 * @param [in] actual object type
	 * @param [in] expected property to check
	 * @return true if the object has such a property
	 * @return false if the object does not have such a property
	 */
	LCSM_API constexpr inline bool TestObjectType(object_type_t actual, object_type_t expected) noexcept
	{
		return static_cast< bool >(actual & expected);
	}

	/// Represents all implemented instructions in standard library.
	enum InstructionType : target_t
	{
		WriteValue,		 ///< Instruction to write a value to a given element.
		PolluteValue,	 ///< Instruction to pollute value in element.
		LastInstructionType = WriteValue + 1
	};
}	 // namespace lcsm

#endif /* LCSM_LCSM_H */
