// #include <lcsm/Model/Circuit.h>
// #include <lcsm/Model/Circuit/Splitter.h>
// #include <lcsm/Model/Identifier.h>
// #include <lcsm/Model/Width.h>
// #include <lcsm/Model/Wire.h>
// #include <lcsm/Support/PointerView.hpp>

// lcsm::model::Splitter::Splitter(lcsm::Width widthIn, lcsm::width_t widthOut) : m_widthIn(widthIn),
// m_widthOut(widthOut)
// {
// 	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
// 	m_
// }

// lcsm::model::Splitter::Splitter(lcsm::Width widthIn, std::size_t widthOut) : m_widthIn(widthIn), m_widthOut(widthOut)
// {
// 	resetConnectors();
// }

// lcsm::model::Splitter::Splitter(const lcsm::model::Splitter &other) :
// 	m_widthIn(other.m_widthIn), m_widthOut(other.m_widthOut)
// {
// 	for (std::size_t i = 0; i < lcsm::model::Splitter::CONNECTORS_SIZE; i++)
// 		m_connectors[i] = other.m_connectors[i];
// }

// lcsm::model::Splitter::Splitter(lcsm::model::Splitter &&other) noexcept :
// 	m_widthIn(other.m_widthIn), m_widthOut(other.m_widthOut)
// {
// 	for (std::size_t i = 0; i < lcsm::model::Splitter::CONNECTORS_SIZE; i++)
// 		m_connectors[i] = other.m_connectors[i];
// }

// lcsm::model::Splitter &lcsm::model::Splitter::operator=(const lcsm::model::Splitter &other)
// {
// 	if (this != &other)
// 		lcsm::model::Splitter(other).swap(*this);
// 	return *this;
// }

// lcsm::model::Splitter &lcsm::model::Splitter::operator=(lcsm::model::Splitter &&other) noexcept
// {
// 	if (this != &other)
// 		lcsm::model::Splitter(std::move(other)).swap(*this);
// 	return *this;
// }

// void lcsm::model::Splitter::swap(lcsm::model::Splitter &other) noexcept
// {
// 	std::swap(m_widthIn, other.m_widthIn);
// 	std::swap(m_widthOut, other.m_widthOut);
// 	for (std::size_t i = 0; i < lcsm::model::Splitter::CONNECTORS_SIZE; i++)
// 		std::swap(m_connectors[i], other.m_connectors[i]);
// }

// lcsm::Width lcsm::model::Splitter::widthIn() const noexcept
// {
// 	return m_widthIn;
// }

// std::size_t lcsm::model::Splitter::widthOut() const noexcept
// {
// 	return m_widthOut;
// }

// void lcsm::model::Splitter::setWidthIn(lcsm::Width newWidthIn) noexcept
// {
// 	m_widthIn = newWidthIn;
// 	resetConnectors();
// }

// void lcsm::model::Splitter::setWidthOut(std::size_t newWidthOut) noexcept
// {
// 	m_widthOut = newWidthOut;
// 	resetConnectors();
// }

// lcsm::Identifier lcsm::model::Splitter::ID() const noexcept
// {
// 	return m_id;
// }

// lcsm::Identifier lcsm::model::Splitter::identify(lcsm::Identifier ID) noexcept
// {
// 	m_id = std::move(ID);
// 	lcsm::Identifier next = m_wireIn.identify(m_id.next());
// 	for (std::size_t i = 0; i < lcsm::model::Splitter::CONNECTORS_SIZE; i++)
// 		next = m_wireOut[i].identify(next.next());
// 	return next;
// }

// void lcsm::model::Splitter::connectIn(lcsm::wire_t &wire, std::size_t i)
// {
// 	if (i != 0)
// 		throw std::logic_error("Splitter element have only 1 input.");
// 	m_wireIn.connectWire(wire);
// }

// void lcsm::model::Splitter::connectOut(lcsm::wire_t &wire, std::size_t i)
// {
// 	if (i >= m_widthOut)
// 		throw std::logic_error(
// 			"Splitter element has only widthOut output "
// 			"connections.");
// 	m_wireOut[i].connectWire(wire);
// }

// void lcsm::model::Splitter::connectBits(lcsm::wire_t &wire)
// {
// 	connectIn(wire, 0);
// }

// void lcsm::model::Splitter::connectBitN(lcsm::wire_t &wire, std::size_t i)
// {
// 	connectOut(wire, i);
// }

// lcsm::CircuitComponentType lcsm::model::Splitter::circuitComponentType() const noexcept
// {
// 	return lcsm::CircuitComponentType::CIRCUIT_COMP_SPLITTER;
// }

// lcsm::model::Splitter *lcsm::model::Splitter::asSplitter() noexcept
// {
// 	return this;
// }

// const lcsm::model::Splitter *lcsm::model::Splitter::asSplitter() const noexcept
// {
// 	return this;
// }

// static constexpr std::size_t Min(std::size_t left, std::size_t right) noexcept
// {
// 	return left < right ? left : right;
// }

// void lcsm::model::Splitter::resetConnectors() noexcept
// {
// 	for (std::size_t i = 0; i < lcsm::model::Splitter::CONNECTORS_SIZE; i++)
// 		m_connectors[i] = lcsm::model::Splitter::NO_CONNECT;

// 	const std::size_t dem = Min(m_widthIn, m_widthOut);
// 	const std::size_t base = m_widthIn / dem;
// 	const std::size_t remain = m_widthIn % dem;

// 	for (std::size_t i = 0; i < dem; i++)
// 	{
// 		m_connectors[i] = base;
// 		if (i < remain)
// 			m_connectors[i]++;
// 	}
// }
