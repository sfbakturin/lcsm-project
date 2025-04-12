#include <Core/CoreScene.h>
#include <Items/Item.h>
#include <lcsm/Model/Identifier.h>

#include <QAction>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QObject>
#include <QPointF>
#include <memory>

Item::Item(CoreScene *coreScene, lcsm::Identifier id, GUIOptions *options) :
	m_coreScene(coreScene), m_id(id), m_options(options), m_freeze(false), m_aboutToBeConnected(false),
	m_direction(Item::ItemDirection::East)
{
	// Context menu.
	m_contextMenu = std::unique_ptr< QMenu >(new QMenu());
	m_connectAction = m_contextMenu->addAction(QObject::tr("Connect..."));
	m_removeAction = m_contextMenu->addAction(QObject::tr("Remove"));
	m_contextMenu->addSeparator();
	m_rotateRightAction = m_contextMenu->addAction(QObject::tr("Rotate right"));
	m_rotateLeftAction = m_contextMenu->addAction(QObject::tr("Rotate left"));
}

lcsm::Identifier Item::id() const noexcept
{
	return m_id;
}

GUIOptions *Item::options() noexcept
{
	return m_options;
}

const GUIOptions *Item::options() const noexcept
{
	return m_options;
}

void Item::setOptions(GUIOptions *options) noexcept
{
	m_options = options;
}

void Item::setFreeze(bool freeze) noexcept
{
	m_freeze = freeze;
	m_contextMenu->setEnabled(!freeze);
	update();
}

bool Item::freeze() const noexcept
{
	return m_freeze;
}

static constexpr uint DIRECT = 4;

void Item::rotateRight()
{
	m_direction = static_cast< Item::ItemDirection >((m_direction + 1) % DIRECT);
	prepareGeometryChange();
}

void Item::rotateLeft()
{
	m_direction = static_cast< Item::ItemDirection >(m_direction == 0 ? Item::ItemDirection::North : (m_direction - 1) % DIRECT);
	prepareGeometryChange();
}

void Item::setAboutToBeConnected(bool aboutToBeConnected)
{
	m_aboutToBeConnected = aboutToBeConnected;
	update();
}

bool Item::aboutToBeConnected() const noexcept
{
	return m_aboutToBeConnected;
}

QPointF Item::absolutePositionOfPort(lcsm::portid_t portId) const
{
	return mapToScene(relativePositionOfPort(portId));
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::MouseButton::LeftButton)
	{
		// Reset connection.
		m_coreScene->resetConnection();
		setAboutToBeConnected(false);

		// If not freezed, accept as graphical item.
		if (!m_freeze)
		{
			QGraphicsItem::mousePressEvent(event);
			update();
		}
		else
		{
			event->accept();
		}
	}
}

void Item::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (!m_freeze)
	{
		prepareGeometryChange();
		if (event->modifiers() & Qt::ShiftModifier)
		{
			update();
		}
		QGraphicsItem::mouseMoveEvent(event);
		return;
	}
	event->accept();
}

void Item::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	update();
}

void Item::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QAction *selected = m_contextMenu->exec(event->screenPos());
	if (selected == m_connectAction)
	{
		connect();
	}
	else if (selected == m_removeAction)
	{
		m_coreScene->removeItem(this);
		delete this;
	}
	else if (selected == m_rotateRightAction)
	{
		rotateRight();
	}
	else if (selected == m_rotateLeftAction)
	{
		rotateLeft();
	}
}
