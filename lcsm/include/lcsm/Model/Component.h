#ifndef LCSM_MODEL_COMPONENT_H
#define LCSM_MODEL_COMPONENT_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Builder.h>
#include <lcsm/Model/File/Reader.h>
#include <lcsm/Model/File/Writer.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/lcsmconfig.h>

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace lcsm
{
	using portid_t = std::ptrdiff_t;

	namespace model
	{
		class LCSM_API Wire;
	}

	class LCSM_API Component
	{
	  public:
		Component() = default;
		Component(label_t name);
		virtual ~Component() noexcept = default;

		virtual std::size_t numOfWires() const noexcept = 0;
		virtual void provideWires(const std::vector< std::shared_ptr< model::Wire > > &wires) = 0;

		virtual Identifier id() const noexcept = 0;
		virtual Identifier identify(Identifier id) noexcept = 0;

		virtual object_type_t objectType() const noexcept = 0;
		virtual ComponentType componentType() const noexcept = 0;

		virtual void connect(portid_t portId, Component *circuit) = 0;

		virtual void disconnect(Component *circuit) noexcept = 0;
		virtual void disconnectAll() noexcept = 0;

		virtual Component *byPort(portid_t portId) noexcept = 0;
		virtual portid_t findPort(const Component *circuit) const noexcept = 0;

		virtual portid_t defaultPort() const noexcept = 0;

		virtual void dump(model::LCSMFileWriter &writer, model::LCSMBuilder &builder) const = 0;
		virtual void copy(Component *circuit, model::LCSMBuilder &builder) const = 0;
		virtual void from(model::LCSMFileReader &reader, model::LCSMBuilder &builder) = 0;

		void setName(const std::string &name);
		void setName(std::string &&name) noexcept;
		void setName(label_t name);
		const std::string &name() const noexcept;

	  protected:
		std::string m_name;
	};
}	 // namespace lcsm

#endif /* LCSM_MODEL_COMPONENT_H */
