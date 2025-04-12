#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/ImmovableItem.h>
#include <Items/Item.h>
#include <lcsm/Model/Identifier.h>

#include <QGraphicsItem>

ImmovableItem::ImmovableItem(CoreScene *coreScene, lcsm::Identifier id, GUIOptions *options) :
	Item(coreScene, id, options)
{
	// Immovable item initial setup.
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::RightButton);
	setAcceptDrops(true);
	setZValue(0.0);
}
