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
