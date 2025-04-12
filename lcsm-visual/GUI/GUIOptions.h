#ifndef LCSM_VISUAL_GUI_GUIOPTIONS_H
#define LCSM_VISUAL_GUI_GUIOPTIONS_H

#include <QRectF>

class GUIOptions
{
  public:
	GUIOptions(int height = 0, int width = 0, int gridSize = 0) noexcept;
	~GUIOptions() noexcept = default;

	GUIOptions(const GUIOptions &other) noexcept;
	GUIOptions(GUIOptions &&other) noexcept;

	GUIOptions &operator=(const GUIOptions &other) noexcept;
	GUIOptions &operator=(GUIOptions &&other) noexcept;

	void swap(GUIOptions &other) noexcept;

	int height() const noexcept;
	void setHeight(int height) noexcept;

	int width() const noexcept;
	void setWidth(int width) noexcept;

	int gridSize() const noexcept;
	void setGridSize(int gridSize) noexcept;

	QRectF sceneRect() const noexcept;

  private:
	int m_height;
	int m_width;
	int m_gridSize;
};

#endif /* LCSM_VISUAL_GUI_GUIOPTIONS_H */
