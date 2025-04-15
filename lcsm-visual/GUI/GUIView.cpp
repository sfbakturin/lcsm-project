#include <Core/CoreScene.h>
#include <GUI/GUIScene.h>
#include <GUI/GUIView.h>

#include <QGraphicsView>
#include <QWidget>

GUIView::GUIView(QWidget *parent) : QGraphicsView(parent), m_coreScene(nullptr)
{
	setMouseTracking(true);
	setRenderHint(QPainter::RenderHint::Antialiasing, true);
	setDragMode(QGraphicsView::DragMode::RubberBandDrag);
	// setOptimizationFlags(QGraphicsView::OptimizationFlag::DontSavePainterState);
	setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::FullViewportUpdate);
	setTransformationAnchor(QGraphicsView::ViewportAnchor::AnchorUnderMouse);
}

void GUIView::setCoreScene(CoreScene *coreScene)
{
	if (m_coreScene)
	{
		setScene(nullptr);
		m_coreScene->setView(nullptr);
	}
	m_coreScene = coreScene;
	if (m_coreScene)
	{
		setScene(m_coreScene->scene());
		m_coreScene->scene()->setCoreScene(coreScene);
		m_coreScene->setView(this);
	}
}

void GUIView::unsetCoreScene()
{
	setCoreScene(nullptr);
}

CoreScene *GUIView::coreScene() noexcept
{
	return m_coreScene;
}
