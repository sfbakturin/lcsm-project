#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/CircuitItem.h>
#include <Items/Item.h>
#include <View/PropertiesList.h>
#include <lcsm/LCSMCircuit.h>

#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

CircuitItem::CircuitItem(CoreScene *scene, const lcsm::LCSMCircuitView &view, GUIOptions *options) :
	MovableItem(scene, view.id(), options), m_view(view)
{
}

Item::ItemType CircuitItem::ty() const noexcept
{
	return Item::ItemType::CircuitItemTy;
}

void CircuitItem::reid() noexcept
{
	m_id = m_view.id();
}

QRectF CircuitItem::boundingRect() const
{
	return QRectF();
}

void CircuitItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

QPointF CircuitItem::relativePositionOfPort(lcsm::portid_t portId) const
{
	return QPointF();
}

void CircuitItem::setProperty(int key, const QVariant &value) {}

void CircuitItem::setPropertiesList(PropertiesList *propertiesList) {}

const lcsm::LCSMCircuitView &CircuitItem::circuit() const noexcept
{
	return m_view;
}

void CircuitItem::connect() {}

bool CircuitItem::rotateActionEnabled() const noexcept
{
	return true;
}

bool CircuitItem::putValueActionEnabled() const noexcept
{
	return false;
}
