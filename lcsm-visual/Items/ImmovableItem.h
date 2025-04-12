#ifndef LCSM_VISUAL_ITEMS_IMMOVABLEITEM_H
#define LCSM_VISUAL_ITEMS_IMMOVABLEITEM_H

#include <GUI/GUIOptions.h>
#include <Items/Item.h>
#include <lcsm/Model/Identifier.h>

class CoreScene;

class ImmovableItem : public Item
{
  public:
	ImmovableItem(CoreScene *coreScene, lcsm::Identifier id, GUIOptions *options = nullptr);
	virtual ~ImmovableItem() noexcept = default;
};

#endif /* LCSM_VISUAL_ITEMS_IMMOVABLEITEM_H */
