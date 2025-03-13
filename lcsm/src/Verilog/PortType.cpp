#include <lcsm/Model/Width.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Verilog/Port.h>

#include <string>
#include <utility>

lcsm::verilog::PortType::PortType() noexcept :
	m_portDirectionType(lcsm::verilog::PortDirectionType::UnknowPortDirectionType),
	m_integerVectorType(lcsm::verilog::IntegerVectorType::UnknownIntegerVectorType),
	m_netType(lcsm::verilog::NetType::UnknownNetType), m_isSigned(false),
	m_outputVariableType(lcsm::verilog::UnknownOutputVariableType), m_range(0, 0), m_rangeValid(false)
{
}

lcsm::verilog::PortType::PortType(const lcsm::verilog::PortType &other) noexcept :
	m_portDirectionType(other.m_portDirectionType), m_integerVectorType(other.m_integerVectorType),
	m_netType(other.m_netType), m_isSigned(other.m_isSigned), m_outputVariableType(other.m_outputVariableType),
	m_range(other.m_range), m_rangeValid(other.m_rangeValid)
{
}

lcsm::verilog::PortType::PortType(lcsm::verilog::PortType &&other) noexcept :
	m_portDirectionType(other.m_portDirectionType), m_integerVectorType(other.m_integerVectorType),
	m_netType(other.m_netType), m_isSigned(other.m_isSigned), m_outputVariableType(other.m_outputVariableType),
	m_range(std::move(other.m_range)), m_rangeValid(other.m_rangeValid)
{
}

lcsm::verilog::PortType &lcsm::verilog::PortType::operator=(const lcsm::verilog::PortType &other) noexcept
{
	return lcsm::support::CopyAssign< lcsm::verilog::PortType >(this, other);
}

lcsm::verilog::PortType &lcsm::verilog::PortType::operator=(lcsm::verilog::PortType &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::verilog::PortType >(this, std::move(other));
}

void lcsm::verilog::PortType::swap(lcsm::verilog::PortType &other) noexcept
{
	std::swap(m_portDirectionType, other.m_portDirectionType);
	std::swap(m_integerVectorType, other.m_integerVectorType);
	std::swap(m_netType, other.m_netType);
	std::swap(m_isSigned, other.m_isSigned);
	std::swap(m_outputVariableType, other.m_outputVariableType);
	std::swap(m_range, other.m_range);
	std::swap(m_rangeValid, other.m_rangeValid);
}

lcsm::verilog::PortDirectionType lcsm::verilog::PortType::portDirectionType() const noexcept
{
	return m_portDirectionType;
}

void lcsm::verilog::PortType::setPortDirectionType(lcsm::verilog::PortDirectionType portDirectionType) noexcept
{
	m_portDirectionType = portDirectionType;
}

lcsm::verilog::NetType lcsm::verilog::PortType::netType() const noexcept
{
	return m_netType;
}

void lcsm::verilog::PortType::setNetType(lcsm::verilog::NetType netType) noexcept
{
	m_netType = netType;
}

bool lcsm::verilog::PortType::isSigned() const noexcept
{
	return m_isSigned;
}

void lcsm::verilog::PortType::setIsSigned(bool isSigned) noexcept
{
	m_isSigned = isSigned;
}

lcsm::verilog::OutputVariableType lcsm::verilog::PortType::outputVariableType() const noexcept
{
	return m_outputVariableType;
}

void lcsm::verilog::PortType::setOutputVariableType(lcsm::verilog::OutputVariableType outputVariableType) noexcept
{
	m_outputVariableType = outputVariableType;
}

lcsm::verilog::IntegerVectorType lcsm::verilog::PortType::integerVectorType() const noexcept
{
	return m_integerVectorType;
}

void lcsm::verilog::PortType::setIntegerVectorType(lcsm::verilog::IntegerVectorType integerVectorType) noexcept
{
	m_integerVectorType = integerVectorType;
}

bool lcsm::verilog::PortType::isRangeValid() const noexcept
{
	return m_rangeValid;
}

const lcsm::verilog::Range &lcsm::verilog::PortType::range() const noexcept
{
	return m_range;
}

void lcsm::verilog::PortType::setRange(const lcsm::verilog::Range &range) noexcept
{
	m_range = range;
	m_rangeValid = true;
}

void lcsm::verilog::PortType::setRange(lcsm::verilog::Range &&range) noexcept
{
	m_range = std::move(range);
	m_rangeValid = true;
}

std::size_t lcsm::verilog::PortType::rangeWidth() const noexcept
{
	const int s = std::min(m_range.first, m_range.second);
	const int e = std::max(m_range.first, m_range.second);
	if (e >= 0)
	{
		return static_cast< std::size_t >(e) - s + 1;
	}
	else
	{
		return static_cast< std::size_t >(-e) - s + 1;
	}
}

bool lcsm::verilog::PortType::rangeLeftToRight() const noexcept
{
	return m_range.first <= m_range.second;
}

int lcsm::verilog::PortType::rangeStart() const noexcept
{
	return (rangeLeftToRight() ? std::min(m_range.first, m_range.second) : std::max(m_range.first, m_range.second));
}

int lcsm::verilog::PortType::rangeEnd() const noexcept
{
	return (rangeLeftToRight() ? std::max(m_range.first, m_range.second) : std::min(m_range.first, m_range.second));
}

std::string lcsm::verilog::PortType::toVerilogString() const
{
	// Generated string:
	// <type> [<range_f>:<rangle_s>]
	std::string builder;

	// Type string.
	if (m_integerVectorType != lcsm::verilog::IntegerVectorType::UnknownIntegerVectorType)
	{
		builder.append(lcsm::verilog::IntegerVectorTypePretty(m_integerVectorType));
	}
	else
	{
		builder.append("wire");
	}

	// Range.
	builder += '[' + std::to_string(m_range.first) + ':' + std::to_string(m_range.second) + ']';

	return builder;
}
