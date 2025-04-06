#include <Items/Item.h>
#include <Items/PinItem.h>
#include <lcsm/Model/std/Pin.h>

PinItem::PinItem(lcsm::model::Pin *pin) : Item(pin->id()), m_pin(pin) {}
