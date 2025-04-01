#ifndef LCSM_MODEL_FILE_READER_H
#define LCSM_MODEL_FILE_READER_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/File/Lexer.h>
#include <lcsm/Model/File/Token.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/IO/Reader.h>
#include <lcsm/lcsmconfig.h>

#include <memory>
#include <string>
#include <tuple>
#include <utility>

namespace lcsm
{
	namespace model
	{
		class LCSM_API LCSMFileReader
		{
		  public:
			LCSMFileReader(const std::shared_ptr< support::Reader > &reader);

			const LCSMFileToken &next();
			const LCSMFileToken &curr() const noexcept;
			void back();

			bool tryParseBeginCircuit();
			void parseBeginCircuit();
			void parseEndCircuit();

			bool tryParseBeginComponent();
			void parseEndComponent();

			void parseBeginConnections();
			void parseEndConnections();

			bool tryParseConnect();

			void parseBeginTunnels();
			void parseEndTunnels();

			bool tryParseTunnel();

			target_t exceptComponentType();
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
			std::tuple< Identifier, Identifier, bool, bool > exceptConnect();
			std::pair< Identifier, Identifier > exceptTunnel();

		  private:
			LCSMFileLexer m_lexer;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_FILE_READER_H */
