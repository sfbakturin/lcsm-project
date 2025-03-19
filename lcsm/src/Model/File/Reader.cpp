#include <lcsm/LCSM.h>
#include <lcsm/Model/File/Kind.h>
#include <lcsm/Model/File/Lexer.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Token.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/IO/Reader.h>
#include <lcsm/Support/Strings.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

lcsm::model::LCSMFileReader::LCSMFileReader(const std::shared_ptr< lcsm::support::Reader > &reader) : m_lexer(reader) {}

lcsm::model::LCSMFileReader::LCSMFileReader(std::shared_ptr< lcsm::support::Reader > &&reader) :
	m_lexer(std::move(reader))
{
}

const lcsm::model::LCSMFileToken &lcsm::model::LCSMFileReader::next()
{
	return m_lexer.nextToken();
}

const lcsm::model::LCSMFileToken &lcsm::model::LCSMFileReader::curr() const noexcept
{
	return m_lexer.token();
}

void lcsm::model::LCSMFileReader::parseBeginCircuit()
{
	if (next() != lcsm::model::LCSMFileKind::BeginCircuitKeyword)
	{
		throw std::logic_error("Excepted 'begincircuit', but got something else.");
	}
}

void lcsm::model::LCSMFileReader::parseEndCircuit()
{
	if (next() != lcsm::model::LCSMFileKind::EndCircuitKeyword)
	{
		throw std::logic_error("Excepted 'endcircuit', but got something else.");
	}
}

bool lcsm::model::LCSMFileReader::tryParseBeginComponent()
{
	if (next() != lcsm::model::LCSMFileKind::BeginComponentKeyword)
	{
		m_lexer.backToken();
		return false;
	}
	else
	{
		return true;
	}
}

void lcsm::model::LCSMFileReader::parseEndComponent()
{
	if (next() != lcsm::model::LCSMFileKind::EndComponentKeyword)
	{
		throw std::logic_error("Excepted 'endcomponent', but got something else.");
	}
}

bool lcsm::model::LCSMFileReader::tryParseBeginConnections()
{
	if (next() != lcsm::model::LCSMFileKind::BeginConnectionsKeyword)
	{
		m_lexer.backToken();
		return false;
	}
	else
	{
		return true;
	}
}

bool lcsm::model::LCSMFileReader::tryParseEndConnections()
{
	if (next() != lcsm::model::LCSMFileKind::EndConnectionsKeyword)
	{
		m_lexer.backToken();
		return false;
	}
	else
	{
		return true;
	}
}

bool lcsm::model::LCSMFileReader::tryParseBeginTunnels()
{
	if (next() != lcsm::model::LCSMFileKind::BeginTunnelsKeyword)
	{
		m_lexer.backToken();
		return false;
	}
	else
	{
		return true;
	}
}

bool lcsm::model::LCSMFileReader::tryParseEndTunnels()
{
	if (next() != lcsm::model::LCSMFileKind::EndTunnelsKeyword)
	{
		m_lexer.backToken();
		return false;
	}
	else
	{
		return true;
	}
}

lcsm::target_t lcsm::model::LCSMFileReader::exceptCircuitType()
{
	// circuit_type --> 'circuittype' INTEGER ';'
	if (next() != lcsm::model::LCSMFileKind::CircuitTypeKeyword)
	{
		throw std::logic_error("Excepted 'circuittype', but got something else.");
	}
	if (next() != lcsm::model::LCSMFileKind::IntegerToken)
	{
		throw std::logic_error("Excepted integer valued token, but got something else.");
	}
	const lcsm::target_t circuitType = static_cast< lcsm::target_t >(curr().asInteger());
	if (next() != lcsm::model::LCSMFileKind::SemicolonCharKeyword)
	{
		throw std::logic_error("Excepted ';', but got something else.");
	}
	return circuitType;
}

lcsm::Identifier lcsm::model::LCSMFileReader::exceptIdentifier()
{
	// id --> 'id' INTEGER ';'
	if (next() != lcsm::model::LCSMFileKind::IdKeyword)
	{
		throw std::logic_error("Expected 'id', but got something else.");
	}
	if (next() != lcsm::model::LCSMFileKind::IntegerToken)
	{
		throw std::logic_error("Excepted integer valued token, but got something else.");
	}
	const lcsm::Identifier::value_type startId = curr().asInteger();
	if (next() != lcsm::model::LCSMFileKind::SemicolonCharKeyword)
	{
		throw std::logic_error("Excepted ';', but got something else.");
	}
	return lcsm::Identifier{ startId };
}

std::string lcsm::model::LCSMFileReader::exceptName()
{
	// name --> 'name' STRING ';'
	if (next() != lcsm::model::LCSMFileKind::NameKeyword)
	{
		throw std::logic_error("Expected 'name', but got something else.");
	}
	if (next() != lcsm::model::LCSMFileKind::StringToken)
	{
		throw std::logic_error("Excepted string valued token, but got something else.");
	}
	const std::string name = lcsm::support::stringsUnescape(curr().asString());
	if (next() != lcsm::model::LCSMFileKind::SemicolonCharKeyword)
	{
		throw std::logic_error("Excepted ';', but got something else.");
	}
	return name;
}

std::pair< std::string, unsigned long long > lcsm::model::LCSMFileReader::exceptIntegerKeyValue()
{
	// key_value --> 'keyvalue' IDENTIFIER INTEGER ';'
	if (next() != lcsm::model::LCSMFileKind::KeyValueKeyword)
	{
		throw std::logic_error("Expected 'keyvalue', but got something else.");
	}
	std::pair< std::string, unsigned long long > integerKeyValue;
	if (next() != lcsm::model::LCSMFileKind::IdentifierToken)
	{
		throw std::logic_error("Excepted identifier valued token, but got something else.");
	}
	integerKeyValue.first = curr().asIdentifier();
	if (next() != lcsm::model::LCSMFileKind::IntegerToken)
	{
		throw std::logic_error("Excepted integer valued token, but got something else.");
	}
	integerKeyValue.second = curr().asInteger();
	if (next() != lcsm::model::LCSMFileKind::SemicolonCharKeyword)
	{
		throw std::logic_error("Excepted ';', but got something else.");
	}
	return integerKeyValue;
}

unsigned long long lcsm::model::LCSMFileReader::exceptIntegerKeyValue(const char *key)
{
	const std::pair< std::string, unsigned long long > parsed = exceptIntegerKeyValue();
	if (parsed.first != key)
	{
		std::string message = "Parsed integer valued key-value, excepted '";
		message.append(key);
		message += "' as key, but actual is '" + parsed.first + '\'';
		throw std::logic_error(message);
	}
	return parsed.second;
}

std::pair< std::string, bool > lcsm::model::LCSMFileReader::exceptBooleanKeyValue()
{
	// key_value --> 'keyvalue' IDENTIFIER BOOLEAN ';'
	if (next() != lcsm::model::LCSMFileKind::KeyValueKeyword)
	{
		throw std::logic_error("Expected 'keyvalue', but got something else.");
	}
	std::pair< std::string, bool > booleanKeyValue;
	if (next() != lcsm::model::LCSMFileKind::IdentifierToken)
	{
		throw std::logic_error("Excepted identifier valued token, but got something else.");
	}
	booleanKeyValue.first = curr().asIdentifier();
	next();
	if (curr() != lcsm::model::LCSMFileKind::TrueKeyword && curr() != lcsm::model::LCSMFileKind::FalseKeyword)
	{
		throw std::logic_error("Excepted 'true' or 'false', but got something else.");
	}
	booleanKeyValue.second = curr() == lcsm::model::LCSMFileKind::TrueKeyword;
	if (next() != lcsm::model::LCSMFileKind::SemicolonCharKeyword)
	{
		throw std::logic_error("Excepted ';', but got something else.");
	}
	return booleanKeyValue;
}

bool lcsm::model::LCSMFileReader::exceptBooleanKeyValue(const char *key)
{
	const std::pair< std::string, bool > parsed = exceptBooleanKeyValue();
	if (parsed.first != key)
	{
		std::string message = "Parsed boolean valued key-value, excepted '";
		message.append(key);
		message += "' as key, but actual is '" + parsed.first + '\'';
		throw std::logic_error(message);
	}
	return parsed.second;
}

std::pair< std::string, std::string > lcsm::model::LCSMFileReader::exceptKeyValue()
{
	// key_value --> 'keyvalue' IDENTIFIER IDENTIFIER ';'
	if (next() != lcsm::model::LCSMFileKind::KeyValueKeyword)
	{
		throw std::logic_error("Expected 'keyvalue', but got something else.");
	}
	std::pair< std::string, std::string > keyValue;
	if (next() != lcsm::model::LCSMFileKind::IdentifierToken)
	{
		throw std::logic_error("Excepted identifier valued token, but got something else.");
	}
	keyValue.first = curr().asIdentifier();
	if (next() != lcsm::model::LCSMFileKind::IdentifierToken)
	{
		throw std::logic_error("Excepted identifier valued token, but got something else.");
	}
	keyValue.second = curr().asIdentifier();
	if (next() != lcsm::model::LCSMFileKind::SemicolonCharKeyword)
	{
		throw std::logic_error("Excepted ';', but got something else.");
	}
	return keyValue;
}

std::string lcsm::model::LCSMFileReader::exceptKeyValue(const char *key)
{
	const std::pair< std::string, std::string > parsed = exceptKeyValue();
	if (parsed.first != key)
	{
		std::string message = "Parsed key-value, excepted '";
		message.append(key);
		message += "' as key, but actual is '" + parsed.first + '\'';
		throw std::logic_error(message);
	}
	return parsed.second;
}

std::pair< std::string, std::string > lcsm::model::LCSMFileReader::exceptEscapedKeyValue()
{
	// key_value --> 'keyvalue' IDENTIFIER STRING ';'
	if (next() != lcsm::model::LCSMFileKind::KeyValueKeyword)
	{
		throw std::logic_error("Expected 'keyvalue', but got something else.");
	}
	std::pair< std::string, std::string > keyValue;
	if (next() != lcsm::model::LCSMFileKind::IdentifierToken)
	{
		throw std::logic_error("Excepted identifier valued token, but got something else.");
	}
	keyValue.first = curr().asIdentifier();
	if (next() != lcsm::model::LCSMFileKind::StringToken)
	{
		throw std::logic_error("Excepted identifier valued token, but got something else.");
	}
	keyValue.second = lcsm::support::stringsUnescape(curr().asString());
	if (next() != lcsm::model::LCSMFileKind::SemicolonCharKeyword)
	{
		throw std::logic_error("Excepted ';', but got something else.");
	}
	return keyValue;
}

std::string lcsm::model::LCSMFileReader::exceptEscapedKeyValue(const char *key)
{
	const std::pair< std::string, std::string > parsed = exceptEscapedKeyValue();
	if (parsed.first != key)
	{
		std::string message = "Parsed stringify key-value, excepted '";
		message.append(key);
		message += "' as key, but actual is '" + parsed.first + '\'';
		throw std::logic_error(message);
	}
	return parsed.second;
}
