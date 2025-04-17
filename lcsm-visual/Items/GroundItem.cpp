#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/ComponentItem.h>
#include <Items/GroundItem.h>
#include <View/PropertiesList.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/std/Ground.h>

#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>
#include <stdexcept>

GroundItem::GroundItem(CoreScene *scene, lcsm::model::Ground *ground, GUIOptions *options) :
	ComponentItem(scene, ground, options), m_ground(ground), m_nameKey(0), m_facingKey(0), m_widthKey(0)
{
}

QRectF GroundItem::boundingRect() const
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

void GroundItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	const qreal gs = static_cast< qreal >(options()->gridSize());

	// Draw lines.
	{
		QPen pen;
		pen.setColor(Qt::GlobalColor::black);
		pen.setWidth(3);

		const QPointF offToRight = QPointF(gs, 0.0);
		const QPointF offToLeft = QPointF(-gs, 0.0);
		const QPointF offToUp = QPointF(0.0, -gs);
		const QPointF offToDown = QPointF(0.0, gs);

		const QPointF startPoint = relativePositionOfPort(lcsm::model::Ground::Port::Wiring);
		painter->setPen(pen);

		switch (m_direction)
		{
		case Item::ItemDirection::East:
		{
			// Line from port to of lines.
			const QPointF startLines = startPoint + offToLeft;
			painter->drawLine(startPoint, startLines);

			// Biggest line.
			const QPointF bigStart = startLines + offToUp;
			const QPointF bigEnd = startLines + offToDown;
			painter->drawLine(bigStart, bigEnd);

			// Medium line.
			const QPointF medStart = bigStart + (offToLeft * 0.5) + (offToDown * 0.25);
			const QPointF medEnd = bigEnd + (offToLeft * 0.5) + (offToUp * 0.25);
			painter->drawLine(medStart, medEnd);

			// Small line.
			const QPointF smallStart = bigStart + offToLeft + (offToDown * 0.5);
			const QPointF smallEnd = bigEnd + offToLeft + (offToUp * 0.5);
			painter->drawLine(smallStart, smallEnd);

			break;
		}
		case Item::ItemDirection::South:
		{
			// Line from port to of lines.
			const QPointF startLines = startPoint + offToUp;
			painter->drawLine(startPoint, startLines);

			// Biggest line.
			const QPointF bigStart = startLines + offToRight;
			const QPointF bigEnd = startLines + offToLeft;
			painter->drawLine(bigStart, bigEnd);

			// Medium line.
			const QPointF medStart = bigStart + (offToUp * 0.5) + (offToLeft * 0.25);
			const QPointF medEnd = bigEnd + (offToUp * 0.5) + (offToRight * 0.25);
			painter->drawLine(medStart, medEnd);

			// Small line.
			const QPointF smallStart = bigStart + offToUp + (offToLeft * 0.5);
			const QPointF smallEnd = bigEnd + offToUp + (offToRight * 0.5);
			painter->drawLine(smallStart, smallEnd);

			break;
		}
		case Item::ItemDirection::West:
		{
			// Line from port to of lines.
			const QPointF startLines = startPoint + offToRight;
			painter->drawLine(startPoint, startLines);

			// Biggest line.
			const QPointF bigStart = startLines + offToUp;
			const QPointF bigEnd = startLines + offToDown;
			painter->drawLine(bigStart, bigEnd);

			// Medium line.
			const QPointF medStart = bigStart + (offToRight * 0.5) + (offToDown * 0.25);
			const QPointF medEnd = bigEnd + (offToRight * 0.5) + (offToUp * 0.25);
			painter->drawLine(medStart, medEnd);

			// Small line.
			const QPointF smallStart = bigStart + offToRight + (offToDown * 0.5);
			const QPointF smallEnd = bigEnd + offToRight + (offToUp * 0.5);
			painter->drawLine(smallStart, smallEnd);

			break;
		}
		case Item::ItemDirection::North:
		{
			// Line from port to of lines.
			const QPointF startLines = startPoint + offToDown;
			painter->drawLine(startPoint, startLines);

			// Biggest line.
			const QPointF bigStart = startLines + offToLeft;
			const QPointF bigEnd = startLines + offToRight;
			painter->drawLine(bigStart, bigEnd);

			// Medium line.
			const QPointF medStart = bigStart + (offToDown * 0.5) + (offToRight * 0.25);
			const QPointF medEnd = bigEnd + (offToDown * 0.5) + (offToLeft * 0.25);
			painter->drawLine(medStart, medEnd);

			// Small line.
			const QPointF smallStart = bigStart + offToDown + (offToRight * 0.5);
			const QPointF smallEnd = bigEnd + offToDown + (offToLeft * 0.5);
			painter->drawLine(smallStart, smallEnd);

			break;
		}
		}
	}

	// Draw port.
	{
		const qreal r = 2.0 * gs * 0.1;
		const QPointF p = relativePositionOfPort(lcsm::model::Ground::Wiring);
		painter->setBrush(Qt::black);
		painter->drawEllipse(p, r, r);
	}
}

QPointF GroundItem::relativePositionOfPort(lcsm::portid_t portId) const
{
	if (portId != lcsm::model::Ground::Port::Wiring)
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

void GroundItem::setProperty(int key, const QVariant &value)
{
	// Name
	if (key == m_nameKey)
	{
		const QString s = value.toString();
		m_ground->setName(s.toStdString());
	}
	// Facing
	else if (key == m_facingKey)
	{
		const uint direction = value.toUInt();
		m_direction = static_cast< Item::ItemDirection >(direction);
		directionChanged();
	}
	// Width
	else if (key == m_widthKey)
	{
		const uint width = value.toUInt();
		m_ground->setWidth(static_cast< lcsm::Width >(width));
	}
}

void GroundItem::setPropertiesList(PropertiesList *propertiesList)
{
	// Name
	{
		const QString name = QString::fromStdString(m_ground->name());
		m_nameKey = propertiesList->addEditableItem(QObject::tr("Name"), name);
	}

	// Facing
	{
		const QList< QString > stringChoices = { QObject::tr("North"), QObject::tr("South"), QObject::tr("East"), QObject::tr("West") };
		const QList< QVariant > valueChoices = { Item::ItemDirection::North, Item::ItemDirection::South, Item::ItemDirection::East, Item::ItemDirection::West };
		m_facingKey = propertiesList->addChoicesItem(QObject::tr("Facing"), stringChoices, valueChoices, m_direction);
	}

	// Width
	{
		QList< QString > stringChoices;
		QList< QVariant > valueChoices;
		for (lcsm::Width w : lcsm::Widths)
		{
			const QString s = lcsm::WidthToCStr(w);
			const QVariant v = w;
			stringChoices << s;
			valueChoices << v;
		}
		m_widthKey = propertiesList->addChoicesItem(QObject::tr("Width"), stringChoices, valueChoices, m_ground->width());
	}
}

void GroundItem::connect()
{
	m_coreScene->connection(id(), lcsm::model::Ground::Port::Wiring);
}

bool GroundItem::rotateActionEnabled() const noexcept
{
	return true;
}

bool GroundItem::putValueActionEnabled() const noexcept
{
	return false;
}

lcsm::width_t GroundItem::inputWidth() const noexcept
{
	return lcsm::Width::LastWidth;
}
