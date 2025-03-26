#ifndef LCSM_MODEL_FILE_KIND_H
#define LCSM_MODEL_FILE_KIND_H

#include <lcsm/lcsmconfig.h>

#include <cstdint>
#include <string>

namespace lcsm
{
	namespace model
	{
		enum LCSMFileKind : std::int8_t
		{
			UnknownKind = -1,
			BeginCircuitKeyword,
			EndCircuitKeyword,
			BeginConnectionsKeyword,
			EndConnectionsKeyword,
			BeginComponentKeyword,
			EndComponentKeyword,
			BeginTunnelsKeyword,
			EndTunnelsKeyword,
			CircuitTypeKeyword,
			IdKeyword,
			NameKeyword,
			KeyValueKeyword,
			ConnectKeyword,
			TunnelKeyword,
			TrueKeyword,
			FalseKeyword,
			SemicolonCharKeyword,
			QuotationMarkKeyword,
			IntegerToken,
			IdentifierToken,
			StringToken,
			EndOfFileToken
		};

		static constexpr std::int8_t KindKeywordStart = LCSMFileKind::BeginCircuitKeyword;
		static constexpr std::int8_t KindKeywordEnd = LCSMFileKind::QuotationMarkKeyword;

		LCSM_API const char *KindToKeyword(LCSMFileKind kind) noexcept;
		LCSM_API LCSMFileKind IsKeyword(const char *s) noexcept;
		LCSM_API LCSMFileKind IsKeyword(const std::string &s) noexcept;
		LCSM_API LCSMFileKind IsKeyword(char c) noexcept;
		LCSM_API LCSMFileKind BoolToKeyword(bool b) noexcept;
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_FILE_KIND_H */
