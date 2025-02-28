#ifndef LCSM_PHYSICAL_CONTEXT_H
#define LCSM_PHYSICAL_CONTEXT_H

#include <initializer_list>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Timestamp.h>

#include <cstddef>
#include <vector>

namespace lcsm
{
	class PrivateContext
	{
		union PrivateContextData
		{
			bool m_bool;
			char m_char;
			short m_short;
			int m_int;
			float m_float;
			double m_double;

			PrivateContextData() noexcept;
		};

	  public:
		PrivateContext(std::size_t n);

		PrivateContext(const PrivateContext &other);
		PrivateContext(PrivateContext &&other) noexcept;

		PrivateContext &operator=(const PrivateContext &other);
		PrivateContext &operator=(PrivateContext &&other) noexcept;

		void swap(PrivateContext &other) noexcept;

		void putBool(std::size_t i, bool boolValue);
		void putChar(std::size_t i, char charValue);
		void putShort(std::size_t i, short shortValue);
		void putInt(std::size_t i, int intValue);
		void putFloat(std::size_t i, float floatValue);
		void putDouble(std::size_t i, double doubleValue);

		bool asBool(std::size_t i) const;
		char asChar(std::size_t i) const;
		short asShort(std::size_t i) const;
		int asInt(std::size_t i) const;
		float asFloat(std::size_t i) const;
		double asDouble(std::size_t i) const;

		std::size_t size() const noexcept;

	  private:
		std::vector< PrivateContextData > m_data;
	};

	class Context
	{
	  public:
		Context();
		Context(std::size_t size, std::size_t privateSize);

		Context(const Context &other);
		Context(Context &&other) noexcept;

		Context &operator=(const Context &other);
		Context &operator=(Context &&other) noexcept;

		void swap(Context &other) noexcept;

		std::size_t size() const noexcept;
		const Timestamp &lastUpdate() const noexcept;
		const DataBits &getValue(std::size_t i = 0) const;
		const std::vector< DataBits > &values() const noexcept;

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

		PrivateContext &privateContext() noexcept;
		const PrivateContext &privateContext() const noexcept;

	  private:
		std::vector< DataBits > m_databits;
		Timestamp m_timestamp;
		PrivateContext m_private;
		bool m_updating;
	};
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_CONTEXT_H */
