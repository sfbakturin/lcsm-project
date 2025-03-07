#include <lcsm/Physical/DataBits.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/Parser/CFileSource.h>
#include <lcsm/Support/Parser/CStringSource.h>
#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Support/Parser/FileSource.h>
#include <lcsm/Support/Parser/StringSource.h>
#include <lcsm/Verilog/Module.h>
#include <lcsm/Verilog/ModuleDeclare/Context.h>
#include <lcsm/Verilog/ModuleDeclare/Parser.h>
#include <lcsm/Verilog/Port.h>
#include <unordered_map>

#include <cstdio>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

lcsm::verilog::Module lcsm::verilog::Module::parse(const std::shared_ptr< lcsm::support::CharSource > &source)
{
	lcsm::verilog::ModuleDeclareContext context;
	lcsm::verilog::ModuleDeclareParser parser{ source };
	parser.parse(context);
	lcsm::verilog::Module m;
	m.m_identifier = context.moduleIdentifier();
	for (std::pair< lcsm::verilog::PortType, std::vector< std::string > > &portDeclaration : context.modulePortDeclarations())
	{
		const lcsm::verilog::PortType portType = portDeclaration.first;
		const lcsm::verilog::IOType ioType = portType.ioType();
		for (std::string &identifier : portDeclaration.second)
		{
			switch (ioType)
			{
			case lcsm::verilog::IOType::UnknowPortType:
				break;
			case lcsm::verilog::IOType::Input:
			{
				m.m_inputPorts.emplace_back(portType, std::move(identifier));
				break;
			}
			case lcsm::verilog::IOType::Inout:
			{
				m.m_inoutPorts.emplace_back(portType, std::move(identifier));

				break;
			}
			case lcsm::verilog::IOType::Output:
			{
				m.m_outputPorts.emplace_back(portType, std::move(identifier));

				break;
			}
			case lcsm::verilog::IOType::OutputReg:
			{
				m.m_outputRegPorts.emplace_back(portType, std::move(identifier));
				break;
			}
			}
		}
	}
	return m;
}

lcsm::verilog::Module::Module(const lcsm::verilog::Module &other) :
	m_identifier(other.m_identifier), m_inputPorts(other.m_inputPorts), m_inoutPorts(other.m_inoutPorts),
	m_outputPorts(other.m_outputPorts), m_outputRegPorts(other.m_outputRegPorts)
{
}

lcsm::verilog::Module::Module(lcsm::verilog::Module &&other) noexcept :
	m_identifier(std::move(other.m_identifier)), m_inputPorts(std::move(other.m_inputPorts)),
	m_inoutPorts(std::move(other.m_inoutPorts)), m_outputPorts(std::move(other.m_outputPorts)),
	m_outputRegPorts(std::move(other.m_outputRegPorts))
{
}

lcsm::verilog::Module &lcsm::verilog::Module::operator=(const lcsm::verilog::Module &other)
{
	return lcsm::support::CopyAssign< lcsm::verilog::Module >(this, other);
}

lcsm::verilog::Module &lcsm::verilog::Module::operator=(lcsm::verilog::Module &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::verilog::Module >(this, std::move(other));
}

void lcsm::verilog::Module::swap(lcsm::verilog::Module &other) noexcept
{
	std::swap(m_identifier, other.m_identifier);
	std::swap(m_inputPorts, other.m_inputPorts);
	std::swap(m_inoutPorts, other.m_inoutPorts);
	std::swap(m_outputPorts, other.m_outputPorts);
	std::swap(m_outputRegPorts, other.m_outputRegPorts);
}

lcsm::verilog::Module lcsm::verilog::Module::fromFile(std::FILE *stream)
{
	// TODO: Implement me.
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::CFileSource >(stream);
	return parse(source);
}

lcsm::verilog::Module lcsm::verilog::Module::fromFile(const char *filename)
{
	// Create source-iterator.
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::FileSource >(filename);

	// Make a module.
	lcsm::verilog::Module module = parse(source);

	// Copy contents.
	std::ifstream file{ filename };
	if (!file.is_open())
	{
		throw std::logic_error("Resource can't be established!");
	}

	std::stringstream buffer;
	buffer << file.rdbuf();

	module.m_sourceModule = buffer.str();

	return module;
}

lcsm::verilog::Module lcsm::verilog::Module::fromFile(std::ifstream &stream)
{
	// Create source-iterator.
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::FileSource >(std::move(stream));

	// Make a module.
	lcsm::verilog::Module module = parse(source);

	// TODO: Implement me.

	return module;
}

lcsm::verilog::Module lcsm::verilog::Module::fromString(const char *string)
{
	// Create source-iterator.
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::CStringSource >(string);

	// Make a module
	lcsm::verilog::Module module = parse(source);

	// Copy contents.
	module.m_sourceModule = string;

	return module;
}

lcsm::verilog::Module lcsm::verilog::Module::fromString(const std::string &string)
{
	// Create source-iterator.
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::StringSource >(string);

	// Make a module
	lcsm::verilog::Module module = parse(source);

	// Copy contents.
	module.m_sourceModule = string;

	return module;
}

lcsm::verilog::Module lcsm::verilog::Module::fromString(std::string &&string)
{
	// Create source-iterator.
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::CStringSource >(string.c_str());

	// Make a module
	lcsm::verilog::Module module = parse(source);

	// Copy contents.
	module.m_sourceModule = std::move(string);

	return module;
}

const std::vector< lcsm::verilog::Port > &lcsm::verilog::Module::inputPorts() const noexcept
{
	return m_inputPorts;
}

const std::vector< lcsm::verilog::Port > &lcsm::verilog::Module::inoutPorts() const noexcept
{
	return m_inoutPorts;
}

const std::vector< lcsm::verilog::Port > &lcsm::verilog::Module::outputPorts() const noexcept
{
	return m_outputPorts;
}

const std::vector< lcsm::verilog::Port > &lcsm::verilog::Module::outputRegPorts() const noexcept
{
	return m_outputRegPorts;
}

std::unordered_map< lcsm::verilog::IOType, std::vector< lcsm::DataBits > >
	lcsm::verilog::Module::invoke(const std::unordered_map< lcsm::verilog::IOType, std::vector< lcsm::DataBits > > &testBenchData)
{
	// TODO: Implement me.
	return testBenchData;
}
