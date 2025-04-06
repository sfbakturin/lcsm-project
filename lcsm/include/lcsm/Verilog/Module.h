#ifndef LCSM_VERILOG_MODULE_H
#define LCSM_VERILOG_MODULE_H

#include <lcsm/Physical/DataBits.h>
#include <lcsm/Support/IO/Reader.h>
#include <lcsm/Verilog/Port.h>
#include <lcsm/lcsmconfig.h>
#include <unordered_map>

#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace lcsm
{
	namespace verilog
	{
		class LCSM_API Module
		{
		  public:
			Module() noexcept = default;
			Module(const Module &other);
			Module(Module &&other) noexcept;

			Module &operator=(const Module &other);
			Module &operator=(Module &&other) noexcept;

			void swap(Module &other) noexcept;

			const std::string &source() const noexcept;

			static Module fromFile(const char *filename);
			static Module fromFile(const std::string &filename);

			static Module fromString(const char *string);
			static Module fromString(const std::string &string);
			static Module fromString(std::string &&string);

			const std::vector< Port > &inputPorts() const noexcept;
			const std::vector< Port > &inoutPorts() const noexcept;
			const std::vector< Port > &outputPorts() const noexcept;

			std::unordered_map< PortDirectionType, std::vector< DataBits > >
				invoke(const std::unordered_map< PortDirectionType, std::vector< DataBits > > &testBenchData) const;

			const std::string &identifier() const noexcept;

		  private:
			std::string m_identifier;
			std::string m_sourceModule;

			std::vector< Port > m_inputPorts;
			std::vector< Port > m_inoutPorts;
			std::vector< Port > m_outputPorts;

			std::vector< std::pair< PortDirectionType, std::size_t > > m_ports;

		  private:
			static Module parse(const std::shared_ptr< support::Reader > &source);
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_MODULE_H */
