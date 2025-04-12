#include <GUI/GUIOptions.h>
#include <lcsm/Support/Algorithm.hpp>

#include <QRectF>
#include <utility>

GUIOptions::GUIOptions(int height, int width, int gridSize) noexcept :
	m_height(height), m_width(width), m_gridSize(gridSize)
{
}

GUIOptions::GUIOptions(const GUIOptions &other) noexcept :
	m_height(other.m_height), m_width(other.m_width), m_gridSize(other.m_gridSize)
{
}

GUIOptions::GUIOptions(GUIOptions &&other) noexcept :
	m_height(other.m_height), m_width(other.m_width), m_gridSize(other.m_gridSize)
{
}

GUIOptions &GUIOptions::operator=(const GUIOptions &other) noexcept
{
	return lcsm::support::CopyAssign< GUIOptions >(this, other);
}

GUIOptions &GUIOptions::operator=(GUIOptions &&other) noexcept
{
	return lcsm::support::MoveAssign< GUIOptions >(this, std::move(other));
}

void GUIOptions::swap(GUIOptions &other) noexcept
{
	std::swap(m_height, other.m_height);
	std::swap(m_width, other.m_width);
	std::swap(m_gridSize, other.m_gridSize);
}

int GUIOptions::height() const noexcept
{
	return m_height;
}

void GUIOptions::setHeight(int height) noexcept
{
	m_height = height;
}

int GUIOptions::width() const noexcept
{
	return m_width;
}

void GUIOptions::setWidth(int width) noexcept
{
	m_width = width;
}

int GUIOptions::gridSize() const noexcept
{
	return m_gridSize;
}

void GUIOptions::setGridSize(int gridSize) noexcept
{
	m_gridSize = gridSize;
}

QRectF GUIOptions::sceneRect() const noexcept
{
	const qreal h = static_cast< qreal >(height());
	const qreal w = static_cast< qreal >(width());
	return QRectF(0.0, 0.0, w, h);
}
