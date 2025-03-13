#ifndef LCSM_VERILOG_PORT_H
#define LCSM_VERILOG_PORT_H

#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace lcsm
{
	namespace verilog
	{
		enum PortDirectionType : std::int8_t
		{
			UnknowPortDirectionType = -1,
			InputPortDirection,
			InoutPortDirection,
			OutputPortDirection
		};

		enum IntegerVectorType : std::int8_t
		{
			UnknownIntegerVectorType = -1,
			BitIntegerVector,
			LogicIntegerVector,
			RegIntegerVector
		};

		enum NetType : std::int8_t
		{
			UnknownNetType = -1,
			Supply0Net,
			Supply1Net,
			TriNet,
			TriandNet,
			TriorNet,
			Tri0Net,
			Tri1Net,
			UwireNet,
			WireNet,
			WandNet,
			WorNet
		};

		enum OutputVariableType : std::int8_t
		{
			UnknownOutputVariableType = -1,
			IntegerOutputVariable,
			TimeOutputVariable
		};

		const char *PortDirectionTypePretty(PortDirectionType portDirectionType) noexcept;
		const char *IntegerVectorTypePretty(IntegerVectorType integerVectorType) noexcept;
		const char *NetTypePretty(NetType netType) noexcept;
		const char *OutputVariableTypePretty(OutputVariableType outputVariableType) noexcept;

		using Range = std::pair< int, int >;

		class PortType
		{
		  public:
			PortType() noexcept;

			PortType(const PortType &other) noexcept;
			PortType(PortType &&other) noexcept;

			PortType &operator=(const PortType &other) noexcept;
			PortType &operator=(PortType &&other) noexcept;

			void swap(PortType &other) noexcept;

			PortDirectionType portDirectionType() const noexcept;
			void setPortDirectionType(PortDirectionType portDirectionType) noexcept;

			NetType netType() const noexcept;
			void setNetType(NetType netType) noexcept;

			bool isSigned() const noexcept;
			void setIsSigned(bool isSigned) noexcept;

			OutputVariableType outputVariableType() const noexcept;
			void setOutputVariableType(OutputVariableType outputVariableType) noexcept;

			IntegerVectorType integerVectorType() const noexcept;
			void setIntegerVectorType(IntegerVectorType integerVectorType) noexcept;

			bool isRangeValid() const noexcept;
			const Range &range() const noexcept;
			void setRange(const Range &range) noexcept;
			void setRange(Range &&range) noexcept;
			std::size_t rangeWidth() const noexcept;
			bool rangeLeftToRight() const noexcept;
			int rangeStart() const noexcept;
			int rangeEnd() const noexcept;

			std::string toVerilogString() const;

		  private:
			PortDirectionType m_portDirectionType;
			IntegerVectorType m_integerVectorType;
			NetType m_netType;
			bool m_isSigned;
			OutputVariableType m_outputVariableType;
			Range m_range;
			bool m_rangeValid;
		};

		class Port
		{
		  public:
			Port(const PortType &portType);
			Port(PortType &&portType);
			Port(const PortType &portType, const std::string &identifier);
			Port(const PortType &portType, std::string &&identifier) noexcept;
			Port(PortType &&portType, const std::string &identifier);
			Port(PortType &&portType, std::string &&identifier) noexcept;

			Port(const Port &other);
			Port(Port &&other) noexcept;

			Port &operator=(const Port &other);
			Port &operator=(Port &&other) noexcept;

			void swap(Port &other) noexcept;

			const PortType &portType() const noexcept;

			const std::string &identifier() const noexcept;
			void setIdentifier(const std::string &identifier);
			void setIdentifier(std::string &&identifier) noexcept;

			std::size_t width() const noexcept;

			std::string verilogPortDeclaration() const;
			std::vector< std::string > verilogPortAssignment(const DataBits &databits) const;
			std::string verilogPortMonitorCall(std::size_t globalPortIndex, std::size_t localPortIndex) const;

		  private:
			PortType m_portType;
			std::string m_identifier;
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_PORT_H */
