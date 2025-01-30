#ifndef LCSM_LCSM_H
#define LCSM_LCSM_H

#include <cstdint>

namespace lcsm
{
	using value_t = std::uint64_t;
	using id_t = std::uint64_t;
	using timestamp_t = std::uint16_t;
	using timer_t = std::uint32_t;
	using target_t = unsigned;

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

		LastCircuitType = TransmissionGate + 1
	};

	enum NodeType : target_t
	{
		Fast,
		Static,
		Dynamic,

		LastNodeType = Dynamic + 1
	};

	enum ObjectType : target_t
	{
		UnknownObjectType,

		PureInt,

		RootInt,

		IntExtIn,
		IntExtOut,

		LastObjectType = IntExtOut + 1
	};

	enum InstructionType : target_t
	{
		WriteValue,
		RequestUpdate,
		UpdateState,

		LastInstructionType = UpdateState + 1
	};
}	 // namespace lcsm

#endif /* LCSM_LCSM_H */
