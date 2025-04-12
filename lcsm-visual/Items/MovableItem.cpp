#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/Item.h>
#include <Items/MovableItem.h>
#include <lcsm/Model/Identifier.h>

#include <QGraphicsItem>
#include <QPointF>
#include <QRectF>
#include <QVariant>

MovableItem::MovableItem(CoreScene *coreScene, lcsm::Identifier id, GUIOptions *options) : Item(coreScene, id, options)
{
	// Movable item initial setup.
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::RightButton | Qt::LeftButton);
	setAcceptDrops(true);
	setZValue(1.0);
}

static inline qreal clamp(qreal vl, qreal lo, qreal hi) noexcept
{
	return (vl < lo) ? lo : ((hi < vl) ? hi : vl);
}

QVariant MovableItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == GraphicsItemChange::ItemPositionChange)
	{
		const QPointF pos = value.toPointF();
		const qreal gs = static_cast< qreal >(options()->gridSize());

		const qreal x = qRound(pos.x() / gs) * gs;
		const qreal y = qRound(pos.y() / gs) * gs;

		const QRectF bounds = scene()->sceneRect();

		const qreal nx = clamp(x, bounds.left(), bounds.right() - boundingRect().width());
		const qreal ny = clamp(y, bounds.top(), bounds.bottom() - boundingRect().height());

		return QPointF(nx, ny);
	}
	return QGraphicsItem::itemChange(change, value);
}
