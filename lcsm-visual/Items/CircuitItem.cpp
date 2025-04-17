#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/CircuitItem.h>
#include <Items/Item.h>
#include <View/DesignDialog.h>
#include <View/PropertiesList.h>
#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/std/Pin.h>

#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPointF>
#include <QRectF>
#include <QString>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>
#include <memory>
#include <string>
#include <utility>

static constexpr unsigned LOGISIM_EVOLUTION_CIRCUIT_SQUARE_WIDTH = 10;
static constexpr unsigned LOGISIM_EVOLUTION_CIRCUIT_SQUARE_HEIGHT_FOR_TITLE = 3;

CircuitItem::CircuitItem(CoreScene *scene, const lcsm::LCSMCircuitView &view, GUIOptions *options) :
	MovableItem(scene, view.id(), options), m_view(view), m_facingKey(0)
{
	int noname = 0;
	const QString inpString = QObject::tr("Input") + ": ";
	for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::Component > > &item : m_view.inputs())
	{
		const lcsm::Component *component = item.second.get();
		const std::string &name = component->name();
		QString realName = inpString;
		if (name.empty())
		{
			realName += '#' + QString::number(noname);
			noname++;
		}
		else
		{
			const QString compName = QString::fromStdString(name);
			realName += compName;
		}
		m_portStrings << realName;
		m_portInputIds << component->id();
	}

	noname = 0;
	const QString outString = QObject::tr("Output") + ": ";
	for (const std::pair< const lcsm::Identifier, std::shared_ptr< lcsm::Component > > &item : m_view.outputs())
	{
		const lcsm::Component *component = item.second.get();
		const std::string &name = component->name();
		QString realName = outString;
		if (name.empty())
		{
			realName += '#' + QString::number(noname);
			noname++;
		}
		else
		{
			const QString compName = QString::fromStdString(name);
			realName += compName;
		}
		m_portStrings << realName;
		m_portOutputIds << component->id();
	}
}

Item::ItemType CircuitItem::ty() const noexcept
{
	return Item::ItemType::CircuitItemTy;
}

void CircuitItem::reid() noexcept
{
	m_id = m_view.id();
}

QRectF CircuitItem::boundingRect() const
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	const qreal w = circuitWidth();
	const qreal h = circuitHeight();
	const qreal additionBoundRectForPortShow = 2 * gridSize;
	switch (m_direction)
	{
	case Item::ItemDirection::East:
	case Item::ItemDirection::West:
	{
		return QRectF(0.0, 0.0, w + additionBoundRectForPortShow * 2, h);
	}
	case Item::ItemDirection::South:
	case Item::ItemDirection::North:
	{
		return QRectF(0.0, 0.0, h, w + additionBoundRectForPortShow * 2);
	}
	}
	return QRectF();
}

void CircuitItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	// Constants.
	const int gsi = options()->gridSize();
	const qreal gs = static_cast< qreal >(gsi);
	const QPen defaultPen = painter->pen();

	// Draw main square.
	{
		QPen pen;
		pen.setBrush(Qt::black);
		pen.setWidth(3);
		painter->setPen(pen);

		const qreal offset = gs * 2.0;

		switch (m_direction)
		{
		case Item::ItemDirection::East:
		case Item::ItemDirection::West:
		{
			const QPointF startPoint = QPointF(offset, 0.0);

			QPainterPath path(startPoint);
			const QRectF rect(startPoint.x(), startPoint.y(), circuitWidth(), circuitHeight());
			path.addRect(rect);

			painter->drawPath(path);

			break;
		}
		case Item::ItemDirection::South:
		case Item::ItemDirection::North:
		{
			const QPointF startPoint = QPointF(0.0, offset);

			QPainterPath path(startPoint);
			const QRectF rect(startPoint.x(), startPoint.y(), circuitHeight(), circuitWidth());
			path.addRect(rect);

			painter->drawPath(path);

			break;
		}
		}
	}

	// Draw ports.
	{
		painter->setPen(defaultPen);
		painter->setBrush(Qt::black);

		const qreal r = 2.0 * gs * 0.1;
		int index = 0;

		for (int p = 0; p < m_portInputIds.size(); p++)
		{
			const lcsm::portid_t portId = static_cast< lcsm::portid_t >(index++);
			const QPointF rpos = relativePositionOfPort(portId);
			painter->drawEllipse(rpos, r, r);
			switch (m_direction)
			{
			case Item::ItemDirection::East:
			{
				const QPointF endline = rpos + QPointF(gs, 0.0);
				painter->drawLine(rpos, endline);
				break;
			}
			case Item::ItemDirection::South:
			{
				const QPointF endline = rpos + QPointF(0.0, gs);
				painter->drawLine(rpos, endline);
				break;
			}
			case Item::ItemDirection::West:
			{
				const QPointF endline = rpos + QPointF(-gs, 0.0);
				painter->drawLine(rpos, endline);
				break;
			}
			case Item::ItemDirection::North:
			{
				const QPointF endline = rpos + QPointF(0.0, -gs);
				painter->drawLine(rpos, endline);
				break;
			}
			}
		}

		for (int p = 0; p < m_portOutputIds.size(); p++)
		{
			const lcsm::portid_t portId = static_cast< lcsm::portid_t >(index++);
			const QPointF rpos = relativePositionOfPort(portId);
			painter->drawEllipse(rpos, r, r);
			switch (m_direction)
			{
			case Item::ItemDirection::East:
			{
				const QPointF endline = rpos + QPointF(-gs, 0.0);
				painter->drawLine(rpos, endline);
				break;
			}
			case Item::ItemDirection::South:
			{
				const QPointF endline = rpos + QPointF(0.0, -gs);
				painter->drawLine(rpos, endline);
				break;
			}
			case Item::ItemDirection::West:
			{
				const QPointF endline = rpos + QPointF(gs, 0.0);
				painter->drawLine(rpos, endline);
				break;
			}
			case Item::ItemDirection::North:
			{
				const QPointF endline = rpos + QPointF(0.0, gs);
				painter->drawLine(rpos, endline);
				break;
			}
			}
		}
	}
}

QPointF CircuitItem::relativePositionOfPort(lcsm::portid_t portId) const
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	bool isOutput = false;
	int index = portId;
	if (index >= m_portInputIds.size())
	{
		isOutput = true;
		index -= m_portInputIds.size();
	}
	qreal xpos = 0.0;
	qreal ypos = 0.0;
	switch (m_direction)
	{
	case Item::ItemDirection::West:
	{
		xpos = gridSize;
		if (!isOutput)
		{
			xpos += gridSize + circuitWidth() + gridSize;
		}
		ypos = circuitHeight() - gridSize;
		if (index != 0)
		{
			ypos -= index * 2 * gridSize;
		}
		break;
	}
	case Item::ItemDirection::East:
	{
		xpos = gridSize;
		if (isOutput)
		{
			xpos += gridSize + circuitWidth() + gridSize;
		}
		ypos = gridSize;
		if (index != 0)
		{
			ypos += index * 2 * gridSize;
		}
		break;
	}
	case Item::ItemDirection::South:
	{
		ypos = gridSize;
		if (isOutput)
		{
			ypos += gridSize + circuitWidth() + gridSize;
		}
		xpos = circuitHeight() - gridSize;
		if (index != 0)
		{
			xpos -= index * 2 * gridSize;
		}
		break;
	}
	case Item::ItemDirection::North:
	{
		ypos = gridSize;
		if (!isOutput)
		{
			ypos += gridSize + circuitWidth() + gridSize;
		}
		xpos = gridSize;
		if (index != 0)
		{
			xpos += index * 2 * gridSize;
		}
		break;
	}
	}
	return QPointF(xpos, ypos);
}

void CircuitItem::setProperty(int key, const QVariant &value)
{
	// Facing
	if (key == m_facingKey)
	{
		const uint direction = value.toUInt();
		m_direction = static_cast< Item::ItemDirection >(direction);
		directionChanged();
	}
}

void CircuitItem::setPropertiesList(PropertiesList *propertiesList)
{
	// Facing.
	{
		const QList< QString > stringChoices = { QObject::tr("North"), QObject::tr("South"), QObject::tr("East"), QObject::tr("West") };
		const QList< QVariant > valueChoices = { Item::ItemDirection::North, Item::ItemDirection::South, Item::ItemDirection::East, Item::ItemDirection::West };
		m_facingKey = propertiesList->addChoicesItem(QObject::tr("Facing"), stringChoices, valueChoices, m_direction);
	}
}

const lcsm::LCSMCircuitView &CircuitItem::circuit() const noexcept
{
	return m_view;
}

void CircuitItem::connect()
{
	const int index = DesignDialog::Connect(m_portStrings);
	if (index < 0)
	{
		return;
	}
	lcsm::Identifier id;
	bool isInput = true;
	int i = index;
	if (i < m_portInputIds.size())
	{
		id = m_portInputIds[i];
	}
	else
	{
		i -= m_portInputIds.size();
		id = m_portOutputIds[i];
		isInput = false;
	}
	m_coreScene->connectionAsCircuit(m_view.id(), id, m_view, isInput, index);
	// Also, there is thought of making lcsm::model::CircuitModel that will behave like lcsm::model::VerilogModule and
	// lcsm::LCSMCircuitView, so that it will be separate lcsm::Component, but it's MODEL, non-existing for all other
	// components.
}

bool CircuitItem::rotateActionEnabled() const noexcept
{
	return true;
}

bool CircuitItem::putValueActionEnabled() const noexcept
{
	return false;
}

lcsm::width_t CircuitItem::inputWidth() const noexcept
{
	return lcsm::Width::QuadWord;
}

qreal CircuitItem::circuitWidth() const noexcept
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	return gridSize * LOGISIM_EVOLUTION_CIRCUIT_SQUARE_WIDTH;
}

qreal CircuitItem::circuitHeight() const noexcept
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	const int n = qMax(m_portInputIds.size(), m_portOutputIds.size());
	const int h = (n - 1) * 2 + 2;
	return gridSize * (h + LOGISIM_EVOLUTION_CIRCUIT_SQUARE_HEIGHT_FOR_TITLE);
}
