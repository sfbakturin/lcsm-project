#ifndef LCSM_VIS_FORMS_MAINWINDOW_H
#define LCSM_VIS_FORMS_MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

  public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow() noexcept;

  private slots:
	void onClosing();

  private:
	Ui::MainWindow *ui;
};

#endif	  // LCSM_VIS_FORMS_MAINWINDOW_H
