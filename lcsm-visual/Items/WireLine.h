#ifndef LCSM_VISUAL_ITEMS_WIREITEMLINE_H
#define LCSM_VISUAL_ITEMS_WIREITEMLINE_H

#include <Items/Item.h>
#include <lcsm/LCSM.h>

#include <QGraphicsItem>
#include <QGraphicsPathItem>

class WireLine : public QGraphicsPathItem
{
  public:
	WireLine(Item *item1, Item *item2, lcsm::portid_t port1, lcsm::portid_t port2, QGraphicsItem *parent = nullptr);

	void adjust();

	Item *item1() noexcept;
	Item *item2() noexcept;

	lcsm::portid_t port1() const noexcept;
	lcsm::portid_t port2() const noexcept;

  private:
	Item *m_item1;
	Item *m_item2;
	lcsm::portid_t m_port1;
	lcsm::portid_t m_port2;
};

#endif /* LCSM_VISUAL_ITEMS_WIREITEMLINE_H */
