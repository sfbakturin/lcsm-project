#include <Core/CoreScene.h>
#include <Core/Library.h>
#include <Items/ComponentItem.h>
#include <Items/Item.h>
#include <Items/PinItem.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Pin.h>

#include <QList>
#include <QObject>
#include <QString>
#include <stdexcept>

const QList< QString > Library::DefaultList = { QObject::tr("Pin") };

ComponentItem *Library::getItem(const QString &name, CoreScene *scene)
{
	if (name == QObject::tr("Pin"))
	{
		return new PinItem(scene, Library::getPin());
	}
	else
	{
		throw std::logic_error("Unknown item!");
	}
}

lcsm::model::Pin *Library::getPin()
{
	return new lcsm::model::Pin("", false, lcsm::Width::Bit1);
}
