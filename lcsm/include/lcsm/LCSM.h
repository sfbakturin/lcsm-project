#ifndef LCSM_LCSM_H
#define LCSM_LCSM_H

#include <cstdint>

namespace lcsm
{
	using value_t = std::uint64_t;
	using id_t = std::uint64_t;
	using target_t = std::int8_t;

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
		LastCircuitType = TransmissionGate + 1
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
	enum ObjectType : target_t
	{
		UnknownObjectType,
		Wiring,
		PureInt,
		RootInt,
		IntExtIn,
		IntExtOut,
		LastObjectType = IntExtOut + 1
	};

	/* All available instructions. */
	enum InstructionType : target_t
	{
		WriteValue,
		LastInstructionType = WriteValue + 1
	};
}	 // namespace lcsm

#endif /* LCSM_LCSM_H */
