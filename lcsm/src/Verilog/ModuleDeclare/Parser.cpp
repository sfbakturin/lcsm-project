#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Verilog/ModuleDeclare/Context.h>
#include <lcsm/Verilog/ModuleDeclare/Lexer.h>
#include <lcsm/Verilog/ModuleDeclare/Parser.h>
#include <lcsm/Verilog/ModuleDeclare/Token.h>
#include <lcsm/Verilog/Port.h>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::verilog::ModuleDeclareParser::ModuleDeclareParser(const std::shared_ptr< lcsm::support::CharSource > &source) :
	m_lex(source)
{
}

lcsm::verilog::ModuleDeclareParser::ModuleDeclareParser(std::shared_ptr< lcsm::support::CharSource > &&source) noexcept :
	m_lex(std::move(source))
{
}

lcsm::verilog::ModuleDeclareParser::ModuleDeclareParser(lcsm::verilog::ModuleDeclareParser &&other) noexcept :
	m_lex(std::move(other.m_lex))
{
}

lcsm::verilog::ModuleDeclareParser &lcsm::verilog::ModuleDeclareParser::operator=(lcsm::verilog::ModuleDeclareParser &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::verilog::ModuleDeclareParser >(this, std::move(other));
}

void lcsm::verilog::ModuleDeclareParser::swap(lcsm::verilog::ModuleDeclareParser &other)
{
	std::swap(m_lex, other.m_lex);
}

static void ruleRange(lcsm::verilog::PortType &portType, lcsm::verilog::ModuleDeclareLexer &lex, bool isOptional = false)
{
	// range --> LSQR INT COLON INT RSQR
	if (lex.nextToken() != lcsm::verilog::ModuleDeclareKind::KW_LSQR)
	{
		if (isOptional)
		{
			lex.backToken();
			return;
		}
		else
		{
			throw std::logic_error("Parser error at trying to parse range!");
		}
	}

	// For future constructed range.
	int left = 0;
	int right = 0;

	if (!lex.nextToken().isInteger())
	{
		throw std::logic_error("Parser error at trying to parse range!");
	}
	else
	{
		left = lex.token().asInteger();
	}

	if (lex.nextToken() != lcsm::verilog::ModuleDeclareKind::KW_COLON)
	{
		throw std::logic_error("Parser error at trying to parse range!");
	}

	if (!lex.nextToken().isInteger())
	{
		throw std::logic_error("Parser error at trying to parse range!");
	}
	else
	{
		right = lex.token().asInteger();
	}

	if (lex.nextToken() != lcsm::verilog::ModuleDeclareKind::KW_RSQR)
	{
		throw std::logic_error("Parser error at trying to parse range!");
	}

	portType.setRange({ left, right });
}

static std::vector< std::string > ruleListOfPortIdentifiers(lcsm::verilog::ModuleDeclareLexer &lex)
{
	// List of port identifiers.
	std::vector< std::string > identifiers;

	// Parse list_of_port_identifiers.
	if (!lex.nextToken().isIdentifier())
	{
		throw std::logic_error("Parser error at trying to parse list_of_port_identifiers!");
	}
	else
	{
		identifiers.push_back(lex.token().asIdentifier());
	}

	while (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_COMMA)
	{
		lex.nextToken();

		// This is port_identifier.
		if (lex.token().isIdentifier())
		{
			identifiers.push_back(lex.token().asIdentifier());
		}
		else
		{
			// Otherwise, look at kind of token and check, then return back.
			switch (lex.token().kind())
			{
			case lcsm::verilog::ModuleDeclareKind::KW_INOUT:
			case lcsm::verilog::ModuleDeclareKind::KW_INPUT:
			case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT:
			{
				lex.backToken();
				return identifiers;
			}
			default:
				break;
			}
			throw std::logic_error("Parser error at trying to parse list_of_port_identifiers!");
		}
	}

	// Return back for other rules.
	lex.backToken();

	return identifiers;
}

static bool ruleNetType(lcsm::verilog::PortType &portType, lcsm::verilog::ModuleDeclareLexer &lex, bool isOptional = false)
{
	// net_type --> SUPPLY0 | SUPPLY1 | TRI | TRIAND | TRIOR | TRIREG | TRE0 | TRI1 | UWIRE | WIRE | WAND | WOR

	switch (lex.nextToken().kind())
	{
	case lcsm::verilog::ModuleDeclareKind::KW_SUPPLY0:
	{
		portType.setNetType(lcsm::verilog::Supply0Net);
		return true;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_SUPPLY1:
	{
		portType.setNetType(lcsm::verilog::Supply1Net);
		return true;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TRI:
	{
		portType.setNetType(lcsm::verilog::TriNet);
		return true;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TRI0:
	{
		portType.setNetType(lcsm::verilog::Tri0Net);
		return true;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TRI1:
	{
		portType.setNetType(lcsm::verilog::Tri1Net);
		return true;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TRIAND:
	{
		portType.setNetType(lcsm::verilog::TriandNet);
		return true;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TRIOR:
	{
		portType.setNetType(lcsm::verilog::TriorNet);
		return true;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_UWIRE:
	{
		portType.setNetType(lcsm::verilog::UwireNet);
		return true;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_WIRE:
	{
		portType.setNetType(lcsm::verilog::WireNet);
		return true;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_WAND:
	{
		portType.setNetType(lcsm::verilog::WireNet);
		return true;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_WOR:
	{
		portType.setNetType(lcsm::verilog::WorNet);
		return true;
	}
	default:
	{
		break;
	}
	}

	if (isOptional)
	{
		lex.backToken();
		return false;
	}

	throw std::logic_error("Parser error at trying to parse net_type!");
}

static bool ruleSigning(lcsm::verilog::PortType &portType, lcsm::verilog::ModuleDeclareLexer &lex, bool isOptional = false)
{
	// signing --> SIGNED | UNSIGNED

	switch (lex.nextToken().kind())
	{
	case lcsm::verilog::ModuleDeclareKind::KW_SIGNED:
	{
		portType.setIsSigned(true);
		return true;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_UNSIGNED:
	{
		portType.setIsSigned(false);
		return true;
	}
	default:
	{
		break;
	}
	}

	if (isOptional)
	{
		lex.backToken();
		return false;
	}

	throw std::logic_error("Parser error at trying to parse signing!");
}

static void ruleImplicitDataType(lcsm::verilog::PortType &portType, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// implicit_data_type --> (signing)? (range)?

	// 1. (signing)?
	ruleSigning(portType, lex, true);

	// 2. (range)?
	ruleRange(portType, lex, true);
}

static void ruleIntegerVectorType(lcsm::verilog::PortType &portType, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// integer_vector_type --> BIT | LOGIC | REG

	switch (lex.nextToken().kind())
	{
	case lcsm::verilog::ModuleDeclareKind::KW_BIT:
	{
		portType.setIntegerVectorType(lcsm::verilog::IntegerVectorType::BitIntegerVector);
		return;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_LOGIC:
	{
		portType.setIntegerVectorType(lcsm::verilog::IntegerVectorType::LogicIntegerVector);
		return;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_REG:
	{
		portType.setIntegerVectorType(lcsm::verilog::IntegerVectorType::RegIntegerVector);
		return;
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Parser error at trying to parse integer_vector_type!");
}

static void ruleDataType(lcsm::verilog::PortType &portType, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// data_type --> integer_vector_type (signing)? (range)?

	// 1. integer_vector_type
	ruleIntegerVectorType(portType, lex);

	// 2. (signing)?
	ruleSigning(portType, lex, true);

	// 3. (range)?
	ruleRange(portType, lex, true);
}

static void ruleDataTypeOrImplicit(lcsm::verilog::PortType &portType, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// data_type_or_implicit --> data_type | implicit_data_type
	// data_type             --> integer_vector_type (signing)? (range)?
	// implicit_data_type    --> (signing)? (range)?

	// 1. Check for (signing)? - if found, then parse it as implicit_data_type, otherwise - as data_type.
	switch (lex.nextToken().kind())
	{
	case lcsm::verilog::ModuleDeclareKind::KW_SIGNED:
	case lcsm::verilog::ModuleDeclareKind::KW_UNSIGNED:
	{
		// 2. This is implicit_data_type. Return back token for next rule and perform rule.
		lex.backToken();
		ruleImplicitDataType(portType, lex);
		return;
	}
	default:
	{
		// Return back token.
		lex.backToken();
		break;
	}
	}

	// 2. Check for (range)? - if fount, then parse it as implicit_data_type, otherwise - as data_type.
	if (lex.nextToken() == lcsm::verilog::ModuleDeclareKind::KW_LSQR)
	{
		// Return token back for rule.
		lex.backToken();
		ruleImplicitDataType(portType, lex);
		return;
	}
	else
	{
		// Return token back.
		lex.backToken();
	}

	// 2.This might be data_type, we should check integer_vector_type first.
	switch (lex.nextToken().kind())
	{
	case lcsm::verilog::ModuleDeclareKind::KW_BIT:
	case lcsm::verilog::ModuleDeclareKind::KW_LOGIC:
	case lcsm::verilog::ModuleDeclareKind::KW_REG:
	{
		// This is data_type. Return token for rule and perform parsing.
		lex.backToken();
		ruleDataType(portType, lex);
		return;
	}
	default:
	{
		// Return token back.
		lex.backToken();
		break;
	}
	}
}

static void ruleNetPortType(lcsm::verilog::PortType &portType, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// net_port_type --> (net_type)? data_type_or_implicit

	// 1. (net_type)?
	ruleNetType(portType, lex, true);

	// 2. data_type_or_implicit.
	ruleDataTypeOrImplicit(portType, lex);
}

static void ruleInoutDeclaration(lcsm::verilog::ModuleDeclareContext &context, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// Port type.
	lcsm::verilog::PortType portType;

	// inout_declaration --> INOUT net_port_type list_of_port_identifiers

	// 1. INOUT
	if (lex.nextToken() != lcsm::verilog::ModuleDeclareKind::KW_INOUT)
	{
		throw std::logic_error("Parser error at trying to parse inout_declaration!");
	}
	else
	{
		portType.setPortDirectionType(lcsm::verilog::PortDirectionType::InoutPortDirection);
	}

	// 2. net_port_type.
	ruleNetPortType(portType, lex);

	// 3. list_of_port_identifiers
	context.addPortDeclaration(portType, ruleListOfPortIdentifiers(lex));
}

static void ruleInputDeclaration(lcsm::verilog::ModuleDeclareContext &context, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// Port type.
	lcsm::verilog::PortType portType;

	// input_declaration --> INPUT net_port_type list_of_port_identifiers

	// 1. INPUT
	if (lex.nextToken() != lcsm::verilog::ModuleDeclareKind::KW_INPUT)
	{
		throw std::logic_error("Parser error at trying to parse input_declaration!");
	}
	else
	{
		portType.setPortDirectionType(lcsm::verilog::PortDirectionType::InputPortDirection);
	}

	// 2. net_port_type.
	ruleNetPortType(portType, lex);

	// 3. list_of_port_identifiers
	context.addPortDeclaration(portType, ruleListOfPortIdentifiers(lex));
}

static void ruleOutputDeclaration(lcsm::verilog::ModuleDeclareContext &context, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// Port type.
	lcsm::verilog::PortType portType;

	// output_declaration --> OUTPUT net_port_type list_of_port_identifiers

	// 1. OUTPUT
	if (lex.nextToken() != lcsm::verilog::ModuleDeclareKind::KW_OUTPUT)
	{
		throw std::logic_error("Parser error at trying to parse input_declaration!");
	}
	else
	{
		portType.setPortDirectionType(lcsm::verilog::PortDirectionType::OutputPortDirection);
	}

	// 2. net_port_type.
	ruleNetPortType(portType, lex);

	// 3. list_of_port_identifiers
	context.addPortDeclaration(portType, ruleListOfPortIdentifiers(lex));
}

static void rulePortDeclaration(lcsm::verilog::ModuleDeclareContext &context, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// port_declaration   --> inout_declaration | input_declaration | output_declaration
	// inout_declaration  --> INOUT ...
	// input_declaration  --> INPUT ...
	// output_declaration --> OUTPUT ... | OUTPUT_REG ...
	switch (lex.nextToken().kind())
	{
	case lcsm::verilog::ModuleDeclareKind::KW_INOUT:
	{
		lex.backToken();
		ruleInoutDeclaration(context, lex);
		return;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_INPUT:
	{
		lex.backToken();
		ruleInputDeclaration(context, lex);
		return;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT:
	{
		lex.backToken();
		ruleOutputDeclaration(context, lex);
		return;
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Parser error at trying to parse port_declaration!");
}

static void ruleListOfPortDeclarations(lcsm::verilog::ModuleDeclareContext &context, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// list_of_port_declarations --> LPAREN (port_declaration  (COMMA port_declaration)*)? RPAREN

	// 1. LPAREN
	if (lex.nextToken() != lcsm::verilog::ModuleDeclareKind::KW_LPAREN)
	{
		throw std::logic_error("Parser error at trying to parse list_of_port_declarations!");
	}

	// 2.1. RPAREN
	if (lex.nextToken() == lcsm::verilog::ModuleDeclareKind::KW_RPAREN)
	{
		return;
	}
	{
		// If this is not RPAREN, then return token.
		lex.backToken();
	}

	// 2.2.1. (port_declaration (COMMA port_declaration)*)?
	rulePortDeclaration(context, lex);
	while (true)
	{
		switch (lex.nextToken().kind())
		{
		case lcsm::verilog::ModuleDeclareKind::KW_RPAREN:
		{
			lex.backToken();
			goto l_finish;
		}
		default:
		{
			lex.backToken();
			rulePortDeclaration(context, lex);
			break;
		}
		}
	}

	// 2.2.2. RPAREN
l_finish:
	if (lex.nextToken().kind() != lcsm::verilog::ModuleDeclareKind::KW_RPAREN)
	{
		throw std::logic_error("Parser error at trying to parse list_of_port_declarations!");
	}
}

static void ruleModuleIdentifier(lcsm::verilog::ModuleDeclareContext &context, lcsm::verilog::ModuleDeclareLexer &lex)
{
	const lcsm::verilog::ModuleDeclareToken &tok = lex.nextToken();
	if (!tok.isIdentifier())
	{
		throw std::logic_error("Parser error at trying to parse module_identifier!");
	}
	else
	{
		context.setModuleIdentifier(lex.token().asIdentifier());
	}
}

static void ruleModuleKeyword(lcsm::verilog::ModuleDeclareLexer &lex)
{
	const lcsm::verilog::ModuleDeclareToken &tok = lex.nextToken();
	if (tok.kind() != lcsm::verilog::ModuleDeclareKind::KW_MODULE)
	{
		throw std::logic_error("Parser error at trying to parse module_keyword!");
	}
}

static void ruleModuleDeclaration(lcsm::verilog::ModuleDeclareContext &context, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// module_declaration -> module_keyword module_identifier list_of_port_declarations SEMI

	// 1. module_keyword
	ruleModuleKeyword(lex);

	// 2. module_identifier
	ruleModuleIdentifier(context, lex);

	// 3. list_of_port_declarations
	ruleListOfPortDeclarations(context, lex);

	// 4. SEMI.
	const lcsm::verilog::ModuleDeclareToken &tok = lex.nextToken();
	if (tok.kind() != lcsm::verilog::ModuleDeclareKind::KW_SEMI)
	{
		throw std::logic_error("Parser error at trying to parse SEMI!");
	}
}

void lcsm::verilog::ModuleDeclareParser::parse(lcsm::verilog::ModuleDeclareContext &context)
{
	ruleModuleDeclaration(context, m_lex);
}
