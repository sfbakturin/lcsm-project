#include <Core/CoreScene.h>
#include <Core/Project.h>
#include <GUI/GUIOptions.h>
#include <GUI/GUIScene.h>
#include <Items/CircuitItem.h>
#include <Items/ComponentItem.h>
#include <Items/Item.h>
#include <Items/PinItem.h>
#include <Items/VerilogItem.h>
#include <Items/WireItem.h>
#include <Items/WireLine.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Verilog.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Module.h>

#include <QAction>
#include <QDebug>
#include <QGraphicsScene>
#include <QHash>
#include <QLineF>
#include <QMenu>
#include <QObject>
#include <QPointF>
#include <QRectF>
#include <memory>
#include <utility>

CoreScene::CoreScene() :
	m_options(nullptr), m_scene(nullptr), m_circuit(nullptr), m_connectionPortId1(0), m_connection1(false)
{
}

CoreScene::CoreScene(lcsm::LCSMCircuit *circuit, GUIOptions *options) :
	m_options(options), m_scene(new GUIScene(options->sceneRect(), this, options->gridSize())), m_circuit(circuit),
	m_connectionPortId1(0), m_connection1(false)
{
	m_scene->setCoreScene(this);
}

CoreScene::CoreScene(CoreScene &&other) noexcept :
	m_options(std::move(other.m_options)), m_items(std::move(other.m_items)), m_scene(std::move(other.m_scene)),
	m_circuit(std::move(other.m_circuit)), m_view(std::move(other.m_view)), m_connectionId1(std::move(other.m_connectionId1)),
	m_connectionPortId1(other.m_connectionPortId1), m_connection1(other.m_connection1)
{
}

CoreScene &CoreScene::operator=(CoreScene &&other)
{
	return lcsm::support::MoveAssign< CoreScene >(this, std::move(other));
}

void CoreScene::swap(CoreScene &other)
{
	std::swap(m_options, other.m_options);
	std::swap(m_items, other.m_items);
	std::swap(m_scene, other.m_scene);
	std::swap(m_circuit, other.m_circuit);
	std::swap(m_view, other.m_view);
	std::swap(m_connectionId1, other.m_connectionId1);
	std::swap(m_connectionPortId1, other.m_connectionPortId1);
	std::swap(m_connection1, other.m_connection1);
}

GUIScene *CoreScene::scene() noexcept
{
	return m_scene.get();
}

GUIView *CoreScene::view() noexcept
{
	return m_view.get();
}

void CoreScene::setView(GUIView *view) noexcept
{
	m_view = view;
}

void CoreScene::connection(lcsm::Identifier id, lcsm::portid_t portId)
{
	if (m_connection1 && m_connectionId1 != id)
	{
		// Connect in backend.
		lcsm::Component *c1 = m_circuit->find(m_connectionId1);
		lcsm::Component *c2 = m_circuit->find(id);
		lcsm::model::Wire *wire = m_circuit->connect(c1, m_connectionPortId1, c2, portId);

		// Create GUI Wire item.
		lcsm::support::PointerView< Item > item1 = m_items[m_connectionId1];
		lcsm::support::PointerView< Item > item2 = m_items[id];

		WireItem *item = new WireItem(this, wire, m_options.get());
		WireLine *line1 = new WireLine(item1.get(), item, m_connectionPortId1, lcsm::model::Wire::Port::Wiring);
		WireLine *line2 = new WireLine(item2.get(), item, portId, lcsm::model::Wire::Port::Wiring);
		line1->setZValue(0.0);
		line2->setZValue(0.0);

		const QPointF p1 = item1->absolutePositionOfPort(m_connectionPortId1);
		const QPointF p2 = item2->absolutePositionOfPort(portId);

		const qreal gs = static_cast< qreal >(m_options->gridSize());

		QPointF pos = QLineF(p1, p2).center();
		pos.setX(qRound(pos.x() / gs) * gs);
		pos.setY(qRound(pos.y() / gs) * gs);

		item->setPos(pos);

		addImpl(item);
		m_scene->addItem(line1);
		m_scene->addItem(line2);

		// Reset GUI and non-GUI connection-action.
		item1->setAboutToBeConnected(false);
		item2->setAboutToBeConnected(false);
		m_connection1 = false;
	}
	else
	{
		m_connectionId1 = id;
		m_connectionPortId1 = portId;
		m_connection1 = true;
		m_items[id]->setAboutToBeConnected(true);
	}
}

void CoreScene::resetConnection()
{
	if (m_connection1)
	{
		m_items[m_connectionId1]->setAboutToBeConnected(false);
	}
	m_connection1 = false;
}

void CoreScene::add(const lcsm::LCSMCircuit &circuit)
{
	lcsm::LCSMCircuitView view = m_circuit->addCircuit(circuit);
	Item *item = new CircuitItem(this, view);
	addImpl(item);
}

void CoreScene::add(const lcsm::verilog::Module &module)
{
	lcsm::model::VerilogModule *verilogModule = m_circuit->createVerilogModule(module);
	Item *item = new VerilogItem(this, verilogModule);
	addImpl(item);
}

void CoreScene::add(ComponentItem *item)
{
	lcsm::Component *component = item->component();
	m_circuit->create(component);
	addImpl(item);
}

void CoreScene::aboutToBeConnected(bool aboutToBeConnected)
{
	for (std::pair< const lcsm::Identifier, lcsm::support::PointerView< Item > > &it : m_items)
	{
		it.second->setAboutToBeConnected(aboutToBeConnected);
	}
}

void CoreScene::setSelected(bool selected)
{
	for (std::pair< const lcsm::Identifier, lcsm::support::PointerView< Item > > &it : m_items)
	{
		it.second->setSelected(selected);
	}
}

void CoreScene::removeItem(Item *item)
{
	// Make scene not to paint this item.
	m_scene->removeItem(item);

	switch (item->ty())
	{
	case Item::ItemType::ComponentItemTy:
	{
		ComponentItem *componentItem = static_cast< ComponentItem * >(item);
		lcsm::Component *component = componentItem->component();
		m_items.erase(component->id());
		m_circuit->remove(component);
		break;
	}
	case Item::ItemType::CircuitItemTy:
	{
		CircuitItem *circuitItem = static_cast< CircuitItem * >(item);
		const lcsm::LCSMCircuitView &circuit = circuitItem->circuit();
		m_items.erase(circuit.id());
		m_circuit->removeCircuit(circuit);
		break;
	}
	case Item::ItemType::VerilogItemTy:
	{
		VerilogItem *verilogItem = static_cast< VerilogItem * >(item);
		lcsm::Component *component = verilogItem->component();
		m_items.erase(component->id());
		m_circuit->remove(component);
		break;
	}
	case Item::ItemType::WireItemTy:
	{
		WireItem *wireItem = static_cast< WireItem * >(item);
		lcsm::Component *wire = wireItem->wire();
		m_items.erase(wire->id());
		m_circuit->remove(wire);
		break;
	}
	}

	// Remove all painted wires.
	for (WireLine *wireLine : item->wireLines())
	{
		Item *i1 = wireLine->item1();
		Item *i2 = wireLine->item2();
		Item *i = (item == i1 ? i2 : i1);
		i->removeWireLine(wireLine);
		m_scene->removeItem(wireLine);
	}

	// Reset to default.
	m_connection1 = false;

	// Make everyone connected know that there is removed item.
	emit removeItem();
}

void CoreScene::commitProperties(lcsm::Component *component)
{
	m_circuit->commitProperties(component);
}

void CoreScene::startSimulate(bool start)
{
	if (start)
	{
		m_engine = std::unique_ptr< lcsm::LCSMEngine >(new lcsm::LCSMEngine(lcsm::LCSMEngine::fromCircuit(m_circuit.cref())));
		m_state = std::unique_ptr< lcsm::LCSMState >(new lcsm::LCSMState(m_engine->fork()));
		for (std::pair< const lcsm::Identifier, lcsm::support::PointerView< Item > > &it : m_items)
		{
			it.second->setSimulate(true);
			it.second->setState(m_state.get());
		}
	}
	else
	{
		m_state.reset();
		m_engine.reset();
		for (std::pair< const lcsm::Identifier, lcsm::support::PointerView< Item > > &it : m_items)
		{
			it.second->setSimulate(false);
			it.second->resetState();
		}
	}
}

void CoreScene::stepSimulate()
{
	m_state->tick();
	m_scene->update();
}

void CoreScene::loopSimulate(bool start)
{
	Q_UNUSED(start);
}

void CoreScene::addImpl(Item *item)
{
	item->setOptions(m_options.get());
	item->reid();
	m_items[item->id()] = item;
	m_scene->addItem(item);
}
