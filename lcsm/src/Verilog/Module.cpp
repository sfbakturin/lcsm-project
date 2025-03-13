#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/Files.h>
#include <lcsm/Support/Parser/CStringSource.h>
#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Support/Parser/FileSource.h>
#include <lcsm/Support/Parser/StringSource.h>
#include <lcsm/Support/Subprocesses.h>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Module.h>
#include <lcsm/Verilog/ModuleDeclare/Context.h>
#include <lcsm/Verilog/ModuleDeclare/Parser.h>
#include <lcsm/Verilog/Port.h>
#include <lcsm/Verilog/Strength.h>
#include <unordered_map>

#include <cctype>
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
	InoutKind,
	OutputKind,
	OutputRegKind,
	HiZKind,
	We0Kind,
	We1Kind,
	WeXKind,
	Pu0Kind,
	Pu1Kind,
	PuXKind,
	St0Kind,
	St1Kind,
	StXKind,
	Su0Kind,
	Su1Kind,
	SuXKind,
	IntegerKind,
	EofKind
};

static const char *AsKeyword(TestBenchOutKind kind) noexcept
{
	switch (kind)
	{
	case TestBenchOutKind::InoutKind:
	{
		return "inout";
	}
	case TestBenchOutKind::OutputKind:
	{
		return "output";
	}
	case TestBenchOutKind::OutputRegKind:
	{
		return "outputreg";
	}
	case TestBenchOutKind::HiZKind:
	{
		return "HiZ";
	}
	case TestBenchOutKind::We0Kind:
	{
		return "We0";
	}
	case TestBenchOutKind::We1Kind:
	{
		return "We1";
	}
	case TestBenchOutKind::WeXKind:
	{
		return "WeX";
	}
	case TestBenchOutKind::Pu0Kind:
	{
		return "Pu0";
	}
	case TestBenchOutKind::Pu1Kind:
	{
		return "Pu1";
	}
	case TestBenchOutKind::PuXKind:
	{
		return "PuX";
	}
	case TestBenchOutKind::St0Kind:
	{
		return "St0";
	}
	case TestBenchOutKind::St1Kind:
	{
		return "St1";
	}
	case TestBenchOutKind::StXKind:
	{
		return "StX";
	}
	case TestBenchOutKind::Su0Kind:
	{
		return "Su0";
	}
	case TestBenchOutKind::Su1Kind:
	{
		return "Su1";
	}
	case TestBenchOutKind::SuXKind:
	{
		return "SuX";
	}
	default:
	{
		break;
	}
	}
	return nullptr;
}

static TestBenchOutKind IsKeyword(const char *str) noexcept
{
	for (signed i = TestBenchOutKind::InoutKind; i <= TestBenchOutKind::SuXKind; i++)
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

	void reset() noexcept;

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
	void skipBlanks();
	void buildString(std::string &builder);
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

void TestBenchOutToken::reset() noexcept
{
	setToken(TestBenchOutKind::UnknownKind);
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
	return TestBenchOutKind::InoutKind <= m_kind && m_kind <= TestBenchOutKind::OutputRegKind;
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

static bool IsNumber(const std::string &s) noexcept
{
	for (char c : s)
	{
		if (!('0' <= c && c <= '9'))
		{
			return false;
		}
	}
	return true;
}

TestBenchOutToken TestBenchOutLexer::nextToken()
{
	// Builder for strings.
	std::string builder;

	// Future kind.
	TestBenchOutKind kind;

	// Skip all blank chars.
	skipBlanks();

	// Check if, EOF.
	if (m_char == lcsm::support::CharSource::EndOfSource)
	{
		m_token.setEof();
		goto l_finish;
	}

	// Continue parsing to blank character (non valid).
	buildString(builder);

	// Check, if keyword.
	kind = IsKeyword(builder);
	if (kind >= 0)
	{
		m_token.setToken(kind);
		goto l_finish;
	}

	// Check, if number.
	if (IsNumber(builder))
	{
		m_token.setToken(std::stoi(builder));
		goto l_finish;
	}

	// Otherwise, we've got an unknown token.
	m_token.reset();

l_finish:
	return m_token;
}

void TestBenchOutLexer::nextChar()
{
	m_char = m_source->next();
}

void TestBenchOutLexer::skipBlanks()
{
	while (std::isspace(m_char))
	{
		nextChar();
	}
}

void TestBenchOutLexer::buildString(std::string &builder)
{
	while (std::isalnum(m_char) || m_char == '_')
	{
		builder.push_back(m_char);
		nextChar();
	}
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
		const lcsm::verilog::PortDirectionType portDirectionType = portType.portDirectionType();
		std::size_t i = 0;
		for (std::string &identifier : portDeclaration.second)
		{
			m.m_ports.emplace_back(portDirectionType, i++);
			switch (portDirectionType)
			{
			case lcsm::verilog::PortDirectionType::UnknowPortDirectionType:
			{
				break;
			}
			case lcsm::verilog::PortDirectionType::InputPortDirection:
			{
				m.m_inputPorts.emplace_back(portType, std::move(identifier));
				break;
			}
			case lcsm::verilog::PortDirectionType::InoutPortDirection:
			{
				m.m_inoutPorts.emplace_back(portType, std::move(identifier));
				break;
			}
			case lcsm::verilog::PortDirectionType::OutputPortDirection:
			{
				m.m_outputPorts.emplace_back(portType, std::move(identifier));
				break;
			}
			}
		}
	}
	return m;
}

lcsm::verilog::Module::Module(const lcsm::verilog::Module &other) :
	m_identifier(other.m_identifier), m_sourceModule(other.m_sourceModule), m_inputPorts(other.m_inputPorts),
	m_inoutPorts(other.m_inoutPorts), m_outputPorts(other.m_outputPorts), m_ports(other.m_ports)
{
}

lcsm::verilog::Module::Module(lcsm::verilog::Module &&other) noexcept :
	m_identifier(std::move(other.m_identifier)), m_sourceModule(std::move(other.m_sourceModule)),
	m_inputPorts(std::move(other.m_inputPorts)), m_inoutPorts(std::move(other.m_inoutPorts)),
	m_outputPorts(std::move(other.m_outputPorts)), m_ports(std::move(other.m_ports))
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

static void SourcePortDeclaration(const std::vector< lcsm::verilog::Port > &ports, std::string &source)
{
	for (const lcsm::verilog::Port &port : ports)
	{
		source += port.verilogPortDeclaration() + ";\n";
	}
}

static void SourcePortAssignment(
	const std::unordered_map< lcsm::verilog::PortDirectionType, std::vector< lcsm::DataBits > > &datas,
	const std::vector< lcsm::verilog::Port > &ports,
	lcsm::verilog::PortDirectionType type,
	std::string &source)
{
	const std::unordered_map< lcsm::verilog::PortDirectionType, std::vector< lcsm::DataBits > >::const_iterator found =
		datas.find(type);
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

static void InitializeOutData(
	const std::vector< lcsm::verilog::Port > &ports,
	std::unordered_map< lcsm::verilog::PortDirectionType, std::vector< lcsm::DataBits > > &out,
	lcsm::verilog::PortDirectionType type)
{
	if (!ports.empty())
	{
		out[type] = std::vector< lcsm::DataBits >(ports.size());
		std::vector< lcsm::DataBits > &databits = out[type];
		for (std::size_t i = 0; i < ports.size(); i++)
		{
			const std::size_t width = ports[i].width();
			databits[i].setWidth(width);
		}
	}
}

std::unordered_map< lcsm::verilog::PortDirectionType, std::vector< lcsm::DataBits > >
	lcsm::verilog::Module::invoke(const std::unordered_map< lcsm::verilog::PortDirectionType, std::vector< lcsm::DataBits > > &testBenchInData) const
{
	// Step 1. Generate testbench module.
	std::string testBenchSource = m_sourceModule;

	// 1.1. Module declaration.
	testBenchSource += "\nmodule " + m_identifier + "_tb;\n";

	// 1.2. Port declaration.
	SourcePortDeclaration(m_inputPorts, testBenchSource);
	SourcePortDeclaration(m_inoutPorts, testBenchSource);
	SourcePortDeclaration(m_outputPorts, testBenchSource);

	// 1.3. Assignment.
	SourcePortAssignment(testBenchInData, m_inputPorts, lcsm::verilog::PortDirectionType::InputPortDirection, testBenchSource);
	SourcePortAssignment(testBenchInData, m_inoutPorts, lcsm::verilog::PortDirectionType::InoutPortDirection, testBenchSource);

	// 1.4. Module declare.
	testBenchSource += m_identifier + " tb(";
	bool needsComma = false;
	for (const std::pair< lcsm::verilog::PortDirectionType, std::size_t > &port : m_ports)
	{
		if (needsComma)
		{
			testBenchSource.push_back(',');
		}
		switch (port.first)
		{
		case lcsm::verilog::PortDirectionType::UnknowPortDirectionType:
		{
			throw std::logic_error("Found unknown port type, impossible!");
		}
		case lcsm::verilog::PortDirectionType::InputPortDirection:
		{
			testBenchSource += m_inputPorts[port.second].identifier();
			break;
		}
		case lcsm::verilog::PortDirectionType::InoutPortDirection:
		{
			testBenchSource += m_inoutPorts[port.second].identifier();
			break;
		}
		case lcsm::verilog::PortDirectionType::OutputPortDirection:
		{
			testBenchSource += m_outputPorts[port.second].identifier();
			break;
		}
		}
		needsComma = true;
	}
	testBenchSource += ");\n";

	// 1.5. Monitor.
	testBenchSource += "initial begin\n";
	std::size_t t = 1;
	for (const std::pair< lcsm::verilog::PortDirectionType, std::size_t > &port : m_ports)
	{
		const lcsm::verilog::Port *p = nullptr;

		switch (port.first)
		{
		case lcsm::verilog::PortDirectionType::UnknowPortDirectionType:
		{
			throw std::logic_error("Found unknown port type, impossible!");
		}
		case lcsm::verilog::PortDirectionType::InputPortDirection:
		{
			// Skip INPUT, as there is no any thoughts to parse input's value.
			continue;
		}
		case lcsm::verilog::PortDirectionType::InoutPortDirection:
		{
			p = std::addressof(m_inoutPorts[port.second]);
			break;
		}
		case lcsm::verilog::PortDirectionType::OutputPortDirection:
		{
			p = std::addressof(m_outputPorts[port.second]);
			break;
		}
		}

		for (std::size_t w = 0; w < p->width(); w++)
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

	const lcsm::support::CompletedProcess iVerilogProcCompleted =
		lcsm::support::subprocessRun("iverilog", { "-g2012", "-o", outputFilename, sourceFilename });
	iVerilogProcCompleted.checkReturnCode();

	const lcsm::support::CompletedProcess vppProcCompleted = lcsm::support::subprocessRun("vvp", { outputFilename });
	vppProcCompleted.checkReturnCode();

	// Step 4. Remove temporary files.
	lcsm::support::removeFile(outputFilename);
	lcsm::support::removeFile(sourceFilename);

	// Step 5. Parse standard output and return as output data.
	std::shared_ptr< lcsm::support::CharSource > source =
		std::make_shared< lcsm::support::CStringSource >(vppProcCompleted.cOut());
	TestBenchOutLexer lex = source;

	// Output data for physical model.
	std::unordered_map< lcsm::verilog::PortDirectionType, std::vector< lcsm::DataBits > > testBenchOutData;

	// Initialize output data.
	InitializeOutData(m_inoutPorts, testBenchOutData, lcsm::verilog::PortDirectionType::InoutPortDirection);
	InitializeOutData(m_outputPorts, testBenchOutData, lcsm::verilog::PortDirectionType::OutputPortDirection);

	// Generated output will be like this:
	// <type> <global index> <local index> <value>, where:
	// <type>                           --> 'input' | 'inout' | 'output' | 'outputreg'
	// <global index> and <local index> --> INTEGER
	// <value>                          --> 'HiZ' | 'We0' | 'We1' | 'Pu0' | 'Pu1' | 'St0' | 'St1' | 'Su0' | 'Su1'
	// end is --> EndOfSource.
	while (!lex.nextToken().isEof())
	{
		// All parsed things.
		std::size_t globalIndex;
		std::size_t localIndex;
		lcsm::verilog::Bit bit;
		lcsm::verilog::Strength strength;
		lcsm::verilog::PortDirectionType type;

		// <type> --> 'inout' | 'output' | 'outputreg'
		switch (lex.token().kind())
		{
		case TestBenchOutKind::InoutKind:
		{
			type = lcsm::verilog::PortDirectionType::InoutPortDirection;
			break;
		}
		case TestBenchOutKind::OutputKind:
		{
			type = lcsm::verilog::PortDirectionType::OutputPortDirection;
			break;
		}
		default:
			throw std::logic_error(
				"Expected 'inout' | 'output' | in generated test bench as first entity "
				"in line!");
		}

		// <global index> --> INTEGER
		if (!lex.nextToken().isInteger())
		{
			throw std::logic_error(
				"Expected integer value in generated test bench as second entity "
				"in line!");
		}
		else
		{
			globalIndex = static_cast< std::size_t >(lex.token().asInteger());
		}

		// <local index> --> INTEGER
		if (!lex.nextToken().isInteger())
		{
			throw std::logic_error(
				"Expected integer value in generated test bench as third entity "
				"in line!");
		}
		else
		{
			localIndex = static_cast< std::size_t >(lex.token().asInteger());
		}

		// <value> --> 'HiZ' | 'We0' | 'We1' | 'Pu0' | 'Pu1' | 'St0' | 'St1' | 'Su0' | 'Su1'
		switch (lex.nextToken().kind())
		{
		case TestBenchOutKind::HiZKind:
		{
			strength = lcsm::verilog::Strength::HighImpedance;
			bit = lcsm::verilog::Bit::Undefined;
			break;
		}
		case TestBenchOutKind::We0Kind:
		{
			strength = lcsm::verilog::Strength::WeakDrive;
			bit = lcsm::verilog::Bit::False;
			break;
		}
		case TestBenchOutKind::We1Kind:
		{
			strength = lcsm::verilog::Strength::WeakDrive;
			bit = lcsm::verilog::Bit::True;
			break;
		}
		case TestBenchOutKind::WeXKind:
		{
			strength = lcsm::verilog::Strength::WeakDrive;
			bit = lcsm::verilog::Bit::Undefined;
			break;
		}
		case TestBenchOutKind::Pu0Kind:
		{
			strength = lcsm::verilog::Strength::PullDrive;
			bit = lcsm::verilog::Bit::False;
			break;
		}
		case TestBenchOutKind::Pu1Kind:
		{
			strength = lcsm::verilog::Strength::PullDrive;
			bit = lcsm::verilog::Bit::True;
			break;
		}
		case TestBenchOutKind::PuXKind:
		{
			strength = lcsm::verilog::Strength::PullDrive;
			bit = lcsm::verilog::Bit::Undefined;
			break;
		}
		case TestBenchOutKind::St0Kind:
		{
			strength = lcsm::verilog::Strength::StrongDrive;
			bit = lcsm::verilog::Bit::False;
			break;
		}
		case TestBenchOutKind::St1Kind:
		{
			strength = lcsm::verilog::Strength::StrongDrive;
			bit = lcsm::verilog::Bit::True;
			break;
		}
		case TestBenchOutKind::StXKind:
		{
			strength = lcsm::verilog::Strength::StrongDrive;
			bit = lcsm::verilog::Bit::Undefined;
			break;
		}
		case TestBenchOutKind::Su0Kind:
		{
			strength = lcsm::verilog::Strength::SupplyDrive;
			bit = lcsm::verilog::Bit::False;
			break;
		}
		case TestBenchOutKind::Su1Kind:
		{
			strength = lcsm::verilog::Strength::SupplyDrive;
			bit = lcsm::verilog::Bit::True;
			break;
		}
		case TestBenchOutKind::SuXKind:
		{
			strength = lcsm::verilog::Strength::SupplyDrive;
			bit = lcsm::verilog::Bit::Undefined;
			break;
		}
		default:
			throw std::logic_error(
				"Expected 'HiZ' | 'We0' | 'We1' | 'WeX' | 'Pu0' | 'Pu1' | 'PuX' | 'St0' | 'St1' | 'StX' | 'Su0' | "
				"'Su1' | 'SuX' in generated test "
				"bench as fourth entity "
				"in line!");
		}

		// Set.
		testBenchOutData[type][globalIndex].set(localIndex, bit, strength);
	}

	return testBenchOutData;
}

const std::string &lcsm::verilog::Module::identifier() const noexcept
{
	return m_identifier;
}
