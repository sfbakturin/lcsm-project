#ifndef LCSM_MODEL_BUILDER_H
#define LCSM_MODEL_BUILDER_H

#include <lcsm/Model/File/Writer.h>

#include <vector>

namespace lcsm
{
	namespace model
	{
		class Wire;
		class Tunnel;

		class LCSMBuilder
		{
		  public:
			LCSMBuilder() = default;

			void addWires(const Wire *wire, bool isFirstComp);
			void addTunnels(const Tunnel *tunnel);

			void dumpToLCSMFile(LCSMFileWriter &writer);

		  private:
			std::vector< std::tuple< lcsm::Identifier, lcsm::Identifier, bool, bool > > m_wires;
			std::vector< std::pair< lcsm::Identifier, lcsm::Identifier > > m_tunnels;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_BUILDER_H */
