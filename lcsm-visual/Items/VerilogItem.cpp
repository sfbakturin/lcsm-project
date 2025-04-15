#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/MovableItem.h>
#include <Items/VerilogItem.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Verilog.h>

#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

VerilogItem::VerilogItem(CoreScene *scene, lcsm::model::VerilogModule *verilogModule, GUIOptions *options) :
	MovableItem(scene, verilogModule->id(), options), m_verilogModule(verilogModule)
{
}

Item::ItemType VerilogItem::ty() const noexcept
{
	return Item::ItemType::VerilogItemTy;
}

void VerilogItem::reid() noexcept
{
	m_id = m_verilogModule->id();
}

QRectF VerilogItem::boundingRect() const
{
	return QRectF();
}

void VerilogItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

QPointF VerilogItem::relativePositionOfPort(lcsm::portid_t portId) const
{
	return QPointF();
}

void VerilogItem::setProperty(int key, const QVariant &value) {}

void VerilogItem::setPropertiesList(PropertiesList *propertiesList) {}

lcsm::Component *VerilogItem::component() noexcept
{
	return m_verilogModule;
}

void VerilogItem::connect() {}

bool VerilogItem::rotateActionEnabled() const noexcept
{
	return true;
}

bool VerilogItem::putValueActionEnabled() const noexcept
{
	return false;
}

lcsm::width_t VerilogItem::inputWidth() const noexcept
{
	return lcsm::Width::QuadWord;
}
