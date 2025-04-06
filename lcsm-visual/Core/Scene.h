#ifndef LCSM_VISUAL_CORE_SCENE_H
#define LCSM_VISUAL_CORE_SCENE_H

#include <Items/Item.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/std/Pin.h>
#include <unordered_map>

#include <memory>

class Scene
{
  public:
	Scene() = default;
	~Scene() noexcept = default;

	Scene(const Scene &other) = delete;
	Scene(Scene &&other) = delete;

	Scene &operator=(const Scene &other) = delete;
	Scene &operator=(Scene &&other) = delete;

	void addToScene(lcsm::model::Pin *pin);

  private:
	std::unordered_map< lcsm::Identifier, std::unique_ptr< Item > > m_items;
};

#endif /* LCSM_VISUAL_CORE_SCENE_H */
