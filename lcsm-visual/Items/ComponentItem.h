#ifndef LCSM_VISUAL_ITEMS_COMPONENTITEM_H
#define LCSM_VISUAL_ITEMS_COMPONENTITEM_H

#include <GUI/GUIOptions.h>
#include <Items/MovableItem.h>
#include <lcsm/Model/Component.h>

#include <QList>
#include <QVariant>

class CoreScene;

class ComponentItem : public MovableItem
{
  public:
	ComponentItem(CoreScene *scene, lcsm::Component *component, GUIOptions *options = nullptr);
	virtual ~ComponentItem() noexcept = default;

	virtual ItemType ty() const noexcept override final;
	virtual void reid() noexcept override final;

	lcsm::Component *component() noexcept;

  protected:
	lcsm::Component *m_component;
};

#endif /* LCSM_VISUAL_ITEMS_COMPONENTITEM_H */
