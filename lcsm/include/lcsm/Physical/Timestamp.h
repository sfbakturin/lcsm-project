#ifndef LCSM_PHYSICAL_TIMESTAMP_H
#define LCSM_PHYSICAL_TIMESTAMP_H

#include <lcsm/LCSM.h>

namespace lcsm
{
	class Timestamp
	{
	  public:
		Timestamp() noexcept;
		Timestamp(timer_t tickFast, timer_t tickStatic, timer_t tickComposite, timer_t tickDynamic, timer_t tickUser) noexcept;

		Timestamp(const Timestamp &other) noexcept;
		Timestamp(Timestamp &&other) noexcept;

		Timestamp &operator=(const Timestamp &other) noexcept;
		Timestamp &operator=(Timestamp &&other) noexcept;

		void swap(Timestamp &other) noexcept;

		bool isReset() const noexcept;

		Timestamp tickFast() const noexcept;
		Timestamp tickStatic() const noexcept;
		Timestamp tickComposite() const noexcept;
		Timestamp tickDynamic() const noexcept;
		Timestamp tickUser() const noexcept;

		Timestamp normalizeToDynamic() const noexcept;
		Timestamp normalizeToComposite() const noexcept;
		Timestamp normalizeToStatic() const noexcept;

		bool operator<(const Timestamp &other) const noexcept;
		bool operator<=(const Timestamp &other) const noexcept;
		bool operator==(const Timestamp &other) const noexcept;
		bool operator!=(const Timestamp &other) const noexcept;
		bool operator>(const Timestamp &other) const noexcept;
		bool operator>=(const Timestamp &other) const noexcept;

		Timestamp operator+(const Timestamp &other) const noexcept;
		Timestamp operator-(const Timestamp &other) const noexcept;

		Timestamp &operator+=(const Timestamp &other) noexcept;
		Timestamp &operator-=(const Timestamp &other) noexcept;

		static const Timestamp RESET;

	  private:
		timer_t m_tickFast;
		timer_t m_tickStatic;
		timer_t m_tickComposite;
		timer_t m_tickDynamic;
		timer_t m_tickUser;

		static constexpr timer_t TICK = static_cast< timer_t >(1);
	};
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_TIMESTAMP_H */
