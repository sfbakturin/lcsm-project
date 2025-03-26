#ifndef LCSM_MODEL_BUILDER_H
#define LCSM_MODEL_BUILDER_H

#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/lcsmconfig.h>
#include <unordered_map>

#include <deque>
#include <tuple>
#include <utility>

namespace lcsm
{
	class LCSM_API LCSMCircuit;

	namespace model
	{
		class LCSM_API Wire;
		class LCSM_API Tunnel;

		class LCSM_API LCSMBuilder
		{
		  public:
			LCSMBuilder() = default;

			void addWires(const Wire *wire, bool isFirstComp);

			void addWire(const std::tuple< Identifier, Identifier, bool, bool > &connect);
			void addWire(std::tuple< Identifier, Identifier, bool, bool > &&connect);
			void addWire(Identifier id1, Identifier id2, bool isFirstComp, bool isSecondComp);
			void addWire(unsigned long long id1, unsigned long long id2, bool isFirstComp, bool isSecondComp);

			void addTunnels(const Tunnel *tunnel);

			void addTunnel(const std::pair< Identifier, Identifier > &tunnel);
			void addTunnel(std::pair< Identifier, Identifier > &&tunnel);
			void addTunnel(Identifier id1, Identifier id2);

			void oldToNew(Identifier oldId, Identifier newId);

			void dump(LCSMFileWriter &writer);
			void finalize(LCSMCircuit *circuit);
			void check();

		  private:
			std::deque< std::tuple< Identifier, Identifier, bool, bool > > m_wires;
			std::deque< std::pair< Identifier, Identifier > > m_tunnels;
			std::unordered_map< Identifier, Identifier > m_oldToNewId;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_BUILDER_H */
