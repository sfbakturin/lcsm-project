#ifndef LCSM_MODEL_FILE_READER_H
#define LCSM_MODEL_FILE_READER_H

#include "lcsm/LCSM.h"
#include <lcsm/Model/File/Lexer.h>
#include <lcsm/Model/File/Token.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/IO/Reader.h>

#include <memory>
#include <string>
#include <utility>

namespace lcsm
{
	namespace model
	{
		class LCSMFileReader
		{
		  public:
			LCSMFileReader(const std::shared_ptr< support::Reader > &reader);
			LCSMFileReader(std::shared_ptr< support::Reader > &&reader);

			const LCSMFileToken &next();
			const LCSMFileToken &curr() const noexcept;

			void parseBeginCircuit();
			void parseEndCircuit();

			bool tryParseBeginComponent();
			void parseEndComponent();

			bool tryParseBeginConnections();
			bool tryParseEndConnections();

			bool tryParseBeginTunnels();
			bool tryParseEndTunnels();

			target_t exceptCircuitType();
			Identifier exceptIdentifier();
			std::string exceptName();
			std::pair< std::string, unsigned long long > exceptIntegerKeyValue();
			unsigned long long exceptIntegerKeyValue(const char *key);
			std::pair< std::string, bool > exceptBooleanKeyValue();
			bool exceptBooleanKeyValue(const char *key);
			std::pair< std::string, std::string > exceptKeyValue();
			std::string exceptKeyValue(const char *key);
			std::pair< std::string, std::string > exceptEscapedKeyValue();
			std::string exceptEscapedKeyValue(const char *key);

		  private:
			LCSMFileLexer m_lexer;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_FILE_READER_H */
