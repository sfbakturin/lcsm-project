#ifndef LCSM_VISUAL_VIEW_DESIGNDIALOG_H
#define LCSM_VISUAL_VIEW_DESIGNDIALOG_H

#include <QList>
#include <QString>

class DesignDialog
{
  public:
	static int Connect(const QList< QString > &portStrings);
};

#endif /* LCSM_VISUAL_VIEW_DESIGNDIALOG_H */
