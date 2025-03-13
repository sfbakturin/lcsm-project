#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Port.h>
#include <lcsm/Verilog/Strength.h>
#include <lcsm/Verilog/Value.h>

#include <string>
#include <utility>
#include <vector>

lcsm::verilog::Port::Port(const lcsm::verilog::PortType &portType) : m_portType(portType) {}

lcsm::verilog::Port::Port(lcsm::verilog::PortType &&portType) : m_portType(portType) {}

lcsm::verilog::Port::Port(const lcsm::verilog::PortType &portType, const std::string &identifier) :
	m_portType(portType), m_identifier(identifier)
{
}

lcsm::verilog::Port::Port(const lcsm::verilog::PortType &portType, std::string &&identifier) noexcept :
	m_portType(portType), m_identifier(std::move(identifier))
{
}

lcsm::verilog::Port::Port(lcsm::verilog::PortType &&portType, const std::string &identifier) :
	m_portType(std::move(portType)), m_identifier(identifier)
{
}

lcsm::verilog::Port::Port(lcsm::verilog::PortType &&portType, std::string &&identifier) noexcept :
	m_portType(std::move(portType)), m_identifier(std::move(identifier))
{
}

lcsm::verilog::Port::Port(const Port &other) : m_portType(other.m_portType), m_identifier(other.m_identifier) {}

lcsm::verilog::Port::Port(Port &&other) noexcept :
	m_portType(std::move(other.m_portType)), m_identifier(std::move(other.m_identifier))
{
}

lcsm::verilog::Port &lcsm::verilog::Port::operator=(const Port &other)
{
	return lcsm::support::CopyAssign< lcsm::verilog::Port >(this, other);
}

lcsm::verilog::Port &lcsm::verilog::Port::operator=(Port &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::verilog::Port >(this, std::move(other));
}

void lcsm::verilog::Port::swap(lcsm::verilog::Port &other) noexcept
{
	std::swap(m_portType, other.m_portType);
	std::swap(m_identifier, other.m_identifier);
}

const lcsm::verilog::PortType &lcsm::verilog::Port::portType() const noexcept
{
	return m_portType;
}

const std::string &lcsm::verilog::Port::identifier() const noexcept
{
	return m_identifier;
}

void lcsm::verilog::Port::setIdentifier(const std::string &identifier)
{
	m_identifier = identifier;
}

void lcsm::verilog::Port::setIdentifier(std::string &&identifier) noexcept
{
	m_identifier = std::move(identifier);
}

std::size_t lcsm::verilog::Port::width() const noexcept
{
	return m_portType.rangeWidth();
}

std::string lcsm::verilog::Port::verilogPortDeclaration() const
{
	// Generated string:
	// <type> <id>
	return m_portType.toVerilogString() + " " + m_identifier;
}

std::vector< std::string > lcsm::verilog::Port::verilogPortAssignment(const lcsm::DataBits &databits) const
{
	std::vector< std::string > portAssignments;

	const int di = (m_portType.rangeLeftToRight() ? 1 : -1);
	int id = m_portType.rangeStart();
	std::size_t supplyUniqueIndex = 0;

	for (std::size_t i = 0; i < m_portType.rangeWidth(); i++)
	{
		const lcsm::verilog::Value value = databits.value(i);
		const lcsm::verilog::Bit bit = value.bit();
		std::string s;

		if (m_portType.integerVectorType() != lcsm::verilog::IntegerVectorType::UnknownIntegerVectorType)
		{
			if (value.strength() != lcsm::verilog::Strength::HighImpedance && bit != lcsm::verilog::Bit::Undefined)
			{
				s = "assign " + m_identifier + '[' + std::to_string(id) + "] = 1'b" + lcsm::verilog::BitPretty(bit) + ';';
			}
		}
		else
		{
			switch (value.strength())
			{
			case lcsm::verilog::Strength::HighImpedance:
			{
				// Do nothing, Verilog will assign this bit as HiZ.
				break;
			}
			case lcsm::verilog::Strength::WeakDrive:
			{
				// assign (weak1, weak0) <id>[<i>] = 1'b0; // We0
				// assign (weak1, weak0) <id>[<i>] = 1'b1; // We1

				if (bit != lcsm::verilog::Bit::Undefined)
				{
					s = "assign (weak1, weak0) " + m_identifier + '[' + std::to_string(id) + "] = 1'b" +
						lcsm::verilog::BitPretty(bit) + ';';
				}

				break;
			}
			case lcsm::verilog::Strength::PullDrive:
			{
				// pulldown (<id>[<i>]); // Pu0
				// pullup (<id>[<i>]);   // Pu1

				std::string kw;

				switch (bit)
				{
				case lcsm::verilog::Bit::Undefined:
				{
					break;
				}
				case lcsm::verilog::Bit::False:
				{
					kw = "pulldown";
					break;
				}
				case lcsm::verilog::Bit::True:
				{
					kw = "pullup";
					break;
				}
				}

				if (!kw.empty())
				{
					s = kw + " (" + m_identifier + '[' + std::to_string(id) + "]);";
				}

				break;
			}
			case lcsm::verilog::Strength::StrongDrive:
			{
				// assign <id>[<i>] = 1'b0; // St0
				// assign <id>[<i>] = 1'b1; // St1

				if (bit != lcsm::verilog::Bit::Undefined)
				{
					s = "assign " + m_identifier + '[' + std::to_string(id) + "] = 1'b" + lcsm::verilog::BitPretty(bit) + ";";
				}

				break;
			}
			case lcsm::verilog::Strength::SupplyDrive:
			{
				// supply0 <id>_s0; assign <id>[<i>] = <id>_s0; // Su0
				// supply1 <id>_s1; assign <id>[<i>] = <id>_s1; // Su1

				std::string supplyIndex = std::to_string(supplyUniqueIndex);

				switch (bit)
				{
				case lcsm::verilog::Bit::Undefined:
				{
					break;
				}
				case lcsm::verilog::Bit::False:
				{
					const std::string supplyVariableName = m_identifier + '_' + std::move(supplyIndex);
					s = "supply0 " + supplyVariableName + "; ";
					s += "assign " + m_identifier + '[' + std::to_string(id) + "] = " + supplyVariableName + ';';
					break;
				}
				case lcsm::verilog::Bit::True:
				{
					const std::string supplyVariableName = m_identifier + '_' + std::move(supplyIndex);
					s = "supply1 " + supplyVariableName + "; ";
					s += "assign " + m_identifier + '[' + std::to_string(id) + "] = " + supplyVariableName + ';';
					break;
				}
				}

				supplyUniqueIndex++;

				break;
			}
			}
		}

		if (!s.empty())
		{
			portAssignments.push_back(std::move(s));
		}

		id += di;
	}

	return portAssignments;
}

std::string lcsm::verilog::Port::verilogPortMonitorCall(std::size_t globalPortIndex, std::size_t localPortIndex) const
{
	const int di = (m_portType.rangeLeftToRight() ? 1 : -1);

	std::string s = "$monitor(\"";
	// Port is always must be parsed successfully, so IOType is known.
	s.append(lcsm::verilog::PortDirectionTypePretty(m_portType.portDirectionType()));
	s.append(' ' + std::to_string(globalPortIndex) + ' ' + std::to_string(localPortIndex) + " %v\", " + m_identifier +
			 '[' + std::to_string(m_portType.rangeStart() + di * static_cast< int >(localPortIndex)) + "])");
	// Generated string:
	// $monitor("<iotype> <global> <local> %v", <id>[<local>])
	return s;
}
