#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/ComponentItem.h>
#include <Items/PowerItem.h>
#include <View/PropertiesList.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/std/Power.h>

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

PowerItem::PowerItem(CoreScene *scene, lcsm::model::Power *power, GUIOptions *options) :
	ComponentItem(scene, power, options), m_power(power), m_nameKey(0), m_facingKey(0), m_widthKey(0)
{
}

QRectF PowerItem::boundingRect() const
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

void PowerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	const qreal gs = static_cast< qreal >(options()->gridSize());

	// Draw triangle.
	{
		QPen pen;
		pen.setColor(Qt::GlobalColor::black);
		pen.setWidth(3);

		const QPointF offToRight = QPointF(gs, 0.0);
		const QPointF offToLeft = QPointF(-gs, 0.0);
		const QPointF offToUp = QPointF(0.0, -gs);
		const QPointF offToDown = QPointF(0.0, gs);

		const QPointF startPoint = relativePositionOfPort(lcsm::model::Power::Port::Wiring);
		QPainterPath path(startPoint);

		switch (m_direction)
		{
		case Item::ItemDirection::East:
		{
			const QPointF startTriangle = startPoint + offToLeft;
			path.lineTo(startTriangle);
			const QPointF triangle1 = startTriangle + offToUp;
			path.lineTo(triangle1);
			const QPointF triangle2 = triangle1 + offToLeft + offToDown;
			path.lineTo(triangle2);
			const QPointF triangle3 = triangle2 + offToRight + offToDown;
			path.lineTo(triangle3);
			path.lineTo(startTriangle);
			break;
		}
		case Item::ItemDirection::South:
		{
			const QPointF startTriangle = startPoint + offToUp;
			path.lineTo(startTriangle);
			const QPointF triangle1 = startTriangle + offToRight;
			path.lineTo(triangle1);
			const QPointF triangle2 = triangle1 + offToLeft + offToUp;
			path.lineTo(triangle2);
			const QPointF triangle3 = triangle2 + offToLeft + offToDown;
			path.lineTo(triangle3);
			path.lineTo(startTriangle);
			break;
		}
		case Item::ItemDirection::West:
		{
			const QPointF startTriangle = startPoint + offToRight;
			path.lineTo(startTriangle);
			const QPointF triangle1 = startTriangle + offToDown;
			path.lineTo(triangle1);
			const QPointF triangle2 = triangle1 + offToRight + offToUp;
			path.lineTo(triangle2);
			const QPointF triangle3 = triangle2 + offToLeft + offToUp;
			path.lineTo(triangle3);
			path.lineTo(startTriangle);
			break;
		}
		case Item::ItemDirection::North:
		{
			const QPointF startTriangle = startPoint + offToDown;
			path.lineTo(startTriangle);
			const QPointF triangle1 = startTriangle + offToLeft;
			path.lineTo(triangle1);
			const QPointF triangle2 = triangle1 + offToDown + offToRight;
			path.lineTo(triangle2);
			const QPointF triangle3 = triangle2 + offToUp + offToRight;
			path.lineTo(triangle3);
			path.lineTo(startTriangle);
			break;
		}
		}

		painter->setPen(pen);
		painter->drawPath(path);
	}

	// Draw port.
	{
		const qreal r = 2.0 * gs * 0.1;
		const QPointF p = relativePositionOfPort(lcsm::model::Power::Wiring);
		painter->setBrush(Qt::black);
		painter->drawEllipse(p, r, r);
	}
}

QPointF PowerItem::relativePositionOfPort(lcsm::portid_t portId) const
{
	if (portId != lcsm::model::Power::Port::Wiring)
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

void PowerItem::setProperty(int key, const QVariant &value)
{
	// Name
	if (key == m_nameKey)
	{
		const QString s = value.toString();
		m_power->setName(s.toStdString());
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
		m_power->setWidth(static_cast< lcsm::Width >(width));
	}
}

void PowerItem::setPropertiesList(PropertiesList *propertiesList)
{
	// Name
	{
		const QString name = QString::fromStdString(m_power->name());
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
		m_widthKey = propertiesList->addChoicesItem(QObject::tr("Width"), stringChoices, valueChoices, m_power->width());
	}
}

void PowerItem::connect()
{
	m_coreScene->connection(id(), lcsm::model::Power::Port::Wiring);
}

bool PowerItem::rotateActionEnabled() const noexcept
{
	return true;
}

bool PowerItem::putValueActionEnabled() const noexcept
{
	return false;
}

lcsm::width_t PowerItem::inputWidth() const noexcept
{
	return lcsm::Width::LastWidth;
}
