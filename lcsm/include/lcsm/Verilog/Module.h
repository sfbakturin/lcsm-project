#ifndef LCSM_VERILOG_MODULE_H
#define LCSM_VERILOG_MODULE_H

#include <lcsm/Physical/DataBits.h>
#include <lcsm/Support/Parser/CharSource.h>
#include <lcsm/Verilog/Port.h>
#include <unordered_map>

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace lcsm
{
	namespace verilog
	{
		class Module
		{
		  public:
			Module(const Module &other);
			Module(Module &&other) noexcept;

			Module &operator=(const Module &other);
			Module &operator=(Module &&other) noexcept;

			void swap(Module &other) noexcept;

			static Module fromFile(std::FILE *stream);
			static Module fromFile(const char *filename);
			static Module fromFile(std::ifstream &stream);

			static Module fromString(const char *string);
			static Module fromString(const std::string &string);
			static Module fromString(std::string &&string);

			const std::vector< Port > &inputPorts() const noexcept;
			const std::vector< Port > &inoutPorts() const noexcept;
			const std::vector< Port > &outputPorts() const noexcept;
			const std::vector< Port > &outputRegPorts() const noexcept;

			std::unordered_map< IOType, std::vector< DataBits > >
				invoke(const std::unordered_map< IOType, std::vector< DataBits > > &testBenchData);

		  private:
			std::string m_identifier;
			std::string m_sourceModule;

			std::vector< Port > m_inputPorts;
			std::vector< Port > m_inoutPorts;
			std::vector< Port > m_outputPorts;
			std::vector< Port > m_outputRegPorts;

		  private:
			Module() noexcept = default;

			static Module parse(const std::shared_ptr< support::CharSource > &source);
		};
	}	 // namespace verilog
}	 // namespace lcsm

#endif /* LCSM_VERILOG_MODULE_H */
