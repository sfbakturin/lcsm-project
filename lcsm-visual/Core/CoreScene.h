#ifndef LCSM_VISUAL_CORE_SCENE_H
#define LCSM_VISUAL_CORE_SCENE_H

#include <GUI/GUIOptions.h>
#include <GUI/GUIScene.h>
#include <GUI/GUIView.h>
#include <Items/ComponentItem.h>
#include <Items/Item.h>
#include <lcsm/LCSM.h>
#include <lcsm/LCSMCircuit.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Module.h>
#include <unordered_map>

#include <QGraphicsScene>
#include <memory>

class CoreScene
{
  public:
	CoreScene() = default;
	CoreScene(lcsm::LCSMCircuit *circuit, GUIOptions *options);
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
	void resetConnection();

	void add(const lcsm::LCSMCircuit &circuit);
	void add(const lcsm::verilog::Module &module);
	void add(ComponentItem *componentItem);

	void freeze(bool freeze);
	void aboutToBeCollected(bool aboutToBeCollected);
	void removeItem(Item *item);

  private:
	lcsm::support::PointerView< GUIOptions > m_options;

	std::unordered_map< lcsm::Identifier, lcsm::support::PointerView< Item > > m_items;
	std::unique_ptr< GUIScene > m_scene;
	lcsm::support::PointerView< lcsm::LCSMCircuit > m_circuit;

	lcsm::support::PointerView< GUIView > m_view;

	/* === CONTEXT MENU === */
	// TODO: Make only one menu for all items.

	/* === CONNECTION === */

	lcsm::Identifier m_connectionId1;
	lcsm::portid_t m_connectionPortId1;
	bool m_connection1;

  private:
	void addImpl(Item *item);
};

#endif /* LCSM_VISUAL_CORE_SCENE_H */
