#ifndef LCSM_MODEL_CIRCUIT_H
#define LCSM_MODEL_CIRCUIT_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/PointerView.hpp>

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace lcsm
{
	using portid_t = std::ptrdiff_t;

	namespace model
	{
		class Wire;
	}

	class Circuit
	{
	  public:
		Circuit() = default;
		Circuit(label_t name);
		virtual ~Circuit() noexcept = default;

		virtual std::size_t numOfWires() const noexcept = 0;
		virtual void provideWires(const std::vector< std::shared_ptr< model::Wire > > &wires) = 0;

		virtual Identifier id() const noexcept = 0;
		virtual Identifier identify(Identifier id) noexcept = 0;

		virtual object_type_t objectType() const noexcept = 0;
		virtual CircuitType circuitType() const noexcept = 0;

		virtual void connect(portid_t portId, Circuit *circuit) = 0;

		virtual void disconnect(Circuit *circuit) noexcept = 0;
		virtual void disconnectAll() noexcept = 0;

		virtual Circuit *byPort(portid_t portId) noexcept = 0;
		virtual portid_t findPort(const Circuit *circuit) const noexcept = 0;

		virtual portid_t defaultPort() const noexcept = 0;

		virtual void dumpToLCSMFile(model::LCSMFileWriter &writer, model::LCSMBuilder &builder) const = 0;

		void setName(const std::string &name);
		void setName(std::string &&name) noexcept;
		void setName(label_t name);
		const std::string &name() const noexcept;
		label_t c_name() const noexcept;

	  protected:
		std::string m_name;
	};
}	 // namespace lcsm

#endif /* LCSM_MODEL_CIRCUIT_H */
