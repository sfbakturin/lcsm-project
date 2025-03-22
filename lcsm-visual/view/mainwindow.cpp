#include "ui_mainwindow.h"
#include <view/mainwindow.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow())
{
	ui->setupUi(this);
	connect(ui->actionExit, SIGNAL(triggered()), SLOT(onClosing()));
}

MainWindow::~MainWindow() noexcept
{
	delete ui;
}

void MainWindow::onClosing()
{
	this->close();
}
