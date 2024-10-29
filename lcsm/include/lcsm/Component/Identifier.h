#ifndef LCSM_COMPONENT_IDENTIFIER_H
#define LCSM_COMPONENT_IDENTIFIER_H

#include <cstddef>
#include <functional>

namespace lcsm
{
	using id_t = unsigned;

	class Identifier
	{
	  public:
		using value_type = id_t;

		Identifier() noexcept;
		Identifier(const Identifier &other) noexcept;
		Identifier(Identifier &&other) noexcept;

		Identifier &operator=(const Identifier &other) noexcept;
		Identifier &operator=(Identifier &&other) noexcept;

		void swap(Identifier &other) noexcept;

		Identifier next() const noexcept;
		std::size_t hashCode() const noexcept;

		bool operator<(const Identifier &other) const noexcept;
		bool operator>(const Identifier &other) const noexcept;
		bool operator==(const Identifier &other) const noexcept;
		bool operator!=(const Identifier &other) const noexcept;
		bool operator<=(const Identifier &other) const noexcept;
		bool operator>=(const Identifier &other) const noexcept;

	  private:
		Identifier(value_type id) noexcept;

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

#endif /* LCSM_COMPONENT_IDENTIFIER_H */
