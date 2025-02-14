#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wire.h>
#include <lcsm/Model/std/Digit.h>
#include <lcsm/Support/PointerView.hpp>

#include <memory>
#include <stdexcept>
#include <utility>

lcsm::model::Digit::Digit(bool hasDecimalPoint) : m_hasDecimalPoint(hasDecimalPoint)
{
	const lcsm::support::PointerView< lcsm::Circuit > circuit = this;
	m_wireData.connectConnect(circuit);
	m_wireDecimalPoint.connectConnect(circuit);
}

bool lcsm::model::Digit::hasDecimalPoint() const noexcept
{
	return m_hasDecimalPoint;
}

void lcsm::model::Digit::setHasDecimalPoint(bool hasDecimalPoint) noexcept
{
	m_hasDecimalPoint = hasDecimalPoint;
	// TODO: If digit now doesn't have decimal point, then we should erase Wire connections (must be another method of
	// lcsm::model::Wire).
}

const lcsm::model::Wire &lcsm::model::Digit::wireData() const noexcept
{
	return m_wireData;
}

const lcsm::model::Wire &lcsm::model::Digit::wireDecimalPoint() const noexcept
{
	return m_wireDecimalPoint;
}

lcsm::Identifier lcsm::model::Digit::id() const noexcept
{
	return m_id;
}

lcsm::Identifier lcsm::model::Digit::identify(lcsm::Identifier id) noexcept
{
	m_id = std::move(id);
	return m_wireDecimalPoint.identify(m_wireData.identify(m_id.next()));
}

lcsm::ObjectType lcsm::model::Digit::objectType() const noexcept
{
	return lcsm::ObjectType::PureInt;
}

lcsm::CircuitType lcsm::model::Digit::circuitType() const noexcept
{
	return lcsm::CircuitType::Digit;
}

void lcsm::model::Digit::connect(lcsm::portid_t portId, const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	const lcsm::model::Digit::Port p = static_cast< lcsm::model::Digit::Port >(portId);
	switch (p)
	{
	case lcsm::model::Digit::Port::WiringData:
		m_wireData.connectToWire(circuit);
		break;
	case lcsm::model::Digit::Port::WiringDecimalPoint:
		if (!m_hasDecimalPoint)
			throw std::logic_error("Digit doesn't have port for decimal point!");
		m_wireDecimalPoint.connectToWire(circuit);
		break;
	default:
		throw std::logic_error("Bad port!");
	}
}

void lcsm::model::Digit::connectData(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Digit::Port::WiringData, circuit);
}

void lcsm::model::Digit::connectDecimalPoint(const lcsm::support::PointerView< lcsm::Circuit > &circuit)
{
	connect(lcsm::model::Digit::Port::WiringDecimalPoint, circuit);
}

lcsm::Circuit *lcsm::model::Digit::byPort(lcsm::portid_t portId)
{
	const lcsm::model::Digit::Port p = static_cast< lcsm::model::Digit::Port >(portId);
	switch (p)
	{
	case lcsm::model::Digit::Port::WiringData:
		return std::addressof(m_wireData);
	case lcsm::model::Digit::Port::WiringDecimalPoint:
		return m_hasDecimalPoint ? std::addressof(m_wireDecimalPoint) : nullptr;
	}
	return nullptr;
}
