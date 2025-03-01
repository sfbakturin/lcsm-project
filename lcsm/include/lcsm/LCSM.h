#ifndef LCSM_LCSM_H
#define LCSM_LCSM_H

#include <cstdint>

namespace lcsm
{
	using value_t = std::uint64_t;
	using id_t = std::uint64_t;
	using target_t = std::int8_t;
	using object_type_t = std::uint32_t;
	using label_t = const char *;

	/* All available model circuits. */
	enum CircuitType : target_t
	{
		Wire,
		Tunnel,
		Pin,
		Constant,
		Power,
		Ground,
		Clock,
		Transistor,
		TransmissionGate,
		Button,
		Digit,
		Probe,
		Splitter,
		LastCircuitType = Splitter + 1
	};

	/* All available physical node types. */
	enum NodeType : target_t
	{
		Fast,
		Static,
		Dynamic,
		FirstNodeType = Fast,
		LastNodeType = Dynamic + 1
	};

	/* All available physical object types. */
	enum ObjectType : object_type_t
	{
		Wiring = 1 << 0,
		Root = 1 << 1,
		Internal = 1 << 2,
		External = 1 << 3,
		Input = 1 << 4,
		Output = 1 << 5
	};

	constexpr inline bool TestObjectType(object_type_t actual, object_type_t expected) noexcept
	{
		return static_cast< bool >(actual & expected);
	}

	/* All available instructions. */
	enum InstructionType : target_t
	{
		WriteValue,
		LastInstructionType = WriteValue + 1
	};
}	 // namespace lcsm

#endif /* LCSM_LCSM_H */
