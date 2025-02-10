#ifndef LCSM_PHYSICAL_TIMESTAMP_H
#define LCSM_PHYSICAL_TIMESTAMP_H

#include <cstddef>
#include <cstdint>
#include <functional>

namespace lcsm
{
	using timescale_t = std::uint64_t;

	class Timestamp
	{
	  public:
		Timestamp() noexcept;

		Timestamp(const Timestamp &other) noexcept;
		Timestamp(Timestamp &&other) noexcept;

		Timestamp &operator=(const Timestamp &other) noexcept;
		Timestamp &operator=(Timestamp &&other) noexcept;

		void swap(Timestamp &other) noexcept;

		Timestamp next() const noexcept;
		bool isReset() const noexcept;

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
