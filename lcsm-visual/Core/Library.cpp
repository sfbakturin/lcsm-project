#include <Core/CoreScene.h>
#include <Core/Library.h>
#include <Items/ClockItem.h>
#include <Items/ComponentItem.h>
#include <Items/ConstantItem.h>
#include <Items/GroundItem.h>
#include <Items/Item.h>
#include <Items/PinItem.h>
#include <Items/PowerItem.h>
#include <Items/TransistorItem.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/std/Clock.h>
#include <lcsm/Model/std/Constant.h>
#include <lcsm/Model/std/Ground.h>
#include <lcsm/Model/std/Pin.h>
#include <lcsm/Model/std/Power.h>
#include <lcsm/Model/std/Transistor.h>

#include <QList>
#include <QObject>
#include <QString>
#include <stdexcept>

static const QString PIN = QObject::tr("Pin");
static const QString CONSTANT = QObject::tr("Constant");
static const QString GROUND = QObject::tr("Ground");
static const QString POWER = QObject::tr("Power");
static const QString CLOCK = QObject::tr("Clock");
static const QString TRANSISTOR = QObject::tr("Transistor");

const QList< QString > Library::DefaultList = { PIN, CONSTANT, GROUND, POWER, CLOCK, TRANSISTOR };

static lcsm::model::Pin *GetPin()
{
	return new lcsm::model::Pin("", false, lcsm::Width::Bit1);
}

static lcsm::model::Constant *GetConstant()
{
	return new lcsm::model::Constant("", lcsm::Width::Bit1, 1);
}

static lcsm::model::Ground *GetGround()
{
	return new lcsm::model::Ground("", lcsm::Width::Bit1);
}

static lcsm::model::Power *GetPower()
{
	return new lcsm::model::Power("", lcsm::Width::Bit1);
}

static lcsm::model::Clock *GetClock()
{
	return new lcsm::model::Clock("", 1, 1, 1);
}

static lcsm::model::Transistor *GetTransistor()
{
	return new lcsm::model::Transistor("", lcsm::model::Transistor::Type::N);
}

ComponentItem *Library::getItem(const QString &name, CoreScene *scene)
{
	if (name == PIN)
	{
		return new PinItem(scene, GetPin());
	}
	else if (name == CONSTANT)
	{
		return new ConstantItem(scene, GetConstant());
	}
	else if (name == GROUND)
	{
		return new GroundItem(scene, GetGround());
	}
	else if (name == POWER)
	{
		return new PowerItem(scene, GetPower());
	}
	else if (name == CLOCK)
	{
		return new ClockItem(scene, GetClock());
	}
	else if (name == TRANSISTOR)
	{
		return new TransistorItem(scene, GetTransistor());
	}
	else
	{
		throw std::logic_error("Unknown item!");
	}
}
