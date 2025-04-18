#ifndef LCSM_VISUAL_GUI_GUISCENE_H
#define LCSM_VISUAL_GUI_GUISCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QRectF>
#include <QWidget>

class CoreScene;

class GUIScene : public QGraphicsScene
{
  public:
	explicit GUIScene(const QRectF &rect = QRectF(), CoreScene *coreScene = nullptr, int gridSize = 8);
	~GUIScene() noexcept = default;

	CoreScene *coreScene() noexcept;
	void setCoreScene(CoreScene *coreScene) noexcept;

	int gridSize() const noexcept;
	void setGridSize(int gridSize) noexcept;

  protected:
	void drawBackground(QPainter *painter, const QRectF &rect) override;

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

  private:
	CoreScene *m_coreScene;
	int m_gridSize;
};

#endif /* LCSM_VISUAL_GUI_GUISCENE_H */
