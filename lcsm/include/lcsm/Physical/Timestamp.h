#ifndef LCSM_PHYSICAL_TIMESTAMP_H
#define LCSM_PHYSICAL_TIMESTAMP_H

#include <lcsm/lcsmconfig.h>

#include <cstddef>
#include <cstdint>
#include <functional>

namespace lcsm
{
	using timescale_t = std::uint64_t;

	class LCSM_API Timestamp
	{
	  public:
		Timestamp() noexcept;
		Timestamp(timescale_t tick, timescale_t subtick) noexcept;

		Timestamp(const Timestamp &other) noexcept;
		Timestamp(Timestamp &&other) noexcept;

		Timestamp &operator=(const Timestamp &other) noexcept;
		Timestamp &operator=(Timestamp &&other) noexcept;

		void swap(Timestamp &other) noexcept;

		timescale_t ticks() const noexcept;
		timescale_t subticks() const noexcept;

		Timestamp next() const noexcept;
		Timestamp subnext() const noexcept;
		bool isReset() const noexcept;

		void reset() noexcept;

		bool operator<(const Timestamp &other) const noexcept;
		bool operator<=(const Timestamp &other) const noexcept;
		bool operator==(const Timestamp &other) const noexcept;
		bool operator!=(const Timestamp &other) const noexcept;
		bool operator>(const Timestamp &other) const noexcept;
		bool operator>=(const Timestamp &other) const noexcept;

		Timestamp operator+(const Timestamp &other) const noexcept;

		Timestamp &operator+=(const Timestamp &other) noexcept;

		Timestamp &operator++() noexcept;
		Timestamp operator++(int) noexcept;

		std::size_t hashCode() const noexcept;

	  private:
		timescale_t m_tick;
		timescale_t m_subtick;
	};
}	 // namespace lcsm

namespace std
{
	template<>
	struct hash< lcsm::Timestamp >
	{
		std::size_t operator()(const lcsm::Timestamp &object) const { return object.hashCode(); }
	};
}	 // namespace std

#endif /* LCSM_PHYSICAL_TIMESTAMP_H */
