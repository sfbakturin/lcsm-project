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
#include <lcsm/Model/Component.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Verilog.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Support/Algorithm.hpp>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Module.h>

#include <QDebug>
#include <QGraphicsScene>
#include <QHash>
#include <QObject>
#include <QRectF>
#include <memory>
#include <utility>

CoreScene::CoreScene(lcsm::LCSMCircuit *circuit, GUIOptions *options) :
	m_options(options), m_scene(new GUIScene(options->sceneRect(), options->gridSize())), m_circuit(circuit),
	m_connectionPortId1(0), m_connection1(false)
{
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

void setView(GUIView *view) noexcept;

void CoreScene::connection(lcsm::Identifier id, lcsm::portid_t portId)
{
	if (m_connection1)
	{
		// Connect in backend.
		lcsm::Component *c1 = m_circuit->find(m_connectionId1);
		lcsm::Component *c2 = m_circuit->find(id);
		lcsm::model::Wire *wire = m_circuit->connect(c1, m_connectionPortId1, c2, portId);

		// Create GUI Wire item.
		lcsm::support::PointerView< Item > item1 = m_items[m_connectionId1];
		lcsm::support::PointerView< Item > item2 = m_items[id];
		WireItem *item = new WireItem(item1.get(), item2.get(), m_connectionPortId1, portId, this, wire);
		addImpl(item);

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

void CoreScene::freeze(bool freeze)
{
	for (std::pair< const lcsm::Identifier, lcsm::support::PointerView< Item > > &it : m_items)
	{
		it.second->setFreeze(freeze);
	}
}

void CoreScene::aboutToBeCollected(bool aboutToBeCollected)
{
	for (std::pair< const lcsm::Identifier, lcsm::support::PointerView< Item > > &it : m_items)
	{
		it.second->setAboutToBeConnected(aboutToBeCollected);
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
		// TODO: Remove all painted wires.
		break;
	}
	case Item::ItemType::CircuitItemTy:
	{
		CircuitItem *circuitItem = static_cast< CircuitItem * >(item);
		const lcsm::LCSMCircuitView &circuit = circuitItem->circuit();
		m_items.erase(circuit.id());
		m_circuit->removeCircuit(circuit);
		// TODO: Remove all painted wires.
		break;
	}
	case Item::ItemType::VerilogItemTy:
	{
		VerilogItem *verilogItem = static_cast< VerilogItem * >(item);
		lcsm::Component *component = verilogItem->component();
		m_items.erase(component->id());
		m_circuit->remove(component);
		// TODO: Remove all painted wires.
		break;
	}
	case Item::ItemType::WireItemTy:
	{
		WireItem *wireItem = static_cast< WireItem * >(item);
		lcsm::Component *wire = wireItem->wire();
		m_items.erase(wire->id());
		m_circuit->remove(wire);
		// TODO: Remove all painted wires.
		break;
	}
	}
}

void CoreScene::addImpl(Item *item)
{
	item->setOptions(m_options.get());
	item->reid();
	m_items[item->id()] = item;
	m_scene->addItem(item);
}
