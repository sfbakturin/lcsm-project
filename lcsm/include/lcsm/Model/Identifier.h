#ifndef LCSM_MODEL_IDENTIFIER_H
#define LCSM_MODEL_IDENTIFIER_H

#include <lcsm/LCSM.h>

#include <cstddef>
#include <functional>
#include <ostream>
#include <string>

namespace lcsm
{
	class Identifier
	{
	  public:
		using value_type = id_t;

		Identifier() noexcept;
		Identifier(value_type startId) noexcept;
		Identifier(const Identifier &other) noexcept;
		Identifier(Identifier &&other) noexcept;

		Identifier &operator=(const Identifier &other) noexcept;
		Identifier &operator=(Identifier &&other) noexcept;

		void swap(Identifier &other) noexcept;

		Identifier next() const noexcept;
		std::size_t hashCode() const noexcept;
		std::string toString() const;

		bool operator<(const Identifier &other) const noexcept;
		bool operator>(const Identifier &other) const noexcept;
		bool operator==(const Identifier &other) const noexcept;
		bool operator!=(const Identifier &other) const noexcept;
		bool operator<=(const Identifier &other) const noexcept;
		bool operator>=(const Identifier &other) const noexcept;

		friend std::ostream &operator<<(std::ostream &os, const Identifier &id);

	  private:
		value_type m_id;
	};
}	 // namespace lcsm

namespace std
{
	template<>
	struct hash< lcsm::Identifier >
	{
		std::size_t operator()(const lcsm::Identifier &object) const { return object.hashCode(); }
	};
}	 // namespace std

#endif /* LCSM_MODEL_IDENTIFIER_H */
