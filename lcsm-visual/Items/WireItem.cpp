#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/Item.h>
#include <Items/MovableItem.h>
#include <Items/WireItem.h>
#include <Items/WireLine.h>
#include <View/PropertiesList.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/Wire.h>

#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>
#include <string>
#include <utility>

static constexpr lcsm::portid_t PORT = lcsm::model::Wire::Port::Wiring;

WireItem::WireItem(CoreScene *coreScene, lcsm::model::Wire *wire, GUIOptions *options) :
	MovableItem(coreScene, wire->id(), options), m_wire(wire), m_nameKey(0)
{
}

Item::ItemType WireItem::ty() const noexcept
{
	return Item::ItemType::WireItemTy;
}

void WireItem::reid() noexcept
{
	m_id = m_wire->id();
}

lcsm::model::Wire *WireItem::wire() noexcept
{
	return m_wire;
}

QRectF WireItem::boundingRect() const
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	const qreal width = static_cast< qreal >(2 * gridSize);
	const qreal r = static_cast< qreal >(width) * 0.2;
	return QRectF(-r / 2.0, -r / 2.0, r, r);
}

void WireItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->drawRect(boundingRect());
}

QPointF WireItem::relativePositionOfPort(lcsm::portid_t portId) const
{
	const lcsm::model::Wire::Port p = static_cast< lcsm::model::Wire::Port >(portId);

	switch (p)
	{
	case lcsm::model::Wire::Port::Wiring:
	{
		return QPointF(0.0, 0.0);
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Bad port!");
}

void WireItem::setProperty(int key, const QVariant &value)
{
	if (key == m_nameKey)
	{
		const QString s = value.toString();
		m_wire->setName(s.toStdString());
	}
}

void WireItem::setPropertiesList(PropertiesList *propertiesList)
{
	// Name
	{
		const QString name = QString::fromStdString(m_wire->name());
		m_nameKey = propertiesList->addEditableItem(QObject::tr("Name"), name);
	}
}

void WireItem::connect()
{
	m_coreScene->connection(id(), PORT);
}

bool WireItem::rotateActionEnabled() const noexcept
{
	return false;
}

bool WireItem::putValueActionEnabled() const noexcept
{
	return false;
}

lcsm::width_t WireItem::inputWidth() const noexcept
{
	return lcsm::Width::QuadWord;
}
