#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/ComponentItem.h>
#include <Items/MovableItem.h>
#include <lcsm/Model/Component.h>

ComponentItem::ComponentItem(CoreScene *scene, lcsm::Component *component, GUIOptions *options) :
	MovableItem(scene, component->id(), options), m_component(component)
{
}

Item::ItemType ComponentItem::ty() const noexcept
{
	return Item::ItemType::ComponentItemTy;
}

void ComponentItem::reid() noexcept
{
	m_id = m_component->id();
}

lcsm::Component *ComponentItem::component() noexcept
{
	return m_component;
}
