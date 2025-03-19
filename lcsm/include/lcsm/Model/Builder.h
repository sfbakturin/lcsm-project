#ifndef LCSM_MODEL_BUILDER_H
#define LCSM_MODEL_BUILDER_H

#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <unordered_map>

#include <deque>

namespace lcsm
{
	class LCSMCircuit;

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
			void oldToNew(Identifier oldId, Identifier newId);

			void dump(LCSMFileWriter &writer);
			void finalize(LCSMCircuit *circuit);
			void check();

		  private:
			std::deque< std::tuple< lcsm::Identifier, lcsm::Identifier, bool, bool > > m_wires;
			std::deque< std::pair< lcsm::Identifier, lcsm::Identifier > > m_tunnels;
			std::unordered_map< Identifier, Identifier > m_oldToNewId;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_BUILDER_H */
