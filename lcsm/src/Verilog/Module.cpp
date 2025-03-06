#include <lcsm/Support/Parser/CFileSource.h>
#include <lcsm/Support/Parser/CStringSource.h>
#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Support/Parser/FileSource.h>
#include <lcsm/Support/Parser/StringSource.h>
#include <lcsm/Verilog/Module.h>
#include <lcsm/Verilog/ModuleDeclare/Parser.h>

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <utility>

static void parse(const std::shared_ptr< lcsm::support::CharSource > &source)
{
	lcsm::verilog::ModuleDeclareParser parser{ source };
	parser.parse();
}

lcsm::verilog::Module lcsm::verilog::Module::fromFile(std::FILE *stream)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::CFileSource >(stream);
	parse(source);
	return {};
}

lcsm::verilog::Module lcsm::verilog::Module::fromFile(const char *filename)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::FileSource >(filename);
	parse(source);
	return {};
}

lcsm::verilog::Module lcsm::verilog::Module::fromFile(std::ifstream &&stream)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::FileSource >(std::move(stream));
	parse(source);
	return {};
}

lcsm::verilog::Module lcsm::verilog::Module::fromString(const char *string)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::CStringSource >(string);
	parse(source);
	return {};
}

lcsm::verilog::Module lcsm::verilog::Module::fromString(const std::string &string)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::StringSource >(string);
	parse(source);
	return {};
}

lcsm::verilog::Module lcsm::verilog::Module::fromString(std::string &&string)
{
	std::shared_ptr< lcsm::support::CharSource > source = std::make_shared< lcsm::support::StringSource >(std::move(string));
	parse(source);
	return {};
}
