#ifndef LCSM_VERILOG_PORT_H
#define LCSM_VERILOG_PORT_H

#include <cstdint>
#include <string>
#include <utility>

namespace lcsm
{
	namespace verilog
	{
		enum IOType : std::int8_t
		{
			UnknowPortType = -1,
			Input,
			Inout,
			Output,
			OutputReg
		};

		enum NetType : std::int8_t
		{
			UnknownPortType = -1,
			Supply0,
			Supply1,
			Tri,
			Triand,
			Trior,
			Tri0,
			Tri1,
			Uwire,
			Wire,
			Wand,
			Wor
		};

		enum OutputVariableType : std::int8_t
		{
			UnknownOutputVariableType = -1,
			Integer,
			Time
		};

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

			IOType ioType() const noexcept;
			void setIOType(IOType ioType) noexcept;

			NetType netType() const noexcept;
			void setNetType(NetType netType) noexcept;

			bool isSigned() const noexcept;
			void setIsSigned(bool isSigned) noexcept;

			OutputVariableType outputVariableType() const noexcept;
			void setOutputVariableType(OutputVariableType outputVariableType) noexcept;

			bool isRangeValid() const noexcept;
			const Range &range() const noexcept;
			void setRange(const Range &range) noexcept;
			void setRange(Range &&range) noexcept;

		  private:
			IOType m_ioType;
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

		  private:
			PortType m_portType;
			std::string m_identifier;
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_PORT_H */
