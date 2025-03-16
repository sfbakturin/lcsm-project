#include <lcsm/Model/File/Kind.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/IO/Writer.h>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

lcsm::model::LCSMFileWriter::LCSMFileWriter(const std::shared_ptr< lcsm::support::Writer > &writer) noexcept :
	m_writer(writer), m_indent(0)
{
}

lcsm::model::LCSMFileWriter::LCSMFileWriter(std::shared_ptr< lcsm::support::Writer > &&writer) noexcept :
	m_writer(std::move(writer)), m_indent(0)
{
}

void lcsm::model::LCSMFileWriter::writeBeginCircuit()
{
	writeIndents();
	m_writer->writeln(lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::BeginCircuitKeyword));
	m_indent += 4;
}

void lcsm::model::LCSMFileWriter::writeEndCircuit()
{
	m_indent -= 4;
	writeIndents();
	m_writer->writeln(lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::EndCircuitKeyword));
}

void lcsm::model::LCSMFileWriter::writeBeginComponent()
{
	writeIndents();
	m_writer->writeln(lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::BeginComponentKeyword));
	m_indent += 4;
}

void lcsm::model::LCSMFileWriter::writeEndComponent()
{
	m_indent -= 4;
	writeIndents();
	m_writer->writeln(lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::EndComponentKeyword));
}

void lcsm::model::LCSMFileWriter::writeBeginConnections()
{
	writeIndents();
	m_writer->writeln(lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::BeginConnectionsKeyword));
	m_indent += 4;
}

void lcsm::model::LCSMFileWriter::writeEndConnections()
{
	m_indent -= 4;
	writeIndents();
	m_writer->writeln(lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::EndConnectionsKeyword));
}

void lcsm::model::LCSMFileWriter::writeBeginTunnels()
{
	writeIndents();
	m_writer->writeln(lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::BeginTunnelsKeyword));
	m_indent += 4;
}

void lcsm::model::LCSMFileWriter::writeEndTunnels()
{
	m_indent -= 4;
	writeIndents();
	m_writer->writeln(lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::EndTunnelsKeyword));
}

void lcsm::model::LCSMFileWriter::writeCircuitTypeDeclaration(int i)
{
	const std::string s = std::to_string(i);
	writeIndents();
	m_writer->write(lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::CircuitTypeKeyword));
	m_writer->write(' ');
	m_writer->write(s);
	m_writer->writeln(';');
}

void lcsm::model::LCSMFileWriter::writeIdDeclaration(lcsm::Identifier id)
{
	const std::string s = id.toString();
	writeIndents();
	// clang-format off
	m_writer->writeln({
		lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::IdKeyword),
		" ",
		s.c_str(),
		";"
	});
	// clang-format on
}

void lcsm::model::LCSMFileWriter::writeNameDeclaration(const std::string &name)
{
	writeIndents();
	// clang-format off
	m_writer->write({
		lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::NameKeyword),
		" "
	});
	// clang-format on
	m_writer->writeEscaped(name);
	m_writer->writeln(';');
}

void lcsm::model::LCSMFileWriter::writeKeyValueDeclaration(const char *key, std::int64_t i)
{
	const std::string s = std::to_string(i);
	writeKeyValueDeclaration(key, s);
}

void lcsm::model::LCSMFileWriter::writeKeyValueDeclaration(const char *key, std::uint64_t i)
{
	const std::string s = std::to_string(i);
	writeKeyValueDeclaration(key, s);
}

void lcsm::model::LCSMFileWriter::writeKeyValueDeclaration(const char *key, const std::string &s)
{
	writeIndents();
	// clang-format off
	m_writer->writeln({
		lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::KeyValueKeyword),
		" ",
		key,
		" ",
		s.c_str(),
		";"
	});
	// clang-format on
}

void lcsm::model::LCSMFileWriter::writeKeyValueDeclaration(const char *key, lcsm::Identifier id)
{
	const std::string s = id.toString();
	writeKeyValueDeclaration(key, s);
}

void lcsm::model::LCSMFileWriter::writeKeyValueEscapedDeclaration(const char *key, const std::string &s)
{
	writeIndents();
	// clang-format off
	m_writer->write({
		lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::KeyValueKeyword),
		" ",
		key,
		" "
	});
	// clang-format on
	m_writer->writeEscaped(s);
	m_writer->writeln(';');
}

void lcsm::model::LCSMFileWriter::writeKeyValueDeclaration(const char *key, bool b)
{
	writeIndents();
	const lcsm::model::LCSMFileKind kind = (b ? lcsm::model::LCSMFileKind::TrueKeyword : lcsm::model::LCSMFileKind::FalseKeyword);
	// clang-format off
	m_writer->writeln({
		lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::KeyValueKeyword),
		" ",
		key,
		" ",
		lcsm::model::KindToKeyword(kind),
		";"
	});
	// clang-format on
}

void lcsm::model::LCSMFileWriter::writeConnectDeclaration(lcsm::Identifier lhs, lcsm::Identifier rhs, bool isFirstComp, bool isSecondComp)
{
	const std::string l = lhs.toString();
	const std::string r = rhs.toString();
	const lcsm::model::LCSMFileKind kind1 = lcsm::model::BoolToKeyword(isFirstComp);
	const lcsm::model::LCSMFileKind kind2 = lcsm::model::BoolToKeyword(isSecondComp);
	writeIndents();
	// clang-format off
	m_writer->writeln({
		lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::ConnectKeyword),
		" ",
		l.c_str(),
		" ",
		r.c_str(),
		" ",
		lcsm::model::KindToKeyword(kind1),
		" ",
		lcsm::model::KindToKeyword(kind2),
		";"
	});
	// clang-format on
}

void lcsm::model::LCSMFileWriter::writeTunnelDeclaration(lcsm::Identifier lhs, lcsm::Identifier rhs)
{
	const std::string l = lhs.toString();
	const std::string r = rhs.toString();
	writeIndents();
	// clang-format off
	m_writer->writeln({
		lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind::ConnectKeyword),
		" ",
		l.c_str(),
		" ",
		r.c_str(),
		";"
	});
	// clang-format on
}

void lcsm::model::LCSMFileWriter::writeIndents()
{
	m_writer->write(' ', m_indent);
}
