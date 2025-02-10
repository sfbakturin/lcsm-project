#ifndef LCSM_PHYSICAL_CONTEXT_H
#define LCSM_PHYSICAL_CONTEXT_H

#include <initializer_list>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Timestamp.h>

#include <cstddef>
#include <vector>

namespace lcsm
{
	class Context
	{
	  public:
		Context();
		Context(std::size_t n);

		Context(const Context &other);
		Context(Context &&other) noexcept;

		Context &operator=(const Context &other);
		Context &operator=(Context &&other) noexcept;

		void swap(Context &other) noexcept;

		std::size_t size() const noexcept;
		const Timestamp &lastUpdate() const noexcept;
		const DataBits &getValue(std::size_t i = 0) const;

		bool isEqualsValues(const Context &other) const;

		void beginUpdate(const Timestamp &timestamp);
		void beginUpdate(Timestamp &&timestamp);
		void updateValue(std::size_t i, const DataBits &databits);
		void updateValue(std::size_t i, DataBits &&databits);
		void endUpdate();

		void updateValues(const Timestamp &timestamp, const std::vector< DataBits > &databits);
		void updateValues(const Timestamp &timestamp, std::vector< DataBits > &&databits);
		void updateValues(Timestamp &&timestamp, const std::vector< DataBits > &databits);
		void updateValues(Timestamp &&timestamp, std::vector< DataBits > &&databits);
		void updateValues(const Timestamp &timestamp, std::initializer_list< DataBits > databits);
		void updateValues(Timestamp &&timestamp, std::initializer_list< DataBits > databits);

		bool neverUpdate() const noexcept;

	  private:
		std::vector< DataBits > m_databits;
		Timestamp m_timestamp;
		bool m_updating;
	};
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_CONTEXT_H */
