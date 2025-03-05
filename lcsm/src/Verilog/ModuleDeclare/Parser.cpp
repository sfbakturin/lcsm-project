#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Verilog/ModuleDeclare/Lexer.h>
#include <lcsm/Verilog/ModuleDeclare/Parser.h>
#include <lcsm/Verilog/ModuleDeclare/Token.h>

#include <fstream>
#include <ios>
#include <stdexcept>
#include <utility>

lcsm::verilog::ModuleDeclareParser::ModuleDeclareParser(const char *filename) :
	m_lex(std::ifstream{ filename, std::ios::in })
{
}

lcsm::verilog::ModuleDeclareParser::ModuleDeclareParser(std::ifstream &&stream) : m_lex(std::move(stream)) {}

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

static void ruleRange(lcsm::verilog::ModuleDeclareLexer &lex, bool needToParseNextToken = true)
{
	// As range may be used in ? context, we traverse to not to parse next token.
	if (needToParseNextToken)
	{
		lex.nextToken();
	}

	// range --> LSQR INT COLON INT RSQR
	if (lex.token().kind() != lcsm::verilog::ModuleDeclareKind::KW_LSQR)
	{
		throw std::logic_error("Parser error at trying to parse range!");
	}

	if (!lex.nextToken().isInteger())
	{
		throw std::logic_error("Parser error at trying to parse range!");
	}

	if (lex.nextToken().kind() != lcsm::verilog::ModuleDeclareKind::KW_COLON)
	{
		throw std::logic_error("Parser error at trying to parse range!");
	}

	if (!lex.nextToken().isInteger())
	{
		throw std::logic_error("Parser error at trying to parse range!");
	}

	if (lex.nextToken().kind() != lcsm::verilog::ModuleDeclareKind::KW_RSQR)
	{
		throw std::logic_error("Parser error at trying to parse range!");
	}
}

static void ruleListOfPortIdentifiers(lcsm::verilog::ModuleDeclareLexer &lex, bool needToParseNextToken = true)
{
	// As list_of_port_identifiers may be used in ? context, we traverse to not to parse next token.
	if (needToParseNextToken)
	{
		lex.nextToken();
	}

	// Parse list_of_port_identifiers.
	if (!lex.token().isIdentifier())
	{
		throw std::logic_error("Parser error at trying to parse inout_declaration!");
	}

	while (lex.nextToken().kind() == lcsm::verilog::ModuleDeclareKind::KW_COMMA)
	{
		if (!lex.nextToken().isIdentifier())
		{
			// throw std::logic_error("Parser error at trying to parse inout_declaration!");
			return;
		}
	}
}

static void ruleInoutDeclaration(lcsm::verilog::ModuleDeclareLexer &lex, bool needToParseNextToken = true)
{
	// As inout_declaration may be used in ? context, we traverse to not to parse next token.
	if (needToParseNextToken)
	{
		lex.nextToken();
	}

	// inout_declaration --> INOUT (net_type)? (SIGNED)? (range)? list_of_port_identifiers
	if (lex.token().kind() != lcsm::verilog::ModuleDeclareKind::KW_INOUT)
	{
		throw std::logic_error("Parser error at trying to parse inout_declaration!");
	}

	// Check keywords: (net_type)? (SIGNED)? <start of (range)?>
	lex.nextToken();
	// Check net_type --> SUPPLY0 | SUPPLY1 | TRI | TRIAND | TRIOR | TRI0 | TRI1 | UWIRE | WIRE | WAND | WOR
	switch (lex.token().kind())
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
		lex.nextToken();
		break;
	}
	default:
		break;
	}

	// Check SIGNED
	if (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_SIGNED)
	{
		lex.nextToken();
	}

	// Check range.
	if (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_RSQR)
	{
		ruleRange(lex, false);
		lex.nextToken();
	}

	// Parse list_of_port_identifiers.
	ruleListOfPortIdentifiers(lex, false);
}

static void ruleInputDeclaration(lcsm::verilog::ModuleDeclareLexer &lex, bool needToParseNextToken = true)
{
	// As inout_declaration may be used in ? context, we traverse to not to parse next token.
	if (needToParseNextToken)
	{
		lex.nextToken();
	}

	// inout_declaration --> INOUT (net_type)? (SIGNED)? (range)? list_of_port_identifiers
	if (lex.token().kind() != lcsm::verilog::ModuleDeclareKind::KW_INPUT)
	{
		throw std::logic_error("Parser error at trying to parse inout_declaration!");
	}

	// Check keywords: (net_type)? (SIGNED)? <start of (range)?>
	lex.nextToken();
	// Check net_type --> SUPPLY0 | SUPPLY1 | TRI | TRIAND | TRIOR | TRI0 | TRI1 | UWIRE | WIRE | WAND | WOR
	switch (lex.token().kind())
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
		lex.nextToken();
		break;
	}
	default:
		break;
	}

	// Check SIGNED
	if (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_SIGNED)
	{
		lex.nextToken();
	}

	// Check range.
	if (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_RSQR)
	{
		ruleRange(lex, false);
		lex.nextToken();
	}

	// Parse list_of_port_identifiers.
	ruleListOfPortIdentifiers(lex, false);
}

static void ruleOutputVariableType(lcsm::verilog::ModuleDeclareLexer &lex, bool needToParseNextToken = true)
{
	// As output_variable_type may be used in ? context, we traverse to not to parse next token.
	if (needToParseNextToken)
	{
		lex.nextToken();
	}

	if (lex.token().kind() != lcsm::verilog::ModuleDeclareKind::KW_INTEGER && lex.token().kind() != lcsm::verilog::ModuleDeclareKind::KW_TIME)
	{
		throw std::logic_error("Parser error at trying to parse output_variable_type!");
	}
}

static void ruleListOfVariablePortIdentifiers(lcsm::verilog::ModuleDeclareLexer &lex, bool needToParseNextToken = true)
{
	// As list_of_variable_port_identifiers  may be used in ? context, we traverse to not to parse next token.
	if (needToParseNextToken)
	{
		lex.nextToken();
	}

	// list_of_variable_port_identifiers --> port_identifier (EQUAL constant_expression)? (COMMA port_identifier (EQUAL
	// constant_expression)?)*
	if (!lex.token().isIdentifier())
	{
		throw std::logic_error("Parser error at trying to parse list_of_variable_port_identifiers!");
	}

	// (EQUAL constant_expression)?
	lex.nextToken();
	if (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_EQUAL)
	{
		lex.nextToken();
		if (!lex.token().isInteger())
		{
			throw std::logic_error("Parser error at trying to parse list_of_variable_port_identifiers!");
		}
	}

	// (COMMA port_identifier (EQUAL constant_expression)?)*
	while (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_COMMA)
	{
		// port_identifier
		lex.nextToken();
		if (!lex.token().isIdentifier())
		{
			throw std::logic_error("Parser error at trying to parse list_of_variable_port_identifiers!");
		}

		// (EQUAL constant_expression)?
		lex.nextToken();
		if (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_EQUAL)
		{
			lex.nextToken();
			if (!lex.token().isInteger())
			{
				throw std::logic_error("Parser error at trying to parse list_of_variable_port_identifiers!");
			}
		}
	}
}

static void ruleOutputDeclaration(lcsm::verilog::ModuleDeclareLexer &lex, bool needToParseNextToken = true)
{
	// As output_declaration may be used in ? context, we traverse to not to parse next token.
	if (needToParseNextToken)
	{
		lex.nextToken();
	}

	// output_declaration --> OUTPUT (net_type)? (SIGNED)? (range)? list_of_port_identifiers |
	//                        OUTPUT_REG (SIGNED)? (range)? list_of_port_identifiers         |
	//                        OUTPUT output_variable_type list_of_variable_port_identifiers
	switch (lex.token().kind())
	{
	case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT:
	{
		// Check keywords: (net_type)?
		lex.nextToken();
		switch (lex.token().kind())
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
			lex.nextToken();

			// Check SIGNED.
			if (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_SIGNED)
			{
				lex.nextToken();
			}

			// Check range.
			if (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_RSQR)
			{
				ruleRange(lex, false);
				lex.nextToken();
			}

			// Parse list_of_port_identifiers.
			ruleListOfPortIdentifiers(lex, false);

			return;
		}
		default:
			break;
		}

		// Check SIGNED.
		if (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_SIGNED)
		{
			lex.nextToken();
		}

		// Check range.
		if (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_LSQR)
		{
			ruleRange(lex, false);
			lex.nextToken();
		}

		// Choose between list_of_port_identifiers | output_variable_type.
		// Check output_variable_type.
		if (lex.token().kind() != lcsm::verilog::ModuleDeclareKind::KW_INTEGER &&
			lex.token().kind() != lcsm::verilog::ModuleDeclareKind::KW_TIME)
		{
			// Parse list_of_port_identifiers.
			ruleListOfPortIdentifiers(lex, false);

			return;
		}
		else
		{
			// Parse output_variable_type.
			ruleOutputVariableType(lex, false);

			// Parse list_of_variable_port_identifiers
			ruleListOfVariablePortIdentifiers(lex);

			return;
		}

		break;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT_REG:
	{
		break;
	}
	default:
		break;
	}

	throw std::logic_error("Parser error at trying to parse output_declaration!");
}

static void rulePortDeclaration(lcsm::verilog::ModuleDeclareLexer &lex, bool needToParseNextToken = true)
{
	// As port_declarations may be used in ? context, we traverse to not to parse next token.
	if (needToParseNextToken)
	{
		lex.nextToken();
	}

	// port_declaration   --> inout_declaration | input_declaration | output_declaration
	// inout_declaration  --> INOUT ...
	// input_declaration  --> INPUT ...
	// output_declaration --> OUTPUT ... | OUTPUT_REG ...
	switch (lex.token().kind())
	{
	case lcsm::verilog::ModuleDeclareKind::KW_INOUT:
	{
		ruleInoutDeclaration(lex, false);
		return;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_INPUT:
	{
		ruleInputDeclaration(lex, false);
		return;
	}
	case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT:
	case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT_REG:
	{
		ruleOutputDeclaration(lex, false);
		return;
	}
	default:
		throw std::logic_error("Parser error at trying to parse port_declarations!");
	}

	throw std::logic_error("Parser error at trying to parse port_declarations!");
}

static void ruleListOfPortDeclarations(lcsm::verilog::ModuleDeclareLexer &lex)
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

	// 2.2.1. (port_declaration (COMMA port_declaration)*)?
	rulePortDeclaration(lex, false);
	// while (lex.token().kind() == lcsm::verilog::ModuleDeclareKind::KW_COMMA)
	bool loop = true;
	while (loop)
	{
		switch (lex.token().kind())
		{
		case lcsm::verilog::ModuleDeclareKind::KW_INOUT:
		case lcsm::verilog::ModuleDeclareKind::KW_INPUT:
		case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT:
		case lcsm::verilog::ModuleDeclareKind::KW_OUTPUT_REG:
		{
			rulePortDeclaration(lex, false);
			break;
		}
		case lcsm::verilog::ModuleDeclareKind::KW_RPAREN:
		{
			loop = false;
			break;
		}
		default:
			throw std::logic_error("Parser error at trying to parse list_of_port_declarations!");
		}
	}

	// 2.2.2. RPAREN
	if (lex.token().kind() != lcsm::verilog::ModuleDeclareKind::KW_RPAREN)
	{
		throw std::logic_error("Parser error at trying to parse list_of_port_declarations!");
	}
}

static void ruleModuleIdentifier(lcsm::verilog::ModuleDeclareLexer &lex)
{
	const lcsm::verilog::ModuleDeclareToken &tok = lex.nextToken();
	if (!tok.isIdentifier())
	{
		throw std::logic_error("Parser error at trying to parse module_identifier!");
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

static void ruleModuleDeclaration(lcsm::verilog::ModuleDeclareLexer &lex)
{
	// module_declaration -> module_keyword module_identifier list_of_port_declarations SEMI

	// 1. module_keyword
	ruleModuleKeyword(lex);

	// 2. module_identifier
	ruleModuleIdentifier(lex);

	// 3. list_of_port_declarations
	ruleListOfPortDeclarations(lex);

	// 4. SEMI.
	const lcsm::verilog::ModuleDeclareToken &tok = lex.nextToken();
	if (tok.kind() != lcsm::verilog::ModuleDeclareKind::KW_SEMI)
	{
		throw std::logic_error("Parser error at trying to parse SEMI!");
	}
}

void lcsm::verilog::ModuleDeclareParser::parse()
{
	ruleModuleDeclaration(m_lex);
}
