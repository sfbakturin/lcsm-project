// #include <QApplication>
// #include <QGraphicsRectItem>
// #include <QGraphicsScene>
// #include <QGraphicsView>
// #include <QPainter>
// #include <QPen>

// constexpr int GRID_SIZE = 32;
// constexpr int SCENE_WIDTH = 640;
// constexpr int SCENE_HEIGHT = 480;

// template< typename T >
// const T &clamp(const T &v, const T &lo, const T &hi)
// {
// 	return (v < lo) ? lo : (hi < v) ? hi : v;
// }

// class MyScene : public QGraphicsScene
// {
//   public:
// 	MyScene(QObject *parent = nullptr) : QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT, parent) {}

//   protected:
// 	void drawBackground(QPainter *painter, const QRectF &rect) override
// 	{
// 		Q_UNUSED(rect);
// 		QPen pen(Qt::gray);
// 		pen.setStyle(Qt::DotLine);
// 		pen.setWidth(2);
// 		painter->setPen(pen);

// 		for (int x = 0; x <= width(); x += GRID_SIZE)
// 			for (int y = 0; y <= height(); y += GRID_SIZE)
// 				painter->drawPoint(x, y);
// 	}
// };

// class GridItem : public QGraphicsRectItem
// {
//   public:
// 	GridItem(qreal x, qreal y, qreal w = GRID_SIZE, qreal h = GRID_SIZE) : QGraphicsRectItem(0, 0, w, h)
// 	{
// 		setBrush(Qt::blue);
// 		setFlag(ItemIsMovable);
// 		setFlag(ItemIsSelectable);
// 		setFlag(ItemSendsGeometryChanges);
// 		setPos(x, y);
// 	}

//   protected:
// 	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override
// 	{
// 		if (change == ItemPositionChange && scene())
// 		{
// 			QPointF newPos = value.toPointF();

// 			// Привязка к сетке
// 			qreal x = qRound(newPos.x() / GRID_SIZE) * GRID_SIZE;
// 			qreal y = qRound(newPos.y() / GRID_SIZE) * GRID_SIZE;

// 			// Ограничения по краям
// 			QRectF bounds = scene()->sceneRect();
// 			x = clamp(x, bounds.left(), bounds.right() - rect().width());
// 			y = clamp(y, bounds.top(), bounds.bottom() - rect().height());

// 			return QPointF(x, y);
// 		}
// 		return QGraphicsItem::itemChange(change, value);
// 	}
// };

// int main(int argc, char *argv[])
// {
// 	QApplication app(argc, argv);

// 	// Настраиваем сцену
// 	MyScene *scene = new MyScene;

// 	// Добавим несколько элементов
// 	for (int i = 0; i < 5; ++i)
// 	{
// 		GridItem *item = new GridItem(i * GRID_SIZE, i * GRID_SIZE);
// 		scene->addItem(item);
// 	}

// 	// Отображаем
// 	QGraphicsView *view = new QGraphicsView(scene);
// 	view->setRenderHint(QPainter::Antialiasing);
// 	view->setWindowTitle("Grid Map + Snap + Bounded");
// 	view->resize(700, 500);
// 	view->show();

// 	return app.exec();
// }

#include <View/MainWindow.h>

#include <QApplication>
#include <QCoreApplication>
#include <QMessageBox>
#include <QObject>
#include <QSettings>
#include <QString>
#include <stdexcept>

int main(int argc, char *argv[])
{
	static const QString MESSAGE_BOX_TITLE = QObject::tr("Application error");

	try
	{
		QApplication a(argc, argv);

		a.setApplicationName("LCSM Visual");

		MainWindow w;
		w.show();

		return a.exec();
	} catch (const std::runtime_error &err)
	{
		QMessageBox::critical(nullptr, MESSAGE_BOX_TITLE, QString("Runtime error: %1").arg(err.what()));
	} catch (const std::exception &err)
	{
		QMessageBox::critical(nullptr, MESSAGE_BOX_TITLE, QString("Exception: %1").arg(err.what()));
	} catch (...)
	{
		QMessageBox::critical(nullptr, MESSAGE_BOX_TITLE, QString("Unknown error encounted"));
	}
}

// #include <QApplication>
// #include <QBrush>
// #include <QGraphicsRectItem>
// #include <QGraphicsScene>
// #include <QGraphicsView>
// #include <QPen>

// class GridItem : public QGraphicsRectItem
// {
//   public:
// 	GridItem(qreal x, qreal y, qreal w, qreal h) : QGraphicsRectItem(x, y, w, h), gridSize(32)
// 	{
// 		setBrush(QBrush(Qt::green));
// 		setFlag(QGraphicsItem::ItemIsMovable);
// 		setFlag(QGraphicsItem::ItemIsSelectable);
// 		setFlag(QGraphicsItem::ItemSendsGeometryChanges);	 // важно для itemChange
// 	}

//   protected:
// 	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override
// 	{
// 		if (change == ItemPositionChange && scene())
// 		{
// 			QPointF newPos = value.toPointF();
// 			// Привязываем позицию к сетке
// 			qreal x = qRound(newPos.x() / gridSize) * gridSize;
// 			qreal y = qRound(newPos.y() / gridSize) * gridSize;
// 			return QPointF(x, y);
// 		}
// 		return QGraphicsItem::itemChange(change, value);
// 	}

//   private:
// 	int gridSize;
// };

// int main(int argc, char *argv[])
// {
// 	QApplication app(argc, argv);

// 	QGraphicsScene *scene = new QGraphicsScene(0, 0, 640, 480);
// 	QGraphicsView *view = new QGraphicsView(scene);
// 	view->setRenderHint(QPainter::Antialiasing);
// 	view->setDragMode(QGraphicsView::RubberBandDrag);

// 	// Добавляем несколько объектов
// 	for (int i = 0; i < 5; ++i)
// 	{
// 		GridItem *item = new GridItem(i * 40, i * 40, 32, 32);
// 		scene->addItem(item);
// 	}

// 	view->setWindowTitle("Drag with Grid Snap");
// 	view->resize(700, 500);
// 	view->show();

// 	return app.exec();
// }
