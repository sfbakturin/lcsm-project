#include <Core/Scene.h>
#include <Items/Item.h>
#include <Items/PinItem.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/std/Pin.h>

#include <QHash>
#include <memory>

void Scene::addToScene(lcsm::model::Pin *pin)
{
	m_items[pin->id()] = std::make_unique< PinItem >(pin);
}
