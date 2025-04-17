#ifndef LCSM_VISUAL_ITEMS_VERILOGITEM_H
#define LCSM_VISUAL_ITEMS_VERILOGITEM_H

#include <GUI/GUIOptions.h>
#include <Items/MovableItem.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/Verilog.h>
#include <lcsm/Verilog/Module.h>

#include <QList>
#include <QPainter>
#include <QPair>
#include <QPointF>
#include <QRectF>
#include <QString>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

class CoreScene;
class PropertiesList;

class VerilogItem : public MovableItem
{
  private:
	enum class PortTypeItem
	{
		Input,
		Output,
		Inout
	};

  public:
	VerilogItem(CoreScene *scene, lcsm::model::VerilogModule *verilogModule, const lcsm::verilog::Module *module, GUIOptions *options = nullptr);

	virtual ItemType ty() const noexcept override final;
	virtual void reid() noexcept override final;

	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	virtual QPointF relativePositionOfPort(lcsm::portid_t portId) const override final;

	virtual void setProperty(int key, const QVariant &value) override final;
	virtual void setPropertiesList(PropertiesList *propertiesList) override final;

	lcsm::Component *component() noexcept;

  protected:
	virtual void connect() override final;

	virtual bool rotateActionEnabled() const noexcept override final;
	virtual bool putValueActionEnabled() const noexcept override final;

	virtual lcsm::width_t inputWidth() const noexcept override final;

  private:
	qreal verilogWidth() const noexcept;
	qreal verilogHeight() const noexcept;

	QPair< PortTypeItem, int > getTypeAndIndexFrom(lcsm::portid_t portId) const;

  private:
	lcsm::model::VerilogModule *m_verilogModule;
	QList< QString > m_portStrings;
	int m_facingKey;
};

#endif /* LCSM_VISUAL_ITEMS_VERILOGITEM_H */
