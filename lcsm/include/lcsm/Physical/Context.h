#ifndef LCSM_PHYSICAL_CONTEXT_H
#define LCSM_PHYSICAL_CONTEXT_H

#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Timestamp.h>

namespace lcsm
{
	class Context
	{
	  public:
		Context() noexcept = default;	 // TODO: Somehow remove default-constructor.
		Context(const Timestamp &timestamp) noexcept;
		Context(Timestamp &&timestamp) noexcept;
		Context(const Timestamp &timestamp, const DataBits &databits) noexcept;
		Context(Timestamp &&timestamp, const DataBits &databits) noexcept;
		Context(const Timestamp &timestamp, DataBits &&databits) noexcept;
		Context(Timestamp &&timestamp, DataBits &&databits) noexcept;

		Context(const Context &other) noexcept;
		Context(Context &&other) noexcept;

		Context &operator=(const Context &other) noexcept;
		Context &operator=(Context &&other) noexcept;

		void swap(Context &other) noexcept;

		const Timestamp &lastUpdate() const noexcept;
		const DataBits &getValue() const noexcept;

		void updateValue(const Timestamp &timestamp, const DataBits &databits) noexcept;
		void updateValue(Timestamp &&timestamp, const DataBits &databits) noexcept;
		void updateValue(const Timestamp &timestamp, DataBits &&databits) noexcept;
		void updateValue(Timestamp &&timestamp, DataBits &&databits) noexcept;

		bool neverUpdate() const noexcept;

	  private:
		Timestamp m_timestamp;
		DataBits m_databits;
	};
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_CONTEXT_H */
