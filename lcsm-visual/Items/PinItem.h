#ifndef LCSM_VISUAL_ITEMS_PINITEM_H
#define LCSM_VISUAL_ITEMS_PINITEM_H

#include <Items/Item.h>
#include <lcsm/Model/std/Pin.h>

class PinItem : public Item
{
  public:
	PinItem(lcsm::model::Pin *pin);
	~PinItem() noexcept = default;

  private:
	lcsm::model::Pin *m_pin;
};

#endif /* LCSM_VISUAL_ITEMS_PINITEM_H */
