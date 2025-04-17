#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/ComponentItem.h>
#include <Items/ConstantItem.h>
#include <View/PropertiesList.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/std/Constant.h>

#include <QFont>
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

ConstantItem::ConstantItem(CoreScene *scene, lcsm::model::Constant *constant, GUIOptions *options) :
	ComponentItem(scene, constant, options), m_constant(constant), m_nameKey(0), m_facingKey(0), m_widthKey(0), m_valueKey(0)
{
}

QRectF ConstantItem::boundingRect() const
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

void ConstantItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	const QFont painterFont = painter->font();
	const qreal gs = static_cast< qreal >(options()->gridSize());
	QRectF squareRect;

	// Draw square.
	{
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

		painter->setBrush(Qt::gray);
		painter->drawPath(path);

		squareRect = rect;
	}

	// Draw port.
	{
		const qreal r = 2.0 * gs * 0.1;
		const QPointF p = relativePositionOfPort(lcsm::model::Constant::Wiring);
		painter->setBrush(Qt::black);
		painter->drawEllipse(p, r, r);
	}

	// Draw text.
	{
		const QString text = "1";
		QPen pen;
		pen.setBrush(Qt::green);
		pen.setWidth(4);
		QFont font = painterFont;
		font.setPointSize(font.pointSize() * 4);
		painter->setPen(pen);
		painter->setFont(font);
		painter->drawText(squareRect, Qt::AlignCenter, text);
	}
}

QPointF ConstantItem::relativePositionOfPort(lcsm::portid_t portId) const
{
	if (portId != lcsm::model::Constant::Port::Wiring)
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

void ConstantItem::setProperty(int key, const QVariant &value)
{
	// Name
	if (key == m_nameKey)
	{
		const QString s = value.toString();
		m_constant->setName(s.toStdString());
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
		m_constant->setWidth(static_cast< lcsm::Width >(width));
	}
	// Value
	else if (key == m_valueKey)
	{
		const lcsm::value_t v = value.toUInt();
		m_constant->setValue(v);
	}
}

void ConstantItem::setPropertiesList(PropertiesList *propertiesList)
{
	// Name
	{
		const QString name = QString::fromStdString(m_constant->name());
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
		m_widthKey = propertiesList->addChoicesItem(QObject::tr("Width"), stringChoices, valueChoices, m_constant->width());
	}

	// Value
	{
		m_valueKey = propertiesList->addNumericalItem(QObject::tr("Value"), m_constant->value(), 0);
	}
}

void ConstantItem::connect()
{
	m_coreScene->connection(id(), lcsm::model::Constant::Port::Wiring);
}

bool ConstantItem::rotateActionEnabled() const noexcept
{
	return true;
}

bool ConstantItem::putValueActionEnabled() const noexcept
{
	return false;
}

lcsm::width_t ConstantItem::inputWidth() const noexcept
{
	return lcsm::Width::LastWidth;
}
