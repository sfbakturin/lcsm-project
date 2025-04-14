#include <Core/CoreScene.h>
#include <Items/Item.h>
#include <Items/WireItem.h>
#include <Items/WireLine.h>
#include <View/PropertiesList.h>
#include <View/SimulateDialog.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Support/PointerView.hpp>

#include <QAction>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QMenu>
#include <QObject>
#include <QPointF>
#include <memory>

Item::Item(CoreScene *coreScene, lcsm::Identifier id, GUIOptions *options) :
	m_coreScene(coreScene), m_id(id), m_options(options), m_aboutToBeConnected(false), m_simulate(false),
	m_direction(Item::ItemDirection::East)
{
	// Design context menu.
	m_designContextMenu = std::unique_ptr< QMenu >(new QMenu());
	m_connectAction = m_designContextMenu->addAction(QObject::tr("Connect..."));
	m_removeAction = m_designContextMenu->addAction(QObject::tr("Remove"));
	m_designContextMenu->addSeparator();
	m_rotateRightAction = m_designContextMenu->addAction(QObject::tr("Rotate right"));
	m_rotateLeftAction = m_designContextMenu->addAction(QObject::tr("Rotate left"));

	// Simulate context menu.
	m_simulateContextMenu = std::unique_ptr< QMenu >(new QMenu());
	m_putValueAction = m_simulateContextMenu->addAction(QObject::tr("Put value..."));
	m_showValueAction = m_simulateContextMenu->addAction(QObject::tr("Show value..."));
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

void Item::setSimulate(bool simulate) noexcept
{
	m_simulate = simulate;
}

bool Item::simulate() const noexcept
{
	return m_simulate;
}

static constexpr uint DIRECT = 4;

void Item::rotateRight()
{
	m_direction = static_cast< Item::ItemDirection >((m_direction + 1) % DIRECT);
	prepareGeometryChange();
	adjust();
}

void Item::rotateLeft()
{
	m_direction = static_cast< Item::ItemDirection >(m_direction == 0 ? Item::ItemDirection::North : (m_direction - 1) % DIRECT);
	prepareGeometryChange();
	adjust();
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

void Item::addWireLine(WireLine *wireLine)
{
	m_wireLines << wireLine;
}

void Item::removeWireLine(WireLine *wireLine)
{
	m_wireLines.removeAll(wireLine);
}

QList< WireLine * > &Item::wireLines() noexcept
{
	return m_wireLines;
}

void Item::setState(lcsm::LCSMState *state) noexcept
{
	m_state = state;
}

void Item::resetState() noexcept
{
	m_state.reset();
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::MouseButton::LeftButton)
	{
		// Reset connection.
		m_coreScene->resetConnection();
		setAboutToBeConnected(false);

		// If not freezed, accept as graphical item.
		if (!m_simulate)
		{
			emit m_coreScene->showItem(this);
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
	if (!m_simulate)
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
	// Deactivate some actions to item's properties.
	m_rotateLeftAction->setEnabled(rotateActionEnabled());
	m_rotateRightAction->setEnabled(rotateActionEnabled());
	m_putValueAction->setEnabled(putValueActionEnabled());

	QAction *selected = nullptr;
	const QPoint p = event->screenPos();

	if (m_simulate)
	{
		selected = m_simulateContextMenu->exec(p);
	}
	else
	{
		selected = m_designContextMenu->exec(p);
	}

	if (m_connectAction == selected)
	{
		connect();
	}
	else if (m_removeAction == selected)
	{
		m_coreScene->removeItem(this);
		delete this;
	}
	else if (m_rotateRightAction == selected)
	{
		rotateRight();
	}
	else if (m_rotateLeftAction == selected)
	{
		rotateLeft();
	}
	else if (m_putValueAction == selected)
	{
		bool ok;
		const lcsm::DataBits databits = SimulateDialog::PutValue(lcsm::Width::QuadWord, std::addressof(ok));
		if (ok)
		{
			m_state->putValue(id(), { databits });
		}
	}
	else if (m_showValueAction == selected)
	{
		SimulateDialog::ShowValue(m_state.cptr(), id());
	}
}

void Item::adjust()
{
	for (WireLine *wireLine : qAsConst(m_wireLines))
	{
		wireLine->adjust();
	}
}
