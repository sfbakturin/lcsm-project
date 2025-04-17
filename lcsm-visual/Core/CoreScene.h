#ifndef LCSM_VISUAL_CORE_SCENE_H
#define LCSM_VISUAL_CORE_SCENE_H

#include <GUI/GUIOptions.h>
#include <GUI/GUIScene.h>
#include <GUI/GUIView.h>
#include <Items/ComponentItem.h>
#include <Items/Item.h>
#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/LCSMEngine.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Module.h>
#include <unordered_map>

#include <QAction>
#include <QGraphicsScene>
#include <QMenu>
#include <QObject>
#include <QPoint>
#include <memory>

class Project;

class CoreScene : public QObject
{
	Q_OBJECT

  public:
	CoreScene();
	CoreScene(Project *project, lcsm::LCSMCircuit *circuit, GUIOptions *options);
	~CoreScene() noexcept = default;

	CoreScene(const CoreScene &other) = delete;
	CoreScene(CoreScene &&other) noexcept;

	CoreScene &operator=(const CoreScene &other) = delete;
	CoreScene &operator=(CoreScene &&other);

	void swap(CoreScene &other);

	GUIScene *scene() noexcept;

	GUIView *view() noexcept;
	void setView(GUIView *view) noexcept;

	void connection(lcsm::Identifier id, lcsm::portid_t portId);
	void connectionAsCircuit(lcsm::Identifier circuitId, lcsm::Identifier componentId, lcsm::LCSMCircuitView &view, bool isInput, int portId);
	void resetConnection();
	void add(const lcsm::LCSMCircuit &circuit);
	void add(const lcsm::verilog::Module &module);
	void add(ComponentItem *componentItem);

	void aboutToBeConnected(bool aboutToBeConnected);
	void setSelected(bool selected);
	void removeItem(Item *item);
	void commitProperties(lcsm::Component *component);

	void startSimulate(bool start);
	void stepSimulate();
	void loopSimulate(bool start);

  signals:
	void showItem(Item *item);
	void removeItem();

  private:
	lcsm::support::PointerView< Project > m_project;
	lcsm::support::PointerView< GUIOptions > m_options;

	std::unordered_map< lcsm::Identifier, lcsm::support::PointerView< Item > > m_items;
	std::unique_ptr< GUIScene > m_scene;
	lcsm::support::PointerView< lcsm::LCSMCircuit > m_circuit;

	lcsm::support::PointerView< GUIView > m_view;

	/* === CONNECTION === */
	lcsm::Identifier m_connectionId1;
	lcsm::portid_t m_connectionPortId1;
	bool m_connection1;

	/* === CONNECTION AS CIRCUIT === */
	lcsm::Identifier m_connectionAsCircuitId1;
	lcsm::Identifier m_connectionAsCircuitComponentId1;
	lcsm::LCSMCircuitView m_connectionAsCircuitView1;
	bool m_connectionAsCircuit1;
	bool m_connectionAsCircuitIsInput1;
	int m_connectionAsCircuitPortId1;

	/* === SIMULATE === */
	std::unique_ptr< lcsm::LCSMEngine > m_engine;
	std::unique_ptr< lcsm::LCSMState > m_state;

  private:
	void addImpl(Item *item);
};

#endif /* LCSM_VISUAL_CORE_SCENE_H */
