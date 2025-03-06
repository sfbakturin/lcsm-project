#include <lcsm/Support/Parser/CFileSource.h>
#include <lcsm/Support/Parser/CStringSource.h>
#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Support/Parser/FileSource.h>
#include <lcsm/Support/Parser/StringSource.h>
#include <lcsm/Verilog/Module.h>
#include <lcsm/Verilog/ModuleDeclare/Context.h>
#include <lcsm/Verilog/ModuleDeclare/Parser.h>
#include <lcsm/Verilog/Port.h>

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <utility>

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
		for (std::string &identifier : portDeclaration.second)
		{
			m.m_ports.emplace_back(portType, std::move(identifier));
		}
	}
	return m;
}

lcsm::verilog::Module lcsm::verilog::Module::fromFile(std::FILE *stream)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::CFileSource >(stream);
	return parse(source);
}

lcsm::verilog::Module lcsm::verilog::Module::fromFile(const char *filename)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::FileSource >(filename);
	return parse(source);
}

lcsm::verilog::Module lcsm::verilog::Module::fromFile(std::ifstream &&stream)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::FileSource >(std::move(stream));
	return parse(source);
}

lcsm::verilog::Module lcsm::verilog::Module::fromString(const char *string)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::CStringSource >(string);
	return parse(source);
}

lcsm::verilog::Module lcsm::verilog::Module::fromString(const std::string &string)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::StringSource >(string);
	return parse(source);
}

lcsm::verilog::Module lcsm::verilog::Module::fromString(std::string &&string)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::StringSource >(std::move(string));
	return parse(source);
}
