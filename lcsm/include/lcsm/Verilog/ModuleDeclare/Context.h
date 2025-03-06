#ifndef LCSM_VERILOG_MODULEDECLARE_CONTEXT_H
#define LCSM_VERILOG_MODULEDECLARE_CONTEXT_H

#include <lcsm/Verilog/Port.h>

#include <string>
#include <utility>
#include <vector>

namespace lcsm
{
	namespace verilog
	{
		class ModuleDeclareContext
		{
		  public:
			ModuleDeclareContext() = default;

			ModuleDeclareContext(const ModuleDeclareContext &other);
			ModuleDeclareContext(ModuleDeclareContext &&other) noexcept;

			ModuleDeclareContext &operator=(const ModuleDeclareContext &other);
			ModuleDeclareContext &operator=(ModuleDeclareContext &&other) noexcept;

			void swap(ModuleDeclareContext &other) noexcept;

			const std::string &moduleIdentifier() const noexcept;
			void setModuleIdentifier(const std::string &moduleIdentifier);
			void setModuleIdentifier(std::string &&moduleIdentifier) noexcept;

			void addPortDeclaration(PortType portType, const std::vector< std::string > &identifiers);
			void addPortDeclaration(PortType portType, std::vector< std::string > &&identifiers);

			const std::vector< std::pair< PortType, std::vector< std::string > > > &modulePortDeclarations() const noexcept;
			std::vector< std::pair< PortType, std::vector< std::string > > > &modulePortDeclarations() noexcept;

		  private:
			std::string m_moduleIdentifier;
			std::vector< std::pair< PortType, std::vector< std::string > > > m_modulePortDeclarations;
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_MODULEDECLARE_CONTEXT_H */
