#include <lcsm/Physical/Context.h>
#include <lcsm/Support/Algorithm.hpp>

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::PrivateContext::PrivateContextData::PrivateContextData() noexcept : m_int(0) {}

lcsm::PrivateContext::PrivateContext(std::size_t n) : m_data(n) {}

lcsm::PrivateContext::PrivateContext(const PrivateContext &other) : m_data(other.m_data) {}

lcsm::PrivateContext::PrivateContext(PrivateContext &&other) noexcept : m_data(std::move(other.m_data)) {}

lcsm::PrivateContext &lcsm::PrivateContext::operator=(const lcsm::PrivateContext &other)
{
	return lcsm::support::CopyAssign< lcsm::PrivateContext >(this, other);
}

lcsm::PrivateContext &lcsm::PrivateContext::operator=(lcsm::PrivateContext &&other) noexcept
{
	return lcsm::support::MoveAssign< lcsm::PrivateContext >(this, std::move(other));
}

void lcsm::PrivateContext::swap(lcsm::PrivateContext &other) noexcept
{
	std::swap(m_data, other.m_data);
}

void lcsm::PrivateContext::putBool(std::size_t i, bool boolValue)
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	m_data[i].m_bool = boolValue;
}

void lcsm::PrivateContext::putChar(std::size_t i, char charValue)
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	m_data[i].m_char = charValue;
}

void lcsm::PrivateContext::putShort(std::size_t i, short shortValue)
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	m_data[i].m_short = shortValue;
}

void lcsm::PrivateContext::putInt(std::size_t i, int intValue)
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	m_data[i].m_int = intValue;
}

void lcsm::PrivateContext::putFloat(std::size_t i, float floatValue)
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	m_data[i].m_float = floatValue;
}

void lcsm::PrivateContext::putDouble(std::size_t i, double doubleValue)
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	m_data[i].m_double = doubleValue;
}

bool lcsm::PrivateContext::asBool(std::size_t i) const
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	return m_data[i].m_bool;
}

char lcsm::PrivateContext::asChar(std::size_t i) const
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	return m_data[i].m_char;
}

short lcsm::PrivateContext::asShort(std::size_t i) const
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	return m_data[i].m_short;
}

int lcsm::PrivateContext::asInt(std::size_t i) const
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	return m_data[i].m_int;
}

float lcsm::PrivateContext::asFloat(std::size_t i) const
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	return m_data[i].m_float;
}

double lcsm::PrivateContext::asDouble(std::size_t i) const
{
	if (i > size())
		throw std::out_of_range("PrivateContext: index violates data size");
	return m_data[i].m_double;
}

std::size_t lcsm::PrivateContext::size() const noexcept
{
	return m_data.size();
}
