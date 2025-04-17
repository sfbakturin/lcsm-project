#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/ComponentItem.h>
#include <Items/TransistorItem.h>
#include <View/DesignDialog.h>
#include <View/PropertiesList.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/std/Transistor.h>

#include <QBrush>
#include <QColor>
#include <QList>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QString>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

TransistorItem::TransistorItem(CoreScene *scene, lcsm::model::Transistor *transistor, GUIOptions *options) :
	ComponentItem(scene, transistor, options), m_transistor(transistor), m_nameKey(0), m_facingKey(0), m_typeKey(0)
{
	m_portStrings << QObject::tr("Base");
	m_portStrings << QObject::tr("Source A");
	m_portStrings << QObject::tr("Source B");
}

QRectF TransistorItem::boundingRect() const
{
	const qreal gs = static_cast< qreal >(options()->gridSize());
	const qreal nw = 4 * gs;
	const qreal nh = 3 * gs;
	switch (m_direction)
	{
	case Item::ItemDirection::West:
	case Item::ItemDirection::East:
	{
		return QRectF(0.0, 0.0, nh, nw);
	}
	case Item::ItemDirection::South:
	case Item::ItemDirection::North:
	{
		return QRectF(0.0, 0.0, nw, nh);
	}
	}
	return QRectF();
}

void TransistorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	const qreal gs = static_cast< qreal >(options()->gridSize());
	const QPointF offToRight = QPointF(gs, 0.0);
	const QPointF offToLeft = QPointF(-gs, 0.0);
	const QPointF offToUp = QPointF(0.0, -gs);
	const QPointF offToDown = QPointF(0.0, gs);

	// Draw sources.
	{
		QPen pen;
		pen.setColor(Qt::GlobalColor::black);
		pen.setWidth(3);

		const QPointF startPoint = relativePositionOfPort(lcsm::model::Transistor::Port::SrcA);
		QPainterPath path(startPoint);

		QPointF p1;
		QPointF p2;
		QPointF p3;
		QPointF p4;
		QPointF p5;

		switch (m_direction)
		{
		case Item::ItemDirection::East:
		{
			p1 = startPoint + offToDown;
			p2 = p1 + offToRight;
			p3 = p2 + offToDown + offToDown;
			p4 = p3 + offToLeft;
			p5 = p4 + offToDown;
			break;
		}
		case Item::ItemDirection::South:
		{
			p1 = startPoint + offToLeft;
			p2 = p1 + offToDown;
			p3 = p2 + offToLeft + offToLeft;
			p4 = p3 + offToUp;
			p5 = p4 + offToLeft;
			break;
		}
		case Item::ItemDirection::West:
		{
			p1 = startPoint + offToUp;
			p2 = p1 + offToLeft;
			p3 = p2 + offToUp + offToUp;
			p4 = p3 + offToRight;
			p5 = p4 + offToUp;
			break;
		}
		case Item::ItemDirection::North:
		{
			p1 = startPoint + offToRight;
			p2 = p1 + offToUp;
			p3 = p2 + offToRight + offToRight;
			p4 = p3 + offToDown;
			p5 = p4 + offToRight;
			break;
		}
		}

		path.lineTo(p1);
		path.lineTo(p2);
		path.lineTo(p3);
		path.lineTo(p4);
		path.lineTo(p5);

		painter->setPen(pen);
		painter->drawPath(path);
	}

	// Draw base.
	{
		QPen pen;
		pen.setColor(Qt::GlobalColor::black);
		pen.setWidth(3);

		const QPointF p0 = relativePositionOfPort(lcsm::model::Transistor::Port::Base);

		QPointF p1;
		QPointF pl;
		QPointF pr;

		switch (m_direction)
		{
		case Item::ItemDirection::East:
		{
			p1 = p0 + offToLeft;
			pl = p1 + offToUp;
			pr = p1 + offToDown;
			break;
		}
		case Item::ItemDirection::South:
		{
			p1 = p0 + offToUp;
			pl = p1 + offToRight;
			pr = p1 + offToLeft;
			break;
		}
		case Item::ItemDirection::West:
		{
			p1 = p0 + offToRight;
			pl = p1 + offToUp;
			pr = p1 + offToDown;
			break;
		}
		case Item::ItemDirection::North:
		{
			p1 = p0 + offToDown;
			pl = p1 + offToRight;
			pr = p1 + offToLeft;
			break;
		}
		}

		painter->setPen(pen);
		painter->drawLine(p0, p1);
		painter->drawLine(pl, pr);

		if (m_transistor->type() == lcsm::model::Transistor::Type::P)
		{
			const qreal r = 2.0 * gs * 0.1;
			painter->setBrush(Qt::black);
			painter->drawEllipse(p1, r, r);
		}
	}
}

QPointF TransistorItem::relativePositionOfPort(lcsm::portid_t p) const
{
	const lcsm::model::Transistor::Port portId = static_cast< lcsm::model::Transistor::Port >(p);
	const qreal gs = static_cast< qreal >(options()->gridSize());
	switch (portId)
	{
	case lcsm::model::Transistor::Port::Base:
	{
		switch (m_direction)
		{
		case Item::ItemDirection::East:
		{
			return QPointF(3 * gs, 2 * gs);
		}
		case Item::ItemDirection::South:
		{
			return QPointF(2 * gs, 3 * gs);
		}
		case Item::ItemDirection::West:
		{
			return QPointF(0.0, 2 * gs);
		}
		case Item::ItemDirection::North:
		{
			return QPointF(2 * gs, 0.0);
		}
		default:
		{
			break;
		}
		}
		break;
	}
	case lcsm::model::Transistor::Port::SrcA:
		switch (m_direction)
		{
		case Item::ItemDirection::East:
		{
			return QPointF(0.0, 0.0);
		}
		case Item::ItemDirection::South:
		{
			return QPointF(4 * gs, 0.0);
		}
		case Item::ItemDirection::West:
		{
			return QPointF(3 * gs, 4 * gs);
		}
		case Item::ItemDirection::North:
		{
			return QPointF(0.0, 3 * gs);
		}
		default:
		{
			break;
		}
		}
		break;
		{
		}
	case lcsm::model::Transistor::Port::SrcB:
		switch (m_direction)
		{
		case Item::ItemDirection::East:
		{
			return QPointF(0.0, 4 * gs);
		}
		case Item::ItemDirection::South:
		{
			return QPointF(0.0, 0.0);
		}
		case Item::ItemDirection::West:
		{
			return QPointF(3 * gs, 0.0);
		}
		case Item::ItemDirection::North:
		{
			return QPointF(4 * gs, 3 * gs);
		}
		default:
		{
			break;
		}
		}
		break;
		{
		}
	default:
	{
		break;
	}
	}
	throw std::logic_error("Bad port!");
}

void TransistorItem::setProperty(int key, const QVariant &value)
{
	// Name
	if (key == m_nameKey)
	{
		const QString s = value.toString();
		m_transistor->setName(s.toStdString());
	}
	// Facing
	else if (key == m_facingKey)
	{
		const uint direction = value.toUInt();
		m_direction = static_cast< Item::ItemDirection >(direction);
		directionChanged();
	}
	// Type
	else if (key == m_typeKey)
	{
		const uint type = value.toUInt();
		m_transistor->setType(static_cast< lcsm::model::Transistor::Type >(type));
		update();
	}
}

void TransistorItem::setPropertiesList(PropertiesList *propertiesList)
{
	// Name
	{
		const QString name = QString::fromStdString(m_transistor->name());
		m_nameKey = propertiesList->addEditableItem(QObject::tr("Name"), name);
	}

	// Facing
	{
		const QList< QString > stringChoices = { QObject::tr("North"), QObject::tr("South"), QObject::tr("East"), QObject::tr("West") };
		const QList< QVariant > valueChoices = { Item::ItemDirection::North, Item::ItemDirection::South, Item::ItemDirection::East, Item::ItemDirection::West };
		m_facingKey = propertiesList->addChoicesItem(QObject::tr("Facing"), stringChoices, valueChoices, m_direction);
	}

	// Type
	{
		const QList< QString > stringChoices = { QObject::tr("P-Type"), QObject::tr("N-Type") };
		const QList< QVariant > valueChoices = { lcsm::model::Transistor::Type::P, lcsm::model::Transistor::Type::N };
		m_typeKey = propertiesList->addChoicesItem(QObject::tr("Type"), stringChoices, valueChoices, m_transistor->type());
	}
}

void TransistorItem::connect()
{
	const int index = DesignDialog::Connect(m_portStrings);
	if (index < 0)
	{
		return;
	}
	m_coreScene->connection(id(), static_cast< lcsm::portid_t >(index));
}

bool TransistorItem::rotateActionEnabled() const noexcept
{
	return true;
}

bool TransistorItem::putValueActionEnabled() const noexcept
{
	return false;
}

lcsm::width_t TransistorItem::inputWidth() const noexcept
{
	return lcsm::Width::LastWidth;
}
