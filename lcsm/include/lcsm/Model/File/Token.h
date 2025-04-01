#ifndef LCSM_MODEL_FILE_TOKEN_H
#define LCSM_MODEL_FILE_TOKEN_H

#include <lcsm/Model/File/Kind.h>
#include <lcsm/lcsmconfig.h>

#include <string>

namespace lcsm
{
	namespace model
	{
		class LCSM_API LCSMFileToken
		{
		  public:
			LCSMFileToken();
			LCSMFileToken(LCSMFileKind kind);
			LCSMFileToken(unsigned long long i);
			LCSMFileToken(const std::string &s, bool isIdentifier);
			LCSMFileToken(std::string &&s, bool isIdentifier) noexcept;

			LCSMFileToken(const LCSMFileToken &other);
			LCSMFileToken(LCSMFileToken &&other) noexcept;

			LCSMFileToken &operator=(const LCSMFileToken &other);
			LCSMFileToken &operator=(LCSMFileToken &&other) noexcept;

			bool operator==(LCSMFileKind kind) const noexcept;
			bool operator!=(LCSMFileKind kind) const noexcept;

			void swap(LCSMFileToken &other) noexcept;

			LCSMFileKind kind() noexcept;

			void setToken(LCSMFileKind kind) noexcept;
			void setToken(unsigned long long i) noexcept;
			void setToken(const std::string &s, bool isIdentifier);
			void setToken(std::string &&s, bool isIdentifier) noexcept;

			void setEof() noexcept;

			bool isKeyword() const noexcept;
			bool isInteger() const noexcept;
			bool isIdentifier() const noexcept;
			bool isString() const noexcept;
			bool isEof() const noexcept;

			unsigned long long asInteger() const noexcept;
			const std::string &asIdentifier() const noexcept;
			const std::string &asString() const noexcept;

		  private:
			LCSMFileKind m_kind;
			unsigned long long m_i;
			std::string m_s;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_FILE_TOKEN_H */
