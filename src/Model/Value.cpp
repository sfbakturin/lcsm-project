#include <sim/Model/Value.h>

sim::Value::Value() noexcept : m_state(sim::Value::State::UNDEFINED) {}

sim::Value::Value(const Value& other) noexcept : m_state(other.m_state) {}

sim::Value::Value(Value&& other) noexcept : m_state(other.m_state) {}
