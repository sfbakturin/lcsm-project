#ifndef SIM_COMPONENT_IDENTIFIER_H
#define SIM_COMPONENT_IDENTIFIER_H

#include <cstddef>
#include <functional>

namespace sim
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
}	 // namespace sim

namespace std
{
	template<>
	struct hash< sim::Identifier >
	{
		std::size_t operator()(const sim::Identifier &object) const
		{
			return object.hashCode();
		}
	};
}	 // namespace std

#endif /* SIM_COMPONENT_IDENTIFIER_H */
