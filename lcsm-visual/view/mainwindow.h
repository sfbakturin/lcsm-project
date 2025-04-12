#ifndef LCSM_VISUAL_VIEW_MAINWINDOW_H
#define LCSM_VISUAL_VIEW_MAINWINDOW_H

#include <Core/Project.h>
#include <GUI/GUIView.h>
#include <View/DesignExplorerList.h>

#include <QAction>
#include <QGraphicsView>
#include <QGroupBox>
#include <QLabel>
#include <QListView>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QTabWidget>
#include <QToolBar>
#include <QWidget>
#include <memory>

class Item;

class MainWindow : public QMainWindow
{
	Q_OBJECT

  public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow() noexcept = default;

  private:
	/* === PROJECT === */

	std::unique_ptr< Project > m_project;

	/* === MAIN === */

	std::unique_ptr< QWidget > m_widget;

	/* === DESIGN === */

	QTabWidget *m_designExplorerTab;
	QGroupBox *m_designPropertiesGroupBox;

	/* Design/Project */

	QTabWidget *m_designExplorerProjectTab;
	QPushButton *m_designExplorerProjectButton;

	QMenu *m_designExplorerMenu;
	QAction *m_createEmptyCircuitAction;
	QAction *m_importCircuitFromFileAction;
	QAction *m_importVerilogModuleAction;
	QAction *m_createVerilogModuleFromTextAction;

	/* Design/Project/Circuit */

	DesignExplorerList *m_designProjectCircuitList;

	/* Design/Project/Verilog */

	DesignExplorerList *m_designProjectVerilogList;

	/* Design/Library */

	DesignExplorerList *m_designLibraryList;

	/* === EDITOR/SIMULATE === */

	QToolBar *m_mapToolBar;
	GUIView *m_mapView;

	QAction *m_startAction;
	QAction *m_resetAction;
	QAction *m_stepAction;
	QAction *m_loopAction;

	QLabel *m_mapName;

	/* === MENU === */

	QAction *m_newProjectAction;
	QAction *m_openProjectAction;
	QAction *m_closeProjectAction;
	QAction *m_saveProjectAction;

	QAction *m_undoAction;
	QAction *m_redoAction;

	QAction *m_aboutAction;

  private slots:
	void onClosing();

	/* === SIMULATE === */

	void onStart(bool triggered);
	void onReset();
	void onStep();
	void onLoop(bool triggered);

	/* === DESIGN === */

	void onCreateEmptyCircuit();
	void onImportCircuitFromFile();
	void onImportVerilogModule();
	void onCreateVerilogModuleFromText();

	void onOpen(const QString &item);
	void onAddToSceneCircuit(const QString &name);
	void onAddToSceneVerilog(const QString &name);
	void onAddToSceneLibrary(const QString &name);
	void onRemove(const QString &item);

	void onShowItem(Item *item);

	/* === MENU === */

	void onNewProject();
	void onOpenProject();
	void onCloseProject();
	void onSaveProject();

	void onAbout();

  private:
	void initWindow();
	void initWindowWidgets();
	void initWindowSetup();

	void initMenu();
	void initMenuFile();
	void initMenuEdit();
	void initMenuHelp();

	void setWorkspace(bool enabled);

	void setEnabledSimulate(bool enabled);
	void setEnabledDesign(bool enabled);
	void setEnabledMenu(bool enabled);
	void setTitle(bool enabled);
};

#endif /* LCSM_VISUAL_VIEW_MAINWINDOW_H */
