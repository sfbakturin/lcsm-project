#include <Core/CoreScene.h>
#include <GUI/GUIScene.h>

#include <QGraphicsScene>
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QWidget>

GUIScene::GUIScene(const QRectF &rect, int gridSize) : QGraphicsScene(rect), m_coreScene(nullptr), m_gridSize(gridSize)
{
}

CoreScene *GUIScene::coreScene() noexcept
{
	return m_coreScene;
}

void GUIScene::setCoreScene(CoreScene *coreScene) noexcept
{
	m_coreScene = coreScene;
}

int GUIScene::gridSize() const noexcept
{
	return m_gridSize;
}

void GUIScene::setGridSize(int gridSize) noexcept
{
	m_gridSize = gridSize;
}

void GUIScene::drawBackground(QPainter *painter, const QRectF &)
{
	QPen pen(Qt::gray);
	pen.setStyle(Qt::DotLine);
	pen.setWidth(2);
	painter->setPen(pen);
	for (int x = 0; x <= width(); x += m_gridSize)
	{
		for (int y = 0; y <= height(); y += m_gridSize)
		{
			painter->drawPoint(x, y);
		}
	}
}
