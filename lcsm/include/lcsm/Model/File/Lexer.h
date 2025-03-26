#ifndef LCSM_MODEL_FILE_LEXER_H
#define LCSM_MODEL_FILE_LEXER_H

#include <lcsm/Model/File/Token.h>
#include <lcsm/Support/IO/Reader.h>
#include <lcsm/lcsmconfig.h>

#include <deque>
#include <memory>

namespace lcsm
{
	namespace model
	{
		class LCSM_API LCSMFileLexer
		{
		  public:
			LCSMFileLexer(const std::shared_ptr< support::Reader > &source);
			LCSMFileLexer(std::shared_ptr< support::Reader > &&source);

			LCSMFileLexer(const LCSMFileLexer &other) = delete;
			LCSMFileLexer(LCSMFileLexer &&other) noexcept;

			LCSMFileLexer &operator=(const LCSMFileLexer &other) = delete;
			LCSMFileLexer &operator=(LCSMFileLexer &&other) noexcept;

			void swap(LCSMFileLexer &other);

			const LCSMFileToken &nextToken();
			const LCSMFileToken &token() const noexcept;
			void backToken();

		  private:
			std::shared_ptr< support::Reader > m_source;
			LCSMFileToken m_token;
			char m_character;
			std::deque< LCSMFileToken > m_previousTokens;

		  private:
			void nextChar();
			void skipBlanks();
			void buildString(std::string &builder);
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODULE_FILE_LEXER_H */
