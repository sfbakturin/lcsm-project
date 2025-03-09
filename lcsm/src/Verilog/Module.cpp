#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/Files.h>
#include <lcsm/Support/Parser/CStringSource.h>
#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Support/Parser/FileSource.h>
#include <lcsm/Support/Parser/StringSource.h>
#include <lcsm/Support/Subprocesses.h>
#include <lcsm/Verilog/Module.h>
#include <lcsm/Verilog/ModuleDeclare/Context.h>
#include <lcsm/Verilog/ModuleDeclare/Parser.h>
#include <lcsm/Verilog/Port.h>
#include <unordered_map>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

enum TestBenchOutKind : signed
{
	UnknownKind = -1,
	InputKind,
	InoutKind,
	OutputKind,
	OutputRegKind,
	IntegerKind,
	EofKind
};

static const char *AsKeyword(TestBenchOutKind kind) noexcept
{
	switch (kind)
	{
	case TestBenchOutKind::InputKind:
		return "input";
	case TestBenchOutKind::InoutKind:
		return "inout";
	case TestBenchOutKind::OutputKind:
		return "output";
	case TestBenchOutKind::OutputRegKind:
		return "outputreg";
	default:
		return nullptr;
	}
	return nullptr;
}

static TestBenchOutKind IsKeyword(const char *str) noexcept
{
	for (unsigned i = TestBenchOutKind::InputKind; i <= TestBenchOutKind::OutputRegKind; i++)
	{
		const TestBenchOutKind kind = static_cast< TestBenchOutKind >(i);
		const char *keyword = AsKeyword(kind);
		if (std::strcmp(str, keyword) == 0)
		{
			return kind;
		}
	}
	return TestBenchOutKind::UnknownKind;
}

static TestBenchOutKind IsKeyword(const std::string &str) noexcept
{
	return IsKeyword(str.c_str());
}

class TestBenchOutToken
{
  public:
	TestBenchOutToken() noexcept;
	TestBenchOutToken(TestBenchOutKind kind) noexcept;
	TestBenchOutToken(int i) noexcept;

	TestBenchOutToken(const TestBenchOutToken &other) noexcept = default;
	TestBenchOutToken(TestBenchOutToken &&other) noexcept = default;

	TestBenchOutToken &operator=(const TestBenchOutToken &other) noexcept;
	TestBenchOutToken &operator=(TestBenchOutToken &&other) noexcept;

	void swap(TestBenchOutToken &other) noexcept;

	TestBenchOutKind kind() const noexcept;

	void setToken(TestBenchOutKind kind) noexcept;
	void setToken(int i) noexcept;
	void setEof() noexcept;

	bool isKeyword() const noexcept;
	bool isInteger() const noexcept;
	bool isEof() const noexcept;

	int asInteger() const noexcept;

  private:
	TestBenchOutKind m_kind;
	int m_i;
};

class TestBenchOutLexer
{
  public:
	TestBenchOutLexer(const std::shared_ptr< lcsm::support::CharSource > &source) noexcept;

	TestBenchOutToken token() const noexcept;
	TestBenchOutToken nextToken();

  private:
	std::shared_ptr< lcsm::support::CharSource > m_source;
	char m_char;
	TestBenchOutToken m_token;

  private:
	void nextChar();
};

TestBenchOutToken::TestBenchOutToken() noexcept : m_kind(TestBenchOutKind::UnknownKind), m_i(0) {}

TestBenchOutToken::TestBenchOutToken(TestBenchOutKind kind) noexcept : m_kind(kind), m_i(0) {}

TestBenchOutToken::TestBenchOutToken(int i) noexcept : m_kind(TestBenchOutKind::IntegerKind), m_i(i) {}

TestBenchOutToken &TestBenchOutToken::operator=(const TestBenchOutToken &other) noexcept
{
	return lcsm::support::CopyAssign< TestBenchOutToken >(this, other);
}

TestBenchOutToken &TestBenchOutToken::operator=(TestBenchOutToken &&other) noexcept
{
	return lcsm::support::MoveAssign< TestBenchOutToken >(this, std::move(other));
}

void TestBenchOutToken::swap(TestBenchOutToken &other) noexcept
{
	std::swap(m_kind, other.m_kind);
	std::swap(m_i, other.m_i);
}

TestBenchOutKind TestBenchOutToken::kind() const noexcept
{
	return m_kind;
}

void TestBenchOutToken::setToken(TestBenchOutKind kind) noexcept
{
	m_kind = kind;
	m_i = 0;
}

void TestBenchOutToken::setToken(int i) noexcept
{
	m_kind = TestBenchOutKind::IntegerKind;
	m_i = i;
}

void TestBenchOutToken::setEof() noexcept
{
	m_kind = TestBenchOutKind::EofKind;
	m_i = 0;
}

bool TestBenchOutToken::isKeyword() const noexcept
{
	return TestBenchOutKind::InputKind <= m_kind && m_kind <= TestBenchOutKind::OutputRegKind;
}

bool TestBenchOutToken::isInteger() const noexcept
{
	return m_kind == TestBenchOutKind::IntegerKind;
}

bool TestBenchOutToken::isEof() const noexcept
{
	return m_kind == TestBenchOutKind::EofKind;
}

int TestBenchOutToken::asInteger() const noexcept
{
	return m_i;
}

TestBenchOutLexer::TestBenchOutLexer(const std::shared_ptr< lcsm::support::CharSource > &source) noexcept :
	m_source(source)
{
	nextChar();
}

TestBenchOutToken TestBenchOutLexer::token() const noexcept
{
	return m_token;
}

TestBenchOutToken TestBenchOutLexer::nextToken()
{
	return m_token;
}

void TestBenchOutLexer::nextChar()
{
	m_char = lcsm::support::CharSource::EndOfSource;
}

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
		std::size_t i = 0;
		for (std::string &identifier : portDeclaration.second)
		{
			m.m_ports.emplace_back(ioType, i++);
			switch (ioType)
			{
			case lcsm::verilog::IOType::UnknowPortType:
			{
				break;
			}
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
	m_identifier(other.m_identifier), m_sourceModule(other.m_sourceModule), m_inputPorts(other.m_inputPorts),
	m_inoutPorts(other.m_inoutPorts), m_outputPorts(other.m_outputPorts), m_outputRegPorts(other.m_outputRegPorts),
	m_ports(other.m_ports)
{
}

lcsm::verilog::Module::Module(lcsm::verilog::Module &&other) noexcept :
	m_identifier(std::move(other.m_identifier)), m_sourceModule(std::move(other.m_sourceModule)),
	m_inputPorts(std::move(other.m_inputPorts)), m_inoutPorts(std::move(other.m_inoutPorts)),
	m_outputPorts(std::move(other.m_outputPorts)), m_outputRegPorts(std::move(other.m_outputRegPorts)),
	m_ports(std::move(other.m_ports))
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
	std::swap(m_sourceModule, other.m_sourceModule);
	std::swap(m_inputPorts, other.m_inputPorts);
	std::swap(m_inoutPorts, other.m_inoutPorts);
	std::swap(m_outputPorts, other.m_outputPorts);
	std::swap(m_outputRegPorts, other.m_outputRegPorts);
	std::swap(m_ports, other.m_ports);
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

lcsm::verilog::Module lcsm::verilog::Module::fromFile(const std::string &filename)
{
	// Re-use existing method.
	return lcsm::verilog::Module::fromFile(filename.c_str());
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

static void SourcePortDeclaration(const std::vector< lcsm::verilog::Port > &ports, std::string &source)
{
	for (const lcsm::verilog::Port &port : ports)
	{
		source += port.verilogPortDeclaration() + ";\n";
	}
}

static void SourcePortAssignment(
	const std::unordered_map< lcsm::verilog::IOType, std::vector< lcsm::DataBits > > &datas,
	const std::vector< lcsm::verilog::Port > &ports,
	lcsm::verilog::IOType type,
	std::string &source)
{
	const std::unordered_map< lcsm::verilog::IOType, std::vector< lcsm::DataBits > >::const_iterator found = datas.find(type);
	if (found == datas.end())
	{
		return;
	}

	const std::vector< lcsm::DataBits > &data = found->second;

	// Ensures, that databits.size() == ports.size() by construction from LCSMEngine/LCSMCircuit.
	for (std::size_t i = 0; i < ports.size(); i++)
	{
		const lcsm::DataBits &databits = data[i];
		const lcsm::verilog::Port &port = ports[i];
		std::vector< std::string > assignments = port.verilogPortAssignment(databits);
		for (std::string &assignment : assignments)
		{
			source += std::move(assignment) + '\n';
		}
	}
}

std::unordered_map< lcsm::verilog::IOType, std::vector< lcsm::DataBits > >
	lcsm::verilog::Module::invoke(const std::unordered_map< lcsm::verilog::IOType, std::vector< lcsm::DataBits > > &testBenchData)
{
	// Step 1. Generate testbench module.
	std::string testBenchSource = m_sourceModule;

	// 1.1. Module declaration.
	testBenchSource += "\nmodule " + m_identifier + "_tb;\n";

	// 1.2. Port declaration.
	SourcePortDeclaration(m_inputPorts, testBenchSource);
	SourcePortDeclaration(m_inoutPorts, testBenchSource);
	SourcePortDeclaration(m_outputPorts, testBenchSource);
	SourcePortDeclaration(m_outputRegPorts, testBenchSource);

	// 1.3. Assignment.
	SourcePortAssignment(testBenchData, m_inputPorts, lcsm::verilog::IOType::Input, testBenchSource);
	SourcePortAssignment(testBenchData, m_inoutPorts, lcsm::verilog::IOType::Inout, testBenchSource);

	// 1.4. Module declare.
	testBenchSource += m_identifier + " tb(";
	bool needsComma = false;
	for (const std::pair< lcsm::verilog::IOType, std::size_t > &port : m_ports)
	{
		if (needsComma)
		{
			testBenchSource.push_back(',');
		}
		switch (port.first)
		{
		case lcsm::verilog::IOType::UnknowPortType:
		{
			throw std::logic_error("Found unknown port type, impossible!");
		}
		case lcsm::verilog::IOType::Input:
		{
			testBenchSource += m_inputPorts[port.second].identifier();
			break;
		}
		case lcsm::verilog::IOType::Inout:
		{
			testBenchSource += m_inoutPorts[port.second].identifier();
			break;
		}
		case lcsm::verilog::IOType::Output:
		{
			testBenchSource += m_outputPorts[port.second].identifier();
			break;
		}
		case lcsm::verilog::IOType::OutputReg:
		{
			testBenchSource += m_outputRegPorts[port.second].identifier();
			break;
		}
		}
		needsComma = true;
	}
	testBenchSource += ");\n";

	// 1.5. Monitor.
	testBenchSource += "initial begin\n";
	std::size_t t = 1;
	for (const std::pair< lcsm::verilog::IOType, std::size_t > &port : m_ports)
	{
		lcsm::verilog::Port *p = nullptr;
		switch (port.first)
		{
		case lcsm::verilog::IOType::UnknowPortType:
		{
			throw std::logic_error("Found unknown port type, impossible!");
		}
		case lcsm::verilog::IOType::Input:
		{
			p = std::addressof(m_inputPorts[port.second]);
			break;
		}
		case lcsm::verilog::IOType::Inout:
		{
			p = std::addressof(m_inoutPorts[port.second]);
			break;
		}
		case lcsm::verilog::IOType::Output:
		{
			p = std::addressof(m_outputPorts[port.second]);
			break;
		}
		case lcsm::verilog::IOType::OutputReg:
		{
			p = std::addressof(m_outputRegPorts[port.second]);
			break;
		}
		}
		for (lcsm::width_t w = 0; w < p->width(); w++)
		{
			testBenchSource += '#' + std::to_string(t) + ' ';
			testBenchSource += p->verilogPortMonitorCall(port.second, w) + ";\n";
			t++;
		}
	}
	testBenchSource += "end\n";

	// 1.6. Endmodule.
	testBenchSource += "endmodule";

	// Step 2. Save test bench source to temporary file.
	const std::string tempTestBenchSourceFilename = std::tmpnam(nullptr);
	const std::string tempTestBenchOutputFilename = std::tmpnam(nullptr);

	if (tempTestBenchSourceFilename.empty() || tempTestBenchOutputFilename.empty())
	{
		throw std::runtime_error("Can't generate random temporary filename!");
	}

	std::ofstream tempTestBenchSourceFile(tempTestBenchSourceFilename);
	if (!tempTestBenchSourceFile.is_open())
	{
		throw std::runtime_error("Can't create temporary file to write!");
	}

	tempTestBenchSourceFile << testBenchSource;
	tempTestBenchSourceFile.close();

	// Step 3. Run Icarus Verilog compiler and VM and get standard output/error and return code.
	const char *outputFilename = tempTestBenchOutputFilename.c_str();
	const char *sourceFilename = tempTestBenchSourceFilename.c_str();

	const lcsm::support::CompletedProcess iVerilogCompilerProcCompleted =
		lcsm::support::subprocessRun("iverilog", { "-g2012", "-o", outputFilename, sourceFilename });
	iVerilogCompilerProcCompleted.checkReturnCode();

	const lcsm::support::CompletedProcess vppVMCompleted = lcsm::support::subprocessRun("vvp", { outputFilename });
	vppVMCompleted.checkReturnCode();

	// Step 4. Remove temporary files.
	lcsm::support::removeFile(tempTestBenchSourceFilename);
	lcsm::support::removeFile(tempTestBenchOutputFilename);

	// Step 5. Parse standard output and return as output data.
	std::shared_ptr< lcsm::support::CharSource > source =
		std::make_shared< lcsm::support::CStringSource >(vppVMCompleted.cOut());
	TestBenchOutLexer lex = source;

	return testBenchData;
}
