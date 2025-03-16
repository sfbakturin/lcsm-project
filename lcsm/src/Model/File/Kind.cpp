#include <lcsm/Model/File/Kind.h>

#include <cstring>
#include <string>

const char *lcsm::model::KindToKeyword(lcsm::model::LCSMFileKind kind) noexcept
{
	switch (kind)
	{
	case lcsm::model::LCSMFileKind::BeginCircuitKeyword:
	{
		return "begincircuit";
	}
	case lcsm::model::LCSMFileKind::EndCircuitKeyword:
	{
		return "endcircuit";
	}
	case lcsm::model::LCSMFileKind::BeginConnectionsKeyword:
	{
		return "beginconnections";
	}
	case lcsm::model::LCSMFileKind::EndConnectionsKeyword:
	{
		return "endconnections";
	}
	case lcsm::model::LCSMFileKind::BeginComponentKeyword:
	{
		return "begincomponent";
	}
	case lcsm::model::LCSMFileKind::EndComponentKeyword:
	{
		return "endcomponent";
	}
	case lcsm::model::LCSMFileKind::BeginTunnelsKeyword:
	{
		return "begintunnels";
	}
	case lcsm::model::LCSMFileKind::EndTunnelsKeyword:
	{
		return "endtunnels";
	}
	case lcsm::model::LCSMFileKind::CircuitTypeKeyword:
	{
		return "circuittype";
	}
	case lcsm::model::LCSMFileKind::IdKeyword:
	{
		return "id";
	}
	case lcsm::model::LCSMFileKind::NameKeyword:
	{
		return "name";
	}
	case lcsm::model::LCSMFileKind::KeyValueKeyword:
	{
		return "keyvalue";
	}
	case lcsm::model::LCSMFileKind::ConnectKeyword:
	{
		return "connect";
	}
	case lcsm::model::LCSMFileKind::TunnelKeyword:
	{
		return "tunnel";
	}
	case lcsm::model::LCSMFileKind::TrueKeyword:
	{
		return "true";
	}
	case lcsm::model::LCSMFileKind::FalseKeyword:
	{
		return "false";
	}
	case lcsm::model::LCSMFileKind::SemicolonCharKeyword:
	{
		return ";";
	}
	case lcsm::model::LCSMFileKind::QuotationMarkKeyword:
	{
		return "\"";
	}
	default:
	{
		break;
	}
	}
	return nullptr;
}

lcsm::model::LCSMFileKind lcsm::model::IsKeyword(const char *s) noexcept
{
	for (std::int8_t it = lcsm::model::KindKeywordStart; it <= lcsm::model::KindKeywordEnd; it++)
	{
		const lcsm::model::LCSMFileKind kind = static_cast< lcsm::model::LCSMFileKind >(it);
		const char *keyword = KindToKeyword(kind);
		if (std::strcmp(keyword, s) == 0)
		{
			return kind;
		}
	}
	return lcsm::model::LCSMFileKind::UnknownKind;
}

lcsm::model::LCSMFileKind lcsm::model::IsKeyword(const std::string &s) noexcept
{
	return IsKeyword(s.c_str());
}

lcsm::model::LCSMFileKind lcsm::model::IsKeyword(char c) noexcept
{
	switch (c)
	{
	case ';':
	{
		return lcsm::model::LCSMFileKind::SemicolonCharKeyword;
	}
	case '"':
	{
		return lcsm::model::LCSMFileKind::QuotationMarkKeyword;
	}
	default:
	{
		break;
	}
	}
	return lcsm::model::LCSMFileKind::UnknownKind;
}

lcsm::model::LCSMFileKind lcsm::model::BoolToKeyword(bool b) noexcept
{
	return b ? lcsm::model::LCSMFileKind::TrueKeyword : lcsm::model::LCSMFileKind::FalseKeyword;
}
