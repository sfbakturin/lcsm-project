#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Splitter.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Support/StaticArray.hpp>

#include <cstddef>
#include <memory>
#include <stdexcept>

lcsm::model::Splitter::Splitter(lcsm::Width widthIn, lcsm::width_t widthOut) : m_widthIn(widthIn), m_widthOut(widthOut)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	for (std::size_t i = 0; i < m_wireOut.size(); i++)
		m_wireOut[i].connectConnect(circuit);
	resetBitmasks();
}

lcsm::Width lcsm::model::Splitter::widthIn() const noexcept
{
	return m_widthIn;
}

void lcsm::model::Splitter::setWidthIn(lcsm::Width widthIn) noexcept
{
	m_widthIn = widthIn;
}

lcsm::width_t lcsm::model::Splitter::widthOut() const noexcept
{
	return m_widthOut;
}

void lcsm::model::Splitter::setWidthOut(lcsm::width_t widthOut)
{
	if (widthOut == 0 || widthOut > 64)
		throw std::logic_error("Integer too big!");
	m_widthOut = widthOut;
	resetBitmasks();
}

lcsm::width_bitmask_t lcsm::model::Splitter::bitsOut(lcsm::portid_t portId) const noexcept
{
	return m_bitsOut[portId];
}

const lcsm::model::Wire &lcsm::model::Splitter::wireIn() const noexcept
{
	return m_wireIn;
}

const lcsm::model::Wire &lcsm::model::Splitter::wireOut(lcsm::portid_t portId) const noexcept
{
	return m_wireOut[portId];
}

lcsm::Identifier lcsm::model::Splitter::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Splitter::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	lcsm::Identifier next = m_wireIn.identify(m_id.next());
	for (std::size_t i = 0; i < m_wireOut.size(); i++)
		next = m_wireOut[i].identify(next.next());
	return next;
}

lcsm::ObjectType lcsm::model::Splitter::objectType() const noexcept
{
	return lcsm::ObjectType::PureInt;
}

lcsm::CircuitType lcsm::model::Splitter::circuitType() const noexcept
{
	return lcsm::CircuitType::Splitter;
}

void lcsm::model::Splitter::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Splitter::Port p = static_cast< lcsm::model::Splitter::Port >(portId);
	switch (p)
	{
	case lcsm::model::Splitter::Port::Out0:
	case lcsm::model::Splitter::Port::Out1:
	case lcsm::model::Splitter::Port::Out2:
	case lcsm::model::Splitter::Port::Out3:
	case lcsm::model::Splitter::Port::Out4:
	case lcsm::model::Splitter::Port::Out5:
	case lcsm::model::Splitter::Port::Out6:
	case lcsm::model::Splitter::Port::Out7:
	case lcsm::model::Splitter::Port::Out8:
	case lcsm::model::Splitter::Port::Out9:
	case lcsm::model::Splitter::Port::Out10:
	case lcsm::model::Splitter::Port::Out11:
	case lcsm::model::Splitter::Port::Out12:
	case lcsm::model::Splitter::Port::Out13:
	case lcsm::model::Splitter::Port::Out14:
	case lcsm::model::Splitter::Port::Out15:
	case lcsm::model::Splitter::Port::Out16:
	case lcsm::model::Splitter::Port::Out17:
	case lcsm::model::Splitter::Port::Out18:
	case lcsm::model::Splitter::Port::Out19:
	case lcsm::model::Splitter::Port::Out20:
	case lcsm::model::Splitter::Port::Out21:
	case lcsm::model::Splitter::Port::Out22:
	case lcsm::model::Splitter::Port::Out23:
	case lcsm::model::Splitter::Port::Out24:
	case lcsm::model::Splitter::Port::Out25:
	case lcsm::model::Splitter::Port::Out26:
	case lcsm::model::Splitter::Port::Out27:
	case lcsm::model::Splitter::Port::Out28:
	case lcsm::model::Splitter::Port::Out29:
	case lcsm::model::Splitter::Port::Out30:
	case lcsm::model::Splitter::Port::Out31:
	case lcsm::model::Splitter::Port::Out32:
	case lcsm::model::Splitter::Port::Out33:
	case lcsm::model::Splitter::Port::Out34:
	case lcsm::model::Splitter::Port::Out35:
	case lcsm::model::Splitter::Port::Out36:
	case lcsm::model::Splitter::Port::Out37:
	case lcsm::model::Splitter::Port::Out38:
	case lcsm::model::Splitter::Port::Out39:
	case lcsm::model::Splitter::Port::Out40:
	case lcsm::model::Splitter::Port::Out41:
	case lcsm::model::Splitter::Port::Out42:
	case lcsm::model::Splitter::Port::Out43:
	case lcsm::model::Splitter::Port::Out44:
	case lcsm::model::Splitter::Port::Out45:
	case lcsm::model::Splitter::Port::Out46:
	case lcsm::model::Splitter::Port::Out47:
	case lcsm::model::Splitter::Port::Out48:
	case lcsm::model::Splitter::Port::Out49:
	case lcsm::model::Splitter::Port::Out50:
	case lcsm::model::Splitter::Port::Out51:
	case lcsm::model::Splitter::Port::Out52:
	case lcsm::model::Splitter::Port::Out53:
	case lcsm::model::Splitter::Port::Out54:
	case lcsm::model::Splitter::Port::Out55:
	case lcsm::model::Splitter::Port::Out56:
	case lcsm::model::Splitter::Port::Out57:
	case lcsm::model::Splitter::Port::Out58:
	case lcsm::model::Splitter::Port::Out59:
	case lcsm::model::Splitter::Port::Out60:
	case lcsm::model::Splitter::Port::Out61:
	case lcsm::model::Splitter::Port::Out62:
	case lcsm::model::Splitter::Port::Out63:
		if (p > m_widthOut)
			throw std::logic_error("Bad");
		m_wireOut[p].connectToWire(circuit);
		break;
	case lcsm::model::Splitter::Port::Input:
		m_wireIn.connectToWire(circuit);
		break;
	default:
		throw std::logic_error("Bad port!");
	}
}

lcsm::Circuit *lcsm::model::Splitter::byPort(lcsm::portid_t portId)
{
	const lcsm::model::Splitter::Port p = static_cast< lcsm::model::Splitter::Port >(portId);
	switch (p)
	{
	case lcsm::model::Splitter::Port::Out0:
	case lcsm::model::Splitter::Port::Out1:
	case lcsm::model::Splitter::Port::Out2:
	case lcsm::model::Splitter::Port::Out3:
	case lcsm::model::Splitter::Port::Out4:
	case lcsm::model::Splitter::Port::Out5:
	case lcsm::model::Splitter::Port::Out6:
	case lcsm::model::Splitter::Port::Out7:
	case lcsm::model::Splitter::Port::Out8:
	case lcsm::model::Splitter::Port::Out9:
	case lcsm::model::Splitter::Port::Out10:
	case lcsm::model::Splitter::Port::Out11:
	case lcsm::model::Splitter::Port::Out12:
	case lcsm::model::Splitter::Port::Out13:
	case lcsm::model::Splitter::Port::Out14:
	case lcsm::model::Splitter::Port::Out15:
	case lcsm::model::Splitter::Port::Out16:
	case lcsm::model::Splitter::Port::Out17:
	case lcsm::model::Splitter::Port::Out18:
	case lcsm::model::Splitter::Port::Out19:
	case lcsm::model::Splitter::Port::Out20:
	case lcsm::model::Splitter::Port::Out21:
	case lcsm::model::Splitter::Port::Out22:
	case lcsm::model::Splitter::Port::Out23:
	case lcsm::model::Splitter::Port::Out24:
	case lcsm::model::Splitter::Port::Out25:
	case lcsm::model::Splitter::Port::Out26:
	case lcsm::model::Splitter::Port::Out27:
	case lcsm::model::Splitter::Port::Out28:
	case lcsm::model::Splitter::Port::Out29:
	case lcsm::model::Splitter::Port::Out30:
	case lcsm::model::Splitter::Port::Out31:
	case lcsm::model::Splitter::Port::Out32:
	case lcsm::model::Splitter::Port::Out33:
	case lcsm::model::Splitter::Port::Out34:
	case lcsm::model::Splitter::Port::Out35:
	case lcsm::model::Splitter::Port::Out36:
	case lcsm::model::Splitter::Port::Out37:
	case lcsm::model::Splitter::Port::Out38:
	case lcsm::model::Splitter::Port::Out39:
	case lcsm::model::Splitter::Port::Out40:
	case lcsm::model::Splitter::Port::Out41:
	case lcsm::model::Splitter::Port::Out42:
	case lcsm::model::Splitter::Port::Out43:
	case lcsm::model::Splitter::Port::Out44:
	case lcsm::model::Splitter::Port::Out45:
	case lcsm::model::Splitter::Port::Out46:
	case lcsm::model::Splitter::Port::Out47:
	case lcsm::model::Splitter::Port::Out48:
	case lcsm::model::Splitter::Port::Out49:
	case lcsm::model::Splitter::Port::Out50:
	case lcsm::model::Splitter::Port::Out51:
	case lcsm::model::Splitter::Port::Out52:
	case lcsm::model::Splitter::Port::Out53:
	case lcsm::model::Splitter::Port::Out54:
	case lcsm::model::Splitter::Port::Out55:
	case lcsm::model::Splitter::Port::Out56:
	case lcsm::model::Splitter::Port::Out57:
	case lcsm::model::Splitter::Port::Out58:
	case lcsm::model::Splitter::Port::Out59:
	case lcsm::model::Splitter::Port::Out60:
	case lcsm::model::Splitter::Port::Out61:
	case lcsm::model::Splitter::Port::Out62:
	case lcsm::model::Splitter::Port::Out63:
		if (p > m_widthOut)
			return nullptr;
		return std::addressof(m_wireOut[p]);
	case lcsm::model::Splitter::Port::Input:
		return std::addressof(m_wireIn);
	}
	return nullptr;
}

static inline constexpr std::size_t Min(std::size_t left, std::size_t right) noexcept
{
	return left < right ? left : right;
}

void lcsm::model::Splitter::resetBitmasks() noexcept
{
	// TODO: Remake to bitmasks.

	static constexpr lcsm::width_bitmask_t NoConnect = 0;
	for (std::size_t i = 0; i < m_bitsOut.size(); i++)
		m_bitsOut[i] = NoConnect;

	const std::size_t dem = Min(m_widthIn, m_widthOut);
	const std::size_t base = m_widthIn / dem;
	const std::size_t remain = m_widthIn % dem;

	for (std::size_t i = 0; i < dem; i++)
	{
		m_bitsOut[i] = base;
		if (i < remain)
			m_bitsOut[i]++;
	}
}
