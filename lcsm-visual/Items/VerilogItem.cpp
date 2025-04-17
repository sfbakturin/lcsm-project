#include <Core/CoreScene.h>
#include <GUI/GUIOptions.h>
#include <Items/MovableItem.h>
#include <Items/VerilogItem.h>
#include <View/DesignDialog.h>
#include <View/PropertiesList.h>
#include <lcsm/LCSM.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Verilog.h>
#include <lcsm/Verilog/Module.h>
#include <lcsm/Verilog/Port.h>

#include <QBrush>
#include <QList>
#include <QPainter>
#include <QPainterPath>
#include <QPair>
#include <QPen>
#include <QPointF>
#include <QRectF>
#include <QString>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QWidget>

static constexpr unsigned LOGISIM_EVOLUTION_CIRCUIT_SQUARE_WIDTH = 10;
static constexpr unsigned LOGISIM_EVOLUTION_CIRCUIT_SQUARE_HEIGHT_FOR_TITLE = 3;

VerilogItem::VerilogItem(CoreScene *scene, lcsm::model::VerilogModule *verilogModule, const lcsm::verilog::Module *module, GUIOptions *options) :
	MovableItem(scene, verilogModule->id(), options), m_verilogModule(verilogModule), m_facingKey(0)
{
	const QString inpString = QObject::tr("Input") + ": ";
	const QString inoString = QObject::tr("Inout") + ": ";
	const QString outString = QObject::tr("Output") + ": ";
	for (const lcsm::verilog::Port &port : module->inputPorts())
	{
		const QString name = inpString + QString::fromStdString(port.identifier());
		m_portStrings << name;
	}
	for (const lcsm::verilog::Port &port : module->inoutPorts())
	{
		const QString name = inoString + QString::fromStdString(port.identifier());
		m_portStrings << name;
	}
	for (const lcsm::verilog::Port &port : module->outputPorts())
	{
		const QString name = outString + QString::fromStdString(port.identifier());
		m_portStrings << name;
	}
}

Item::ItemType VerilogItem::ty() const noexcept
{
	return Item::ItemType::VerilogItemTy;
}

void VerilogItem::reid() noexcept
{
	m_id = m_verilogModule->id();
}

QRectF VerilogItem::boundingRect() const
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	const qreal w = verilogWidth();
	const qreal h = verilogHeight();
	const qreal additionBoundRectForPortShow = 2 * gridSize;
	const qreal offset = additionBoundRectForPortShow * 2;
	switch (m_direction)
	{
	case Item::ItemDirection::East:
	case Item::ItemDirection::West:
	{
		return QRectF(0.0, 0.0, w + offset, h + offset);
	}
	case Item::ItemDirection::South:
	case Item::ItemDirection::North:
	{
		return QRectF(0.0, 0.0, h + offset, w + offset);
	}
	}
	return QRectF();
}

void VerilogItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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
			const QPointF startPoint = QPointF(offset, offset);

			QPainterPath path(startPoint);
			const QRectF rect(startPoint.x(), startPoint.y(), verilogWidth(), verilogHeight());
			path.addRect(rect);

			painter->drawPath(path);

			break;
		}
		case Item::ItemDirection::South:
		case Item::ItemDirection::North:
		{
			const QPointF startPoint = QPointF(0.0, offset);

			QPainterPath path(startPoint);
			const QRectF rect(startPoint.x(), startPoint.y(), verilogHeight(), verilogWidth());
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
		const QPointF offToRight = QPointF(gs, 0.0);
		const QPointF offToLeft = QPointF(-gs, 0.0);
		const QPointF offToUp = QPointF(0.0, -gs);
		const QPointF offToDown = QPointF(0.0, gs);

		for (int i = 0; i < m_portStrings.size(); i++)
		{
			const lcsm::portid_t portId = static_cast< lcsm::portid_t >(i);
			const QPair< VerilogItem::PortTypeItem, int > p = getTypeAndIndexFrom(portId);
			const VerilogItem::PortTypeItem type = p.first;
			const QPointF rpos = relativePositionOfPort(portId);
			painter->drawEllipse(rpos, r, r);
			QPointF endline = rpos;
			switch (m_direction)
			{
			case Item::ItemDirection::East:
			{
				switch (type)
				{
				case VerilogItem::PortTypeItem::Input:
				{
					endline += offToRight;
					break;
				}
				case VerilogItem::PortTypeItem::Output:
				{
					endline += offToLeft;
					break;
				}
				case VerilogItem::PortTypeItem::Inout:
				{
					endline += offToDown;
					break;
				}
				}
				break;
			}
			case Item::ItemDirection::South:
			{
				switch (type)
				{
				case VerilogItem::PortTypeItem::Input:
				{
					endline += offToDown;
					break;
				}
				case VerilogItem::PortTypeItem::Output:
				{
					endline += offToUp;
					break;
				}
				case VerilogItem::PortTypeItem::Inout:
				{
					endline += offToLeft;
					break;
				}
				}
				break;
			}
			case Item::ItemDirection::West:
			{
				switch (type)
				{
				case VerilogItem::PortTypeItem::Input:
				{
					endline += offToLeft;
					break;
				}
				case VerilogItem::PortTypeItem::Output:
				{
					endline += offToRight;
					break;
				}
				case VerilogItem::PortTypeItem::Inout:
				{
					endline += offToUp;
					break;
				}
				}
				break;
			}
			case Item::ItemDirection::North:
			{
				switch (type)
				{
				case VerilogItem::PortTypeItem::Input:
				{
					endline += offToUp;
					break;
				}
				case VerilogItem::PortTypeItem::Output:
				{
					endline += offToDown;
					break;
				}
				case VerilogItem::PortTypeItem::Inout:
				{
					endline += offToRight;
					break;
				}
				}
				break;
			}
			}
			painter->drawLine(rpos, endline);
		}
	}
}

QPointF VerilogItem::relativePositionOfPort(lcsm::portid_t portId) const
{
	const QPair< VerilogItem::PortTypeItem, int > p = getTypeAndIndexFrom(portId);
	const VerilogItem::PortTypeItem type = p.first;
	const int index = p.second;
	const qreal gs = static_cast< qreal >(options()->gridSize());
	const QRectF bound = boundingRect();

	qreal xpos = 0.0;
	qreal ypos = 0.0;

	switch (m_direction)
	{
	case Item::ItemDirection::East:
	{
		// X.
		switch (type)
		{
		case VerilogItem::PortTypeItem::Input:
		{
			xpos = gs;
			break;
		}
		case VerilogItem::PortTypeItem::Output:
		{
			xpos = bound.width() - gs;
			break;
		}
		case VerilogItem::PortTypeItem::Inout:
		{
			xpos = 3 * gs;
			xpos += 2 * index * gs;
			break;
		}
		}

		// Y.
		switch (type)
		{
		case VerilogItem::PortTypeItem::Input:
		{
			ypos = 3 * gs;
			ypos += 2 * index * gs;
			break;
		}
		case VerilogItem::PortTypeItem::Output:
		{
			ypos = 3 * gs;
			ypos += 2 * index * gs;
			break;
		}
		case VerilogItem::PortTypeItem::Inout:
		{
			ypos = gs;
			break;
		}
		}

		// break
		break;
	}
	case Item::ItemDirection::South:
	{
		// X.
		switch (type)
		{
		case VerilogItem::PortTypeItem::Input:
		{
			xpos = bound.width() - 3 * gs;
			xpos -= 2 * index * gs;
			break;
		}
		case VerilogItem::PortTypeItem::Output:
		{
			xpos = bound.width() - 3 * gs;
			xpos -= 2 * index * gs;
			break;
		}
		case VerilogItem::PortTypeItem::Inout:
		{
			xpos = bound.width() - gs;
			break;
		}
		}

		// Y.
		switch (type)
		{
		case VerilogItem::PortTypeItem::Input:
		{
			ypos = gs;
			break;
		}
		case VerilogItem::PortTypeItem::Output:
		{
			ypos = bound.height() - gs;
			break;
		}
		case VerilogItem::PortTypeItem::Inout:
		{
			ypos = 3 * gs;
			ypos += 2 * index * gs;
			break;
		}
		}

		// break
		break;
	}
	case Item::ItemDirection::West:
	{
		// X.
		switch (type)
		{
		case VerilogItem::PortTypeItem::Input:
		{
			xpos = bound.width() - gs;
			break;
		}
		case VerilogItem::PortTypeItem::Output:
		{
			xpos = gs;
			break;
		}
		case VerilogItem::PortTypeItem::Inout:
		{
			xpos = bound.width() - 3 * gs;
			xpos -= 2 * index * gs;
			break;
		}
		}

		// Y.
		switch (type)
		{
		case VerilogItem::PortTypeItem::Input:
		{
			ypos = bound.height() - 3 * gs;
			ypos -= 2 * index * gs;
			break;
		}
		case VerilogItem::PortTypeItem::Output:
		{
			ypos = bound.height() - 3 * gs;
			ypos -= 2 * index * gs;
			break;
		}
		case VerilogItem::PortTypeItem::Inout:
		{
			ypos = bound.height() - gs;
			break;
		}
		}

		// break
		break;
	}
	case Item::ItemDirection::North:
	{
		// X.
		switch (type)
		{
		case VerilogItem::PortTypeItem::Input:
		{
			xpos = 3 * gs;
			xpos += 2 * index * gs;
			break;
		}
		case VerilogItem::PortTypeItem::Output:
		{
			xpos = 3 * gs;
			xpos += 2 * index * gs;
			break;
		}
		case VerilogItem::PortTypeItem::Inout:
		{
			xpos = gs;
			break;
		}
		}

		// Y.
		switch (type)
		{
		case VerilogItem::PortTypeItem::Input:
		{
			ypos = bound.height() - gs;
			break;
		}
		case VerilogItem::PortTypeItem::Output:
		{
			ypos = gs;
			break;
		}
		case VerilogItem::PortTypeItem::Inout:
		{
			ypos = bound.height() - 3 * gs;
			ypos -= 2 * index * gs;
			break;
		}
		}

		// break
		break;
	}
	}

	return QPointF(xpos, ypos);
}

void VerilogItem::setProperty(int key, const QVariant &value)
{
	// Facing
	if (key == m_facingKey)
	{
		const uint direction = value.toUInt();
		m_direction = static_cast< Item::ItemDirection >(direction);
		directionChanged();
	}
}

void VerilogItem::setPropertiesList(PropertiesList *propertiesList)
{
	// Facing.
	{
		const QList< QString > stringChoices = { QObject::tr("North"), QObject::tr("South"), QObject::tr("East"), QObject::tr("West") };
		const QList< QVariant > valueChoices = { Item::ItemDirection::North, Item::ItemDirection::South, Item::ItemDirection::East, Item::ItemDirection::West };
		m_facingKey = propertiesList->addChoicesItem(QObject::tr("Facing"), stringChoices, valueChoices, m_direction);
	}
}

lcsm::Component *VerilogItem::component() noexcept
{
	return m_verilogModule;
}

void VerilogItem::connect()
{
	const int index = DesignDialog::Connect(m_portStrings);
	if (index < 0)
	{
		return;
	}
	m_coreScene->connection(m_verilogModule->id(), static_cast< lcsm::portid_t >(index));
}

bool VerilogItem::rotateActionEnabled() const noexcept
{
	return true;
}

bool VerilogItem::putValueActionEnabled() const noexcept
{
	return false;
}

lcsm::width_t VerilogItem::inputWidth() const noexcept
{
	return lcsm::Width::QuadWord;
}

qreal VerilogItem::verilogWidth() const noexcept
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	const unsigned n = (m_verilogModule->numOfInouts() - 1) * 2 + 2;
	const unsigned w = qMax(LOGISIM_EVOLUTION_CIRCUIT_SQUARE_WIDTH, n);
	return gridSize * w;
}

qreal VerilogItem::verilogHeight() const noexcept
{
	const qreal gridSize = static_cast< qreal >(options()->gridSize());
	const int n = qMax(m_verilogModule->numOfInputs(), m_verilogModule->numOfOutputs());
	const int h = (n - 1) * 2 + 2;
	return gridSize * (h + LOGISIM_EVOLUTION_CIRCUIT_SQUARE_HEIGHT_FOR_TITLE);
}

QPair< VerilogItem::PortTypeItem, int > VerilogItem::getTypeAndIndexFrom(lcsm::portid_t portId) const
{
	PortTypeItem type = PortTypeItem::Input;
	int index = portId;

	if (index >= m_verilogModule->numOfInputs())
	{
		index -= m_verilogModule->numOfInputs();
		type = PortTypeItem::Inout;
	}
	else
	{
		goto lReturn;
	}

	if (index >= m_verilogModule->numOfInouts())
	{
		index -= m_verilogModule->numOfInouts();
		type = PortTypeItem::Output;
	}
	else
	{
		goto lReturn;
	}

lReturn:
	return QPair< VerilogItem::PortTypeItem, int >(type, index);
}
