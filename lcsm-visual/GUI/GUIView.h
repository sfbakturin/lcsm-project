#ifndef LCSM_VISUAL_GUI_GUIVIEW_H
#define LCSM_VISUAL_GUI_GUIVIEW_H

#include <QGraphicsView>
#include <QWidget>

class CoreScene;
class Item;

class GUIView : public QGraphicsView
{
  public:
	explicit GUIView(QWidget *parent = nullptr);
	~GUIView() noexcept = default;

	void setCoreScene(CoreScene *coreScene);
	void unsetCoreScene();

	CoreScene *coreScene() noexcept;

  signals:
	void showItem(Item *item);

  private:
	CoreScene *m_coreScene;
};

#endif /* LCSM_VISUAL_GUI_GUIVIEW_H */
