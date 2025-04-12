#ifndef LCSM_VISUAL_ITEMS_MOVABLEITEM_H
#define LCSM_VISUAL_ITEMS_MOVABLEITEM_H

#include <GUI/GUIOptions.h>
#include <Items/Item.h>
#include <lcsm/Model/Identifier.h>

#include <QGraphicsItem>
#include <QVariant>

class CoreScene;

class MovableItem : public Item
{
  public:
	MovableItem(CoreScene *coreScene, lcsm::Identifier id, GUIOptions *options = nullptr);
	virtual ~MovableItem() noexcept = default;

  protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif /* LCSM_VISUAL_ITEMS_MOVABLEITEM_H */
