#ifndef LCSM_MODEL_FILE_WRITER_H
#define LCSM_MODEL_FILE_WRITER_H

#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/IO/Writer.h>
#include <lcsm/lcsmconfig.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace lcsm
{
	namespace model
	{
		class LCSM_API LCSMFileWriter
		{
		  public:
			LCSMFileWriter(const std::shared_ptr< support::Writer > &writer) noexcept;
			LCSMFileWriter(std::shared_ptr< support::Writer > &&writer) noexcept;

			void writeBeginCircuit();
			void writeEndCircuit();

			void writeBeginComponent();
			void writeEndComponent();

			void writeBeginConnections();
			void writeEndConnections();

			void writeBeginTunnels();
			void writeEndTunnels();

			void writeCircuitTypeDeclaration(int i);
			void writeIdDeclaration(Identifier id);
			void writeNameDeclaration(const std::string &name);
			void writeKeyValueDeclaration(const char *key, std::int64_t i);
			void writeKeyValueDeclaration(const char *key, std::uint64_t i);
			void writeKeyValueDeclaration(const char *key, const std::string &s);
			void writeKeyValueDeclaration(const char *key, Identifier id);
			void writeKeyValueEscapedDeclaration(const char *key, const std::string &s);
			void writeKeyValueDeclaration(const char *key, bool b);

			void writeConnectDeclaration(Identifier lhs, Identifier rhs, bool isFirstComp, bool isSecondComp);
			void writeTunnelDeclaration(Identifier lhs, Identifier rhs);

		  private:
			std::shared_ptr< support::Writer > m_writer;
			std::size_t m_indent;

		  private:
			void writeIndents();
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_FILE_WRITER_H */
