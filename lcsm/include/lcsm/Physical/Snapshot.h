#ifndef LCSM_PHYSICAL_SNAPSHOT_H
#define LCSM_PHYSICAL_SNAPSHOT_H

#include <lcsm/Model/Identifier.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/lcsmconfig.h>
#include <unordered_map>

#include <cstddef>
#include <vector>

namespace lcsm
{
	class LCSM_API Snapshot
	{
	  public:
		Snapshot() = default;

		Snapshot(const Snapshot &other);
		Snapshot(Snapshot &&other) noexcept;

		Snapshot &operator=(const Snapshot &other);
		Snapshot &operator=(Snapshot &&other) noexcept;

		void swap(Snapshot &other) noexcept;

		bool equalsStrict(const Snapshot &other) const;
		bool equalsValues(const Snapshot &other) const;

		support::PointerView< Context > allocate(Identifier id, std::size_t contextSize, std::size_t privateContextSize);
		support::PointerView< Context > at(Identifier id);

		std::size_t valueSize(Identifier id) const;
		const DataBits &valueOf(Identifier id, std::size_t i) const;
		const std::vector< DataBits > &valuesOf(Identifier id) const;

		void clearPollution() noexcept;

		bool empty() const noexcept;
		explicit operator bool() const noexcept;

	  private:
		std::unordered_map< Identifier, Context > m_contexts;
	};
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_SNAPSHOT_H */
