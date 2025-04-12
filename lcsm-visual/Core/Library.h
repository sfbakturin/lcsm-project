#ifndef LCSM_VISUAL_CORE_LIBRARY_H
#define LCSM_VISUAL_CORE_LIBRARY_H

#include <lcsm/Model/std/Pin.h>

#include <QList>
#include <QString>

class CoreScene;
class ComponentItem;

class Library
{
  public:
	static const QList< QString > DefaultList;

	static ComponentItem *getItem(const QString &name, CoreScene *scene);

  private:
	static lcsm::model::Pin *getPin();
};

#endif /* LCSM_VISUAL_CORE_LIBRARY_H */
