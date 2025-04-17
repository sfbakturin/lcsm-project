#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/ClockItem.h>
#include <Items/ComponentItem.h>
#include <View/PropertiesList.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/std/Clock.h>

#include <QPainter>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

ClockItem::ClockItem(CoreScene *scene, lcsm::model::Clock *clock, GUIOptions *options) :
	ComponentItem(scene, clock, options), m_clock(clock), m_nameKey(0), m_facingKey(0), m_highDurationKey(0),
	m_lowDurationKey(0), m_phaseOffsetKey(0)
{
}

QRectF ClockItem::boundingRect() const
{
	const qreal gs = static_cast< qreal >(options()->gridSize());
	const qreal nw = 3 * gs;
	const qreal nh = 2 * gs;
	switch (m_direction)
	{
	case Item::ItemDirection::East:
	case Item::ItemDirection::West:
	{
		return QRectF(0.0, 0.0, nw, nh);
	}
	case Item::ItemDirection::South:
	case Item::ItemDirection::North:
	{
		return QRectF(0.0, 0.0, nh, nw);
	}
	}
	return QRectF();
}

void ClockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	const QPen defaultPen = painter->pen();
	const QBrush defaultBrush = painter->brush();
	const qreal gs = static_cast< qreal >(options()->gridSize());
	QPointF center;

	// Draw square.
	{
		QPen pen;
		pen.setColor(Qt::GlobalColor::black);
		pen.setWidth(3);

		QPointF startPoint = QPointF(0.0, 0.0);
		QPointF endPoint = QPointF(0.0, 0.0);

		switch (m_direction)
		{
		case Item::ItemDirection::East:
		case Item::ItemDirection::South:
		{
			endPoint = QPointF(2 * gs, 2 * gs);
			break;
		}
		case Item::ItemDirection::West:
		{
			startPoint = QPointF(gs, 0.0);
			endPoint = QPointF(3 * gs, 2 * gs);
			break;
		}
		case Item::ItemDirection::North:
		{
			startPoint = QPointF(0.0, gs);
			endPoint = QPointF(2 * gs, 3 * gs);
			break;
		}
		}

		QPainterPath path(startPoint);
		const QRectF rect(startPoint, endPoint);
		path.addRect(rect);

		painter->setBrush(Qt::white);
		painter->setPen(pen);
		painter->drawPath(path);

		center = rect.center();
	}

	// Draw symbol.
	{
		Qt::GlobalColor color = Qt::GlobalColor::black;
		bool isTrueBit = false;
		if (simulate())
		{
			const lcsm::DataBits &databits = m_state->valueOf(id());
			isTrueBit = databits.bit(0) == lcsm::verilog::Bit::True;
			if (isTrueBit)
			{
				color = Qt::GlobalColor::green;
			}
			else if (databits.bit(0) != lcsm::verilog::Bit::Undefined)
			{
				color = Qt::GlobalColor::darkGreen;
			}
			else
			{
				isTrueBit = false;
			}
		}

		QPen pen;
		pen.setColor(color);
		pen.setWidth(3);

		const QPointF offToRight = QPointF(gs, 0.0);
		const QPointF offToLeft = QPointF(-gs, 0.0);
		const QPointF offToUp = QPointF(0.0, -gs);
		const QPointF offToDown = QPointF(0.0, gs);

		QPainterPath path;

		QPointF p0;
		QPointF p1;
		QPointF p2;
		QPointF p3;
		QPointF p4;
		QPointF p5;

		if (isTrueBit)
		{
			p0 = center + (offToLeft * 0.5) + (offToDown * 0.5);
			p1 = p0 + offToUp;
			p2 = p1 + (offToRight * 0.5);
			p3 = p2 + offToDown;
			p4 = p3 + (offToRight * 0.5);
			p5 = p4 + offToUp;
		}
		else
		{
			p0 = center + (offToLeft * 0.5) + (offToUp * 0.5);
			p1 = p0 + offToDown;
			p2 = p1 + (offToRight * 0.5);
			p3 = p2 + offToUp;
			p4 = p3 + (offToRight * 0.5);
			p5 = p4 + offToDown;
		}

		path.moveTo(p0);
		path.lineTo(p1);
		path.lineTo(p2);
		path.lineTo(p3);
		path.lineTo(p4);
		path.lineTo(p5);

		painter->setPen(pen);
		painter->drawPath(path);
	}

	// Draw port.
	{
		const qreal r = 2.0 * gs * 0.1;
		const QPointF p = relativePositionOfPort(lcsm::model::Ground::Wiring);
		painter->setPen(defaultPen);
		painter->setBrush(Qt::black);
		painter->drawEllipse(p, r, r);
	}
}

QPointF ClockItem::relativePositionOfPort(lcsm::portid_t portId) const
{
	if (portId != lcsm::model::Clock::Port::Wiring)
	{
		throw std::logic_error("Bad port!");
	}
	const qreal gs = static_cast< qreal >(options()->gridSize());
	switch (m_direction)
	{
	case Item::ItemDirection::East:
	{
		return QPointF(2 * gs, gs);
	}
	case Item::ItemDirection::South:
	{
		return QPointF(gs, 2 * gs);
	}
	case Item::ItemDirection::West:
	{
		return QPointF(gs, gs);
	}
	case Item::ItemDirection::North:
	{
		return QPointF(gs, gs);
	}
	}
	return QPointF();
}

void ClockItem::setProperty(int key, const QVariant &value)
{
	// Name
	if (key == m_nameKey)
	{
		const QString s = value.toString();
		m_clock->setName(s.toStdString());
	}
	// Facing
	else if (key == m_facingKey)
	{
		const uint direction = value.toUInt();
		m_direction = static_cast< Item::ItemDirection >(direction);
		directionChanged();
	}
	// High Duration (in ticks)
	else if (key == m_highDurationKey)
	{
		const unsigned highDuration = value.toUInt();
		m_clock->setHighDuration(highDuration);
	}
	// Low Duration (in ticks)
	else if (key == m_lowDurationKey)
	{
		const unsigned lowDuration = value.toUInt();
		m_clock->setLowDuration(lowDuration);
	}
	// Phase Offset (in ticks)
	else if (key == m_phaseOffsetKey)
	{
		const unsigned phaseOffset = value.toUInt();
		m_clock->setPhaseOffset(phaseOffset);
	}
}

void ClockItem::setPropertiesList(PropertiesList *propertiesList)
{
	// Name
	{
		const QString name = QString::fromStdString(m_clock->name());
		m_nameKey = propertiesList->addEditableItem(QObject::tr("Name"), name);
	}

	// Facing
	{
		const QList< QString > stringChoices = { QObject::tr("North"), QObject::tr("South"), QObject::tr("East"), QObject::tr("West") };
		const QList< QVariant > valueChoices = { Item::ItemDirection::North, Item::ItemDirection::South, Item::ItemDirection::East, Item::ItemDirection::West };
		m_facingKey = propertiesList->addChoicesItem(QObject::tr("Facing"), stringChoices, valueChoices, m_direction);
	}

	// High Duration (in ticks)
	{
		m_highDurationKey = propertiesList->addNumericalItem(QObject::tr("High Duration (in ticks)"), m_clock->highDuration(), 0);
	}

	// Low Duration (in ticks)
	{
		m_lowDurationKey = propertiesList->addNumericalItem(QObject::tr("Low Duration (in ticks)"), m_clock->lowDuration(), 0);
	}

	// Phase Offset (in ticks)
	{
		m_phaseOffsetKey = propertiesList->addNumericalItem(QObject::tr("Phase Offset (in ticks)"), m_clock->phaseOffset(), 0);
	}
}

void ClockItem::connect()
{
	m_coreScene->connection(id(), lcsm::model::Clock::Port::Wiring);
}

bool ClockItem::rotateActionEnabled() const noexcept
{
	return true;
}

bool ClockItem::putValueActionEnabled() const noexcept
{
	return false;
}

lcsm::width_t ClockItem::inputWidth() const noexcept
{
	return lcsm::Width::LastWidth;
}
