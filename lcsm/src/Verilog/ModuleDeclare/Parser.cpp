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

lcsm::verilog::ModuleDeclareParser::ModuleDeclareParser(std::shared_ptr< lcsm::support::CharSource > &&source) :
	m_lex(std::move(source))
{
}

lcsm::verilog::ModuleDeclareParser::ModuleDeclareParser(lcsm::verilog::ModuleDeclareParser &&other) :
	m_lex(std::move(other.m_lex))
{
}

lcsm::verilog::ModuleDeclareParser &lcsm::verilog::ModuleDeclareParser::operator=(lcsm::verilog::ModuleDeclareParser &&other)
{
	return lcsm::support::MoveAssign< lcsm::verilog::ModuleDeclareParser >(this, std::move(other));
}

void lcsm::verilog::ModuleDeclareParser::swap(lcsm::verilog::ModuleDeclareParser &other)
{
	std::swap(m_lex, other.m_lex);
}

static void ruleRange(lcsm::verilog::PortType &portType, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// range --> LSQR INT COLON INT RSQR
	if (lex.nextToken().kind() != lcsm::verilog::ModuleDeclareKind::KW_LSQR)
	{
		throw std::logic_error("Parser error at trying to parse range!");
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

	if (lex.nextToken().kind() != lcsm::verilog::ModuleDeclareKind::KW_COLON)
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

	if (lex.nextToken().kind() != lcsm::verilog::ModuleDeclareKind::KW_RSQR)
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
		throw std::logic_error("Parser error at trying to parse inout_declaration!");
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
			// Otherwise, return back.
			lex.backToken();
			return identifiers;
		}
	}

	// Return back for other rules.
	lex.backToken();

	return identifiers;
}

static void ruleInoutDeclaration(lcsm::verilog::ModuleDeclareContext &context, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// Port type.
	lcsm::verilog::PortType portType;

	// inout_declaration --> INOUT (net_type)? (SIGNED)? (range)? list_of_port_identifiers
	if (lex.nextToken().kind() != lcsm::verilog::ModuleDeclareKind::KW_INOUT)
	{
		throw std::logic_error("Parser error at trying to parse inout_declaration!");
	}
	else
	{
		portType.setIOType(lcsm::verilog::IOType::Inout);
	}

	// Check keywords: (net_type)? (SIGNED)? <start of (range)?>
	lex.nextToken();
	// Check net_type --> SUPPLY0 | SUPPLY1 | TRI | TRIAND | TRIOR | TRI0 | TRI1 | UWIRE | WIRE | WAND | WOR
	switch (lex.token().kind())
	{
	case lcsm::verilog::KW_SUPPLY0:
	{
		portType.setNetType(lcsm::verilog::NetType::Supply0);
		break;
	}
	case lcsm::verilog::KW_SUPPLY1:
	{
		portType.setNetType(lcsm::verilog::NetType::Supply1);
		break;
	}
	case lcsm::verilog::KW_TRI:
	{
		portType.setNetType(lcsm::verilog::NetType::Tri);
		break;
	}
	case lcsm::verilog::KW_TRI0:
	{
		portType.setNetType(lcsm::verilog::NetType::Tri0);
		break;
	}
	case lcsm::verilog::KW_TRI1:
	{
		portType.setNetType(lcsm::verilog::NetType::Tri1);
		break;
	}
	case lcsm::verilog::KW_TRIAND:
	{
		portType.setNetType(lcsm::verilog::NetType::Triand);
		break;
	}
	case lcsm::verilog::KW_TRIOR:
	{
		portType.setNetType(lcsm::verilog::NetType::Trior);
		break;
	}
	case lcsm::verilog::KW_UWIRE:
	{
		portType.setNetType(lcsm::verilog::NetType::Uwire);
		break;
	}
	case lcsm::verilog::KW_WIRE:
	{
		portType.setNetType(lcsm::verilog::NetType::Wire);
		break;
	}
	case lcsm::verilog::KW_WAND:
	{
		portType.setNetType(lcsm::verilog::NetType::Wand);
		break;
	}
	case lcsm::verilog::KW_WOR:
	{
		portType.setNetType(lcsm::verilog::NetType::Wor);
		break;
	}
	default:
	{
		// Return back.
		lex.backToken();
		break;
	}
	}

	// Check SIGNED
	if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_SIGNED)
	{
		portType.setIsSigned(true);
	}
	else
	{
		// Return back.
		lex.backToken();
	}

	// Check range.
	if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_RSQR)
	{
		// Return back token for range rule.
		lex.backToken();
		// Parse range.
		ruleRange(portType, lex);
	}
	else
	{
		// Return back.
		lex.backToken();
	}

	// Parse list_of_port_identifiers.
	context.addPortDeclaration(portType, ruleListOfPortIdentifiers(lex));
}

static void ruleInputDeclaration(lcsm::verilog::ModuleDeclareContext &context, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// Port type.
	lcsm::verilog::PortType portType;

	// inout_declaration --> INOUT (net_type)? (SIGNED)? (range)? list_of_port_identifiers
	if (lex.nextToken().kind() != lcsm::verilog::ModuleDeclareKind::KW_INPUT)
	{
		throw std::logic_error("Parser error at trying to parse inout_declaration!");
	}
	else
	{
		portType.setIOType(lcsm::verilog::IOType::Input);
	}

	// Check keywords: (net_type)? (SIGNED)? <start of (range)?>
	// Check net_type --> SUPPLY0 | SUPPLY1 | TRI | TRIAND | TRIOR | TRI0 | TRI1 | UWIRE | WIRE | WAND | WOR
	switch (lex.nextToken().kind())
	{
	case lcsm::verilog::KW_SUPPLY0:
	{
		portType.setNetType(lcsm::verilog::NetType::Supply0);
		break;
	}
	case lcsm::verilog::KW_SUPPLY1:
	{
		portType.setNetType(lcsm::verilog::NetType::Supply1);
		break;
	}
	case lcsm::verilog::KW_TRI:
	{
		portType.setNetType(lcsm::verilog::NetType::Tri);
		break;
	}
	case lcsm::verilog::KW_TRI0:
	{
		portType.setNetType(lcsm::verilog::NetType::Tri0);
		break;
	}
	case lcsm::verilog::KW_TRI1:
	{
		portType.setNetType(lcsm::verilog::NetType::Tri1);
		break;
	}
	case lcsm::verilog::KW_TRIAND:
	{
		portType.setNetType(lcsm::verilog::NetType::Triand);
		break;
	}
	case lcsm::verilog::KW_TRIOR:
	{
		portType.setNetType(lcsm::verilog::NetType::Trior);
		break;
	}
	case lcsm::verilog::KW_UWIRE:
	{
		portType.setNetType(lcsm::verilog::NetType::Uwire);
		break;
	}
	case lcsm::verilog::KW_WIRE:
	{
		portType.setNetType(lcsm::verilog::NetType::Wire);
		break;
	}
	case lcsm::verilog::KW_WAND:
	{
		portType.setNetType(lcsm::verilog::NetType::Wand);
		break;
	}
	case lcsm::verilog::KW_WOR:
	{
		portType.setNetType(lcsm::verilog::NetType::Wor);
		break;
	}
	default:
	{
		lex.backToken();
		break;
	}
	}

	// Check SIGNED
	if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_SIGNED)
	{
		portType.setIsSigned(true);
	}
	else
	{
		lex.backToken();
	}

	// Check range.
	if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_LSQR)
	{
		lex.backToken();
		ruleRange(portType, lex);
	}
	else
	{
		lex.backToken();
	}

	// Parse list_of_port_identifiers.
	context.addPortDeclaration(portType, ruleListOfPortIdentifiers(lex));
}

static void ruleOutputVariableType(lcsm::verilog::PortType &portType, lcsm::verilog::ModuleDeclareLexer &lex)
{
	switch (lex.nextToken().kind())
	{
	case lcsm::verilog::ModuleDeclareKind::KW_INTEGER:
	{
		portType.setOutputVariableType(lcsm::verilog::OutputVariableType::Integer);
		return;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_TIME:
	{
		portType.setOutputVariableType(lcsm::verilog::OutputVariableType::Time);
		return;
	}
	default:
	{
		break;
	}
	}
	throw std::logic_error("Parser error at trying to parse output_variable_type!");
}

static void ruleListOfVariablePortIdentifiers(lcsm::verilog::ModuleDeclareLexer &lex)
{
	// list_of_variable_port_identifiers --> port_identifier (EQUAL constant_expression)? (COMMA port_identifier (EQUAL
	// constant_expression)?)*
	if (!lex.nextToken().isIdentifier())
	{
		throw std::logic_error("Parser error at trying to parse list_of_variable_port_identifiers!");
	}

	// (EQUAL constant_expression)?
	if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_EQUAL)
	{
		if (!lex.nextToken().isInteger())
		{
			throw std::logic_error("Parser error at trying to parse list_of_variable_port_identifiers!");
		}
	}
	else
	{
		// Return back.
		lex.backToken();
	}

	// (COMMA port_identifier (EQUAL constant_expression)?)*
	// FIXME: Might fail at ", input"
	while (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_COMMA)
	{
		// port_identifier
		lex.nextToken();
		if (lex.token().isIdentifier())
		{
			goto l_parse_constant_expression;
		}
		else
		{
			// Otherwise, return back.
			lex.backToken();
			return;
		}

		// (EQUAL constant_expression)?
l_parse_constant_expression:
		if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_EQUAL)
		{
			if (!lex.nextToken().isInteger())
			{
				throw std::logic_error("Parser error at trying to parse list_of_variable_port_identifiers!");
			}
		}
		else
		{
			// Return back.
			lex.backToken();
		}
	}

	// Return back for other rules.
	lex.backToken();
}

static void ruleOutputDeclaration(lcsm::verilog::ModuleDeclareContext &context, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// Port type.
	lcsm::verilog::PortType portType;

	// output_declaration --> OUTPUT (net_type)? (SIGNED)? (range)? list_of_port_identifiers |
	//                        OUTPUT_REG (SIGNED)? (range)? list_of_port_identifiers         |
	//                        OUTPUT output_variable_type list_of_variable_port_identifiers
	switch (lex.nextToken().kind())
	{
	case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT:
	{
		// Check keywords: (net_type)?
		portType.setIOType(lcsm::verilog::IOType::Output);
		switch (lex.nextToken().kind())
		{
		case lcsm::verilog::KW_SUPPLY0:
		case lcsm::verilog::KW_SUPPLY1:
		case lcsm::verilog::KW_TRI:
		case lcsm::verilog::KW_TRI0:
		case lcsm::verilog::KW_TRI1:
		case lcsm::verilog::KW_TRIAND:
		case lcsm::verilog::KW_TRIOR:
		case lcsm::verilog::KW_UWIRE:
		case lcsm::verilog::KW_WIRE:
		case lcsm::verilog::KW_WAND:
		case lcsm::verilog::KW_WOR:
		{
			// Then we should now parse (SIGNED)? (range)? list_of_port_identifiers

			// Check SIGNED.
			if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_SIGNED)
			{
				portType.setIsSigned(true);
			}
			else
			{
				lex.backToken();
			}

			// Check range.
			if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_LSQR)
			{
				lex.backToken();
				ruleRange(portType, lex);
			}
			else
			{
				lex.backToken();
			}

			// Parse list_of_port_identifiers.
			context.addPortDeclaration(portType, ruleListOfPortIdentifiers(lex));

			return;
		}
		default:
		{
			lex.backToken();
			break;
		}
		}

		// Check SIGNED.
		if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_SIGNED)
		{
			portType.setIsSigned(true);
		}
		else
		{
			lex.backToken();
		}

		// Check range.
		if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_LSQR)
		{
			lex.backToken();
			ruleRange(portType, lex);
		}
		else
		{
			lex.backToken();
		}

		// Choose between list_of_port_identifiers | output_variable_type.
		// Check output_variable_type.
		lex.nextToken();
		if (lex.token().kind() != lcsm::verilog::ModuleDeclareKind::KW_INTEGER &&
			lex.token().kind() != lcsm::verilog::ModuleDeclareKind::KW_TIME)
		{
			// Return back for list_of_port_identifiers.
			lex.backToken();
			// Parse list_of_port_identifiers.
			context.addPortDeclaration(portType, ruleListOfPortIdentifiers(lex));
			return;
		}
		else
		{
			// Return back for output_variable_type.
			lex.backToken();

			// Parse output_variable_type.
			ruleOutputVariableType(portType, lex);

			// Parse list_of_variable_port_identifiers
			ruleListOfVariablePortIdentifiers(lex);

			return;
		}

		break;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT_REG:
	{
		portType.setIOType(lcsm::verilog::IOType::OutputReg);

		// Check SIGNED.
		if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_SIGNED)
		{
			portType.setIsSigned(true);
		}
		else
		{
			lex.backToken();
		}

		// Check range.
		if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_LSQR)
		{
			// Return back for range.
			lex.backToken();
			// Parse range.
			ruleRange(portType, lex);
		}
		else
		{
			// Return back.
			lex.backToken();
		}

		// Parse list_of_port_identifiers.
		context.addPortDeclaration(portType, ruleListOfPortIdentifiers(lex));

		return;
	}
	default:
		break;
	}

	throw std::logic_error("Parser error at trying to parse output_declaration!");
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
	case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT_REG:
	{
		lex.backToken();
		ruleOutputDeclaration(context, lex);
		return;
	}
	default:
		throw std::logic_error("Parser error at trying to parse port_declarations!");
	}

	throw std::logic_error("Parser error at trying to parse port_declarations!");
}

static void ruleListOfPortDeclarations(lcsm::verilog::ModuleDeclareContext &context, lcsm::verilog::ModuleDeclareLexer &lex)
{
	// list_of_port_declarations --> LPAREN (port_declaration  (COMMA port_declaration)*)? RPAREN

	// 1. LPAREN
	if (lex.nextToken().kind() != lcsm::verilog::ModuleDeclareKind::KW_LPAREN)
	{
		throw std::logic_error("Parser error at trying to parse list_of_port_declarations!");
	}

	// 2.1. RPAREN
	if (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_RPAREN)
	{
		return;
	}
	{
		// If this is not RPAREN, then return token.
		lex.backToken();
	}

	// 2.2.1. (port_declaration (COMMA port_declaration)*)?
	rulePortDeclaration(context, lex);
	// while (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_COMMA)
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
