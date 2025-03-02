#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Splitter.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Support/StaticArray.hpp>

#include <cmath>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Splitter::Splitter(lcsm::Width widthIn, lcsm::width_t widthOut) :
	lcsm::model::Splitter("", widthIn, widthOut)
{
}

lcsm::model::Splitter::Splitter(lcsm::label_t name, lcsm::Width widthIn, lcsm::width_t widthOut) :
	lcsm::Circuit(name), m_widthIn(widthIn), m_widthOut(widthOut)
{
}

lcsm::model::Splitter::~Splitter() noexcept
{
	disconnectAll();
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
	resetBitsOuts();
}

const std::pair< std::size_t, std::size_t > &lcsm::model::Splitter::bitsOut(lcsm::portid_t portId) const noexcept
{
	return m_bitsOuts[portId];
}

const lcsm::model::Wire *lcsm::model::Splitter::wireIn() const noexcept
{
	return m_wireIn.get();
}

const lcsm::model::Wire *lcsm::model::Splitter::wireOut(lcsm::portid_t portId) const noexcept
{
	return m_wireOuts[portId].get();
}

std::size_t lcsm::model::Splitter::numOfWires() const noexcept
{
	return lcsm::Width::LastWidth + 1;
}

void lcsm::model::Splitter::provideWires(const std::vector< std::shared_ptr< lcsm::model::Wire > > &wires)
{
	if (wires.size() != numOfWires())
		throw std::logic_error("Bad num of wires!");
	for (std::size_t i = 0; i < numOfWires() - 1; i++)
	{
		m_wireOuts.push_back(wires[i]);
		m_wireOuts[i]->connectConnect(this);
	}
	m_wireIn = wires[numOfWires() - 1];
	m_wireIn->connectConnect(this);
	resetBitsOuts();
}

lcsm::Identifier lcsm::model::Splitter::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Splitter::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	lcsm::Identifier next = m_wireIn->identify(m_id.next());
	for (std::size_t i = 0; i < m_wireOuts.size(); i++)
		next = m_wireOuts[i]->identify(next.next());
	return next;
}

lcsm::object_type_t lcsm::model::Splitter::objectType() const noexcept
{
	return lcsm::ObjectType::Internal;
}

lcsm::CircuitType lcsm::model::Splitter::circuitType() const noexcept
{
	return lcsm::CircuitType::Splitter;
}

void lcsm::model::Splitter::connect(lcsm::portid_t portId, lcsm::Circuit *circuit)
{
	lcsm::model::Wire *wire = static_cast< lcsm::model::Wire * >(byPort(portId));
	if (!wire)
		throw std::logic_error("Bad port!");
	wire->connectToWire(circuit);
}

lcsm::Circuit *lcsm::model::Splitter::byPort(lcsm::portid_t portId) noexcept
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
	{
		if (p > m_widthOut)
		{
			return nullptr;
		}
		return m_wireOuts[p].get();
	}
	case lcsm::model::Splitter::Port::Input:
	{
		return m_wireIn.get();
	}
	}
	return nullptr;
}

lcsm::portid_t lcsm::model::Splitter::findPort(const lcsm::Circuit *circuit) const noexcept
{
	if (circuit == m_wireIn.get())
	{
		return lcsm::model::Splitter::Port::Input;
	}
	else
	{
		for (lcsm::width_t i = 0; i < m_widthOut; i++)
		{
			if (circuit == m_wireOuts[i].get())
			{
				return static_cast< lcsm::portid_t >(i);
			}
		}
		return -1;
	}
}

lcsm::portid_t lcsm::model::Splitter::defaultPort() const noexcept
{
	return lcsm::model::Splitter::Port::Input;
}

void lcsm::model::Splitter::disconnect(lcsm::Circuit *) noexcept
{
	// Do nothing.
}

void lcsm::model::Splitter::disconnectAll() noexcept
{
	m_wireIn->disconnectAll();
	for (std::size_t i = 0; i < m_wireOuts.size(); i++)
		m_wireOuts[i]->disconnectAll();
	m_wireOuts.clear();
}

static inline constexpr std::size_t min(std::size_t left, std::size_t right) noexcept
{
	return left < right ? left : right;
}

void lcsm::model::Splitter::resetBitsOuts() noexcept
{
	// Clear from old pairs.
	m_bitsOuts.clear();

	// Get sizes.
	const std::size_t dem = min(m_widthIn, m_widthOut);
	const std::size_t base = static_cast< std::size_t >(std::floor(static_cast< float >(m_widthIn) / dem));
	const std::size_t remain = m_widthIn % dem;

	// Generate pairs.
	lcsm::width_t prevBegin = 0;
	lcsm::width_t prevEnd = base;
	for (std::size_t i = 0; i < dem; i++)
	{
		m_bitsOuts.emplace_back(prevBegin, prevEnd);
		prevEnd += base;
		prevBegin = prevEnd + 1;
		prevEnd += (i < remain);
	}
}
