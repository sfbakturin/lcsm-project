#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/ComponentItem.h>
#include <Items/MovableItem.h>
#include <Items/PinItem.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/std/Pin.h>

#include <QDebug>
#include <QObject>
#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

PinItem::PinItem(CoreScene *scene, lcsm::model::Pin *pin, GUIOptions *options) :
	ComponentItem(scene, pin, options), m_pin(pin)
{
}

QRectF PinItem::boundingRect() const
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	const qreal width = static_cast< qreal >(2 * gridSize);
	const qreal extend = static_cast< qreal >(3 * gridSize);
	switch (m_direction)
	{
	case Item::ItemDirection::East:
	case Item::ItemDirection::West:
	{
		return QRectF(0, 0, extend, width);
	}
	case Item::ItemDirection::South:
	case Item::ItemDirection::North:
	{
		return QRectF(0, 0, width, extend);
	}
	}
	return QRectF();
}

void PinItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	const qreal width = static_cast< qreal >(2 * gridSize);

	const QPointF offset = offsetBoundingRect();
	const QPointF rpos = relativePositionOfPort() + offset;
	const qreal r = static_cast< qreal >(width) * 0.2;

	const qreal bound = static_cast< qreal >(width) * 0.1;
	const qreal boundWidth = width - bound;

	const QString tin = QObject::tr("Input");
	const QString tout = QObject::tr("Output");

	const QBrush selected = (isSelected() ? Qt::red : (aboutToBeConnected() ? Qt::blue : Qt::black));

	// Draw bounds 1.
	{
		painter->setBrush(selected);
		const QPointF p1 = offset;
		const QPointF p2 = QPointF(width, width) + offset;
		painter->drawRect(QRectF(p1, p2));
	}

	// Draw bounds 2.
	{
		painter->setBrush(Qt::white);
		const QPointF p1 = QPointF(bound, bound) + offset;
		const QPointF p2 = QPointF(boundWidth, boundWidth) + offset;
		painter->drawRect(QRectF(p1, p2));
	}

	// Draw port.
	{
		painter->setBrush(selected);
		painter->drawEllipse(rpos, r, r);
	}

	// Draw text.
	{
		painter->setBrush(Qt::green);
		const QPointF p1 = QPointF(bound, bound) + offset;
		const QPointF p2 = QPointF(boundWidth, boundWidth) + offset;
		const QRectF rr(p1, p2);
		painter->drawText(
			rr,
			Qt::AlignCenter,
			(m_pin->output() ? tout : tin) + '\n' + QString("%1 bits").arg(static_cast< int >(m_pin->width())));
	}
}

QPointF PinItem::relativePositionOfPort(lcsm::portid_t portId) const
{
	const lcsm::model::Pin::Port p = static_cast< lcsm::model::Pin::Port >(portId);

	switch (p)
	{
	case lcsm::model::Pin::Port::Internal:
	{
		const QPointF rel = relativePositionOfPort();
		const QPointF off = offsetBoundingRect();
		const QPointF p = rel + off;
		return QPointF(p.x(), p.y());
	}
	default:
	{
		break;
	}
	}

	throw std::logic_error("Bad port!");
}

void PinItem::connect()
{
	m_coreScene->connection(id(), lcsm::model::Pin::Port::Internal);
}

QPointF PinItem::relativePositionOfPort() const noexcept
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	const qreal width = static_cast< qreal >(2 * gridSize);
	switch (m_direction)
	{
	case Item::ItemDirection::East:
	{
		return QPointF(width, gridSize);
	}
	case Item::ItemDirection::South:
	{
		return QPointF(gridSize, width);
	}
	case Item::ItemDirection::West:
	{
		return QPointF(0.0, gridSize);
	}
	case Item::ItemDirection::North:
	{
		return QPointF(gridSize, 0.0);
	}
	}
	return QPoint();
}

QPointF PinItem::offsetBoundingRect() const noexcept
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	switch (m_direction)
	{
	case Item::ItemDirection::East:
	case Item::ItemDirection::South:
	{
		return QPointF();
	}
	case Item::ItemDirection::West:
	{
		return QPointF(gridSize, 0.0);
	}
	case Item::ItemDirection::North:
	{
		return QPointF(0.0, gridSize);
	}
	default:
	{
		break;
	}
	}
	return QPointF();
}
