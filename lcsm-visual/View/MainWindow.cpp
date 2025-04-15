#include <Core/CoreScene.h>
#include <Core/Library.h>
#include <Core/Project.h>
#include <GUI/GUIView.h>
#include <Items/Item.h>
#include <View/DesignExplorerList.h>
#include <View/MainWindow.h>
#include <View/PropertiesList.h>

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QGraphicsView>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QKeySequence>
#include <QLabel>
#include <QLayout>
#include <QListView>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSize>
#include <QSplitter>
#include <QString>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>

static constexpr QSize DEFAULT_WINDOW_SIZE(640, 480);

static constexpr unsigned DESIGN_EXPLORER_LIST_OPTION_BASE =
	DesignExplorerList::ListOption::AddeableToCircuit | DesignExplorerList::ListOption::Removeable;
static constexpr unsigned DESIGN_EXPLORER_OPTIONS_CIRCUIT = DESIGN_EXPLORER_LIST_OPTION_BASE | DesignExplorerList::ListOption::Openable;
static constexpr unsigned DESIGN_EXPLORER_OPTIONS_VERILOG = DESIGN_EXPLORER_LIST_OPTION_BASE;
static constexpr unsigned DESIGN_EXPLORER_OPTIONS_LIBRARY = DesignExplorerList::ListOption::AddeableToCircuit;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	// Initialize main widget.
	initWindow();

	// Initialize menu bar.
	initMenu();

	// Deactivate workspace, as there is no open project.
	setWorkspace(false);

	// Deactivate lists, as there is no open project.
	m_designProjectCircuitList->setAddToSceneEnabled(false);
	m_designProjectVerilogList->setAddToSceneEnabled(false);
	m_designLibraryList->setAddToSceneEnabled(false);
}

void MainWindow::onClosing()
{
	this->close();
}

void MainWindow::onStart(bool triggered)
{
	// Unselect all.
	m_mapView->coreScene()->aboutToBeConnected(false);
	m_mapView->coreScene()->setSelected(false);

	// Kill properties.
	m_designPropertiesList->clearAll();

	if (triggered)
	{
		// Activate actions.
		m_resetAction->setEnabled(true);
		m_stepAction->setEnabled(true);
		m_loopAction->setEnabled(true);

		// Deactivate design.
		setEnabledDesign(false);
	}
	else
	{
		// Reset checked.
		m_startAction->setChecked(false);
		m_loopAction->setChecked(false);

		// Deactivate actions.
		m_resetAction->setEnabled(false);
		m_stepAction->setEnabled(false);
		m_loopAction->setEnabled(false);

		// Activate design.
		setEnabledDesign(true);
	}

	// Start/stop simulate.
	m_mapView->coreScene()->startSimulate(triggered);
}

void MainWindow::onReset()
{
	onStart(false);
}

void MainWindow::onStep()
{
	m_mapView->coreScene()->stepSimulate();
}

void MainWindow::onLoop(bool triggered)
{
	m_mapView->coreScene()->loopSimulate(triggered);
}

void MainWindow::onCreateEmptyCircuit()
{
	bool ok;
	QString name = QInputDialog::getText(this, tr("Create empty circuit..."), tr("Circuit name:"), QLineEdit::Normal, "", &ok);
	if (ok && !name.isEmpty())
	{
		if (m_project->isCircuitExists(name))
		{
			QMessageBox::critical(nullptr, tr("Error creating circuit"), tr("Can't create circuit with same existing name"));
		}
		else
		{
			const QString added = m_project->createEmptyCircuit(name);
			m_designProjectCircuitList->add(added);
		}
	}
	else
	{
		throw std::logic_error("Attempt to create circuit with invalid name");
	}
}

void MainWindow::onImportCircuitFromFile()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Import circuit from file..."), QDir::homePath());
	if (!filename.isEmpty())
	{
		const QString added = m_project->importCircuitFromFile(filename);
		m_designProjectCircuitList->add(added);
	}
	else
	{
		throw std::logic_error("Attempt to import circuit with invalid path");
	}
}

void MainWindow::onImportVerilogModule()
{
	QString filename =
		QFileDialog::getOpenFileName(this, tr("Import Verilog module..."), QDir::homePath(), tr("Verilog/SystemVerilog (*.v *.sv)"));
	if (!filename.isEmpty())
	{
		const QString added = m_project->importVerilogModule(filename);
		m_designProjectVerilogList->add(added);
	}
	else
	{
		throw std::logic_error("Attempt to import Verilog module with invalid path");
	}
}

void MainWindow::onCreateVerilogModuleFromText()
{
	bool ok;
	QString text = QInputDialog::getMultiLineText(this, tr("Create Verilog module from text..."), tr("Text:"), "", &ok);
	if (ok && !text.isEmpty())
	{
		const QString added = m_project->createVerilogModuleFromText(text);
		m_designProjectVerilogList->add(added);
	}
}

void MainWindow::onOpen(const QString &name)
{
	// Set simulate tools activated.
	setEnabledSimulate(true);
	// Open circuit.
	if (m_mapView->coreScene())
	{
		disconnect(m_mapView->coreScene(), SIGNAL(showItem(Item *)));
		disconnect(m_mapView->coreScene(), SIGNAL(removeItem()));
	}
	m_mapView->setCoreScene(std::addressof(m_project->getSceneOf(name)));
	connect(m_mapView->coreScene(), SIGNAL(showItem(Item *)), SLOT(onShowItem(Item *)));
	connect(m_mapView->coreScene(), SIGNAL(removeItem()), SLOT(onRemoveItem()));
	// Set design list activate.
	m_designProjectCircuitList->setAddToSceneEnabled(true);
	m_designProjectVerilogList->setAddToSceneEnabled(true);
	m_designLibraryList->setAddToSceneEnabled(true);
	// Set selected name.
	m_mapName->setText(name);
}

void MainWindow::onAddToSceneCircuit(const QString &name)
{
	CoreScene *coreScene = m_mapView->coreScene();
	coreScene->add(m_project->getCircuitOf(name));
}

void MainWindow::onAddToSceneVerilog(const QString &name)
{
	CoreScene *coreScene = m_mapView->coreScene();
	coreScene->add(m_project->getVerilogOf(name));
}

void MainWindow::onAddToSceneLibrary(const QString &name)
{
	CoreScene *coreScene = m_mapView->coreScene();
	coreScene->add(Library::getItem(name, coreScene));
}

void MainWindow::onRemove(const QString &item)
{
	// TODO: unset and deactivate simulate tools only when item is opened.
	m_mapView->unsetCoreScene();
	setEnabledSimulate(false);
	m_designProjectCircuitList->setAddToSceneEnabled(false);
	m_designProjectVerilogList->setAddToSceneEnabled(false);
	m_designLibraryList->setAddToSceneEnabled(false);
	m_mapName->setText(tr("not selected"));
	// Remove from project.
	m_project->removeCircuitOrVerilog(item);
}

void MainWindow::onShowItem(Item *item)
{
	m_designPropertiesList->setCurrentItem(item);
}

void MainWindow::onRemoveItem()
{
	m_designPropertiesList->clearAll();
}

void MainWindow::onNewProject()
{
	bool ok;
	QString name = QInputDialog::getText(this, tr("New project..."), tr("Project name:"), QLineEdit::Normal, "", &ok);
	if (ok && !name.isEmpty())
	{
		// Create new project.
		m_project = std::unique_ptr< Project >(new Project(name));
		// Activate workspace, except the simulate tools.
		setEnabledDesign(true);
		setEnabledMenu(true);
		setTitle(true);
	}
}

void MainWindow::onOpenProject()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open project..."), QDir::homePath());
	if (!filename.isEmpty())
	{
		// Open existing project.
		m_project = Project::fromFile(filename);
		// Activate workspace, except the simulate tools.
		setEnabledDesign(true);
		setEnabledMenu(true);
		setTitle(true);
	}
}

void MainWindow::onCloseProject()
{
	// Reset project.
	m_project.reset();
	// Deactivate workspace.
	setWorkspace(false);
}

void MainWindow::onSaveProject()
{
	if (!m_project->isSavePointDetermined())
	{
		QString filename = QFileDialog::getSaveFileName(this, tr("Save project..."), QDir::homePath());
		if (filename.isEmpty())
		{
			return;
		}
		m_project->setFilename(filename);
		setTitle(true);
	}
	m_project->save();
}

void MainWindow::onAbout() {}

void MainWindow::initWindow()
{
	initWindowWidgets();
	initWindowSetup();
}

void MainWindow::initWindowWidgets()
{
	// === WINDOW WIDGET and LAYOUT ===
	QWidget *windowWidget = new QWidget();
	QHBoxLayout *windowLayout = new QHBoxLayout(windowWidget);

	// === LEFT PANEL ===
	{
		// Left panel's splitter.
		QSplitter *splitter = new QSplitter(Qt::Orientation::Vertical, windowWidget);

		// Left panel: design explorer tab.
		m_designExplorerTab = new QTabWidget(splitter);

		// Left panel: design explorer - select Project widget and layout.
		QWidget *designExplorerProjectWidget = new QWidget(m_designExplorerTab);
		QVBoxLayout *designExplorerProjectLayout = new QVBoxLayout(designExplorerProjectWidget);

		// Left panel: design explorer - select Project - list view of items in project and button to add.
		m_designExplorerProjectTab = new QTabWidget(designExplorerProjectWidget);

		m_designProjectCircuitList = new DesignExplorerList(DESIGN_EXPLORER_OPTIONS_CIRCUIT, m_designExplorerProjectTab);
		m_designProjectVerilogList = new DesignExplorerList(DESIGN_EXPLORER_OPTIONS_VERILOG, m_designExplorerProjectTab);

		connect(m_designProjectCircuitList, SIGNAL(open(QString)), SLOT(onOpen(QString)));
		connect(m_designProjectVerilogList, SIGNAL(open(QString)), SLOT(onOpen(QString)));
		connect(m_designProjectCircuitList, SIGNAL(addToScene(QString)), SLOT(onAddToSceneCircuit(QString)));
		connect(m_designProjectVerilogList, SIGNAL(addToScene(QString)), SLOT(onAddToSceneVerilog(QString)));
		connect(m_designProjectCircuitList, SIGNAL(remove(QString)), SLOT(onRemove(QString)));
		connect(m_designProjectVerilogList, SIGNAL(remove(QString)), SLOT(onRemove(QString)));

		m_designExplorerProjectTab->addTab(m_designProjectCircuitList, tr("Circuit"));
		m_designExplorerProjectTab->addTab(m_designProjectVerilogList, tr("Verilog"));

		m_designExplorerProjectButton = new QPushButton(designExplorerProjectWidget);
		m_designExplorerProjectButton->setText(tr("Add circuit..."));
		m_designExplorerProjectButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
		m_designExplorerProjectButton->setContextMenuPolicy(Qt::ContextMenuPolicy::ActionsContextMenu);

		m_designExplorerMenu = new QMenu(m_designExplorerProjectButton);
		m_designExplorerProjectButton->setMenu(m_designExplorerMenu);

		// Create empty circuit...
		m_createEmptyCircuitAction = m_designExplorerMenu->addAction(tr("Create empty circuit..."));

		// Import circuit from file...
		m_importCircuitFromFileAction = m_designExplorerMenu->addAction(tr("Import circuit from file..."));

		// Import Verilog module...
		m_importVerilogModuleAction = m_designExplorerMenu->addAction(tr("Import Verilog module..."));

		// Create Verilog module from text...
		m_createVerilogModuleFromTextAction = m_designExplorerMenu->addAction(tr("Create Verilog module from text..."));

		designExplorerProjectLayout->addWidget(m_designExplorerProjectTab);
		designExplorerProjectLayout->addWidget(m_designExplorerProjectButton);

		// Left panel: design exporer - select Library library and layout.
		QWidget *designExplorerLibraryWidget = new QWidget(m_designExplorerTab);
		QVBoxLayout *designExplorerLibraryLayout = new QVBoxLayout(designExplorerLibraryWidget);

		m_designLibraryList = new DesignExplorerList(DESIGN_EXPLORER_OPTIONS_LIBRARY, designExplorerLibraryWidget);

		connect(m_designLibraryList, SIGNAL(addToScene(QString)), SLOT(onAddToSceneLibrary(QString)));

		designExplorerLibraryLayout->addWidget(m_designLibraryList);

		m_designExplorerTab->setTabPosition(QTabWidget::TabPosition::West);
		m_designExplorerTab->addTab(designExplorerProjectWidget, tr("PROJECT"));
		m_designExplorerTab->addTab(designExplorerLibraryWidget, tr("LIBRARY"));

		// Left panel: properties.
		m_designPropertiesGroupBox = new QGroupBox(tr("Properties"), splitter);
		QVBoxLayout *designPropertiesGroupBoxLayout = new QVBoxLayout(m_designPropertiesGroupBox);

		m_designPropertiesList = new PropertiesList(m_designPropertiesGroupBox);
		designPropertiesGroupBoxLayout->addWidget(m_designPropertiesList);

		m_designPropertiesGroupBox->setLayout(designPropertiesGroupBoxLayout);

		splitter->addWidget(m_designExplorerTab);
		splitter->addWidget(m_designPropertiesGroupBox);
		splitter->setStretchFactor(0, 1);
		splitter->setStretchFactor(1, 1);

		windowLayout->addWidget(splitter, 0);
	}

	// === RIGHT PANEL ===
	{
		// Right panel's splitter.
		QSplitter *splitter = new QSplitter(Qt::Orientation::Vertical, windowWidget);

		// Right panel: tool bar and actions.
		m_mapToolBar = new QToolBar(splitter);

		// START
		m_startAction = m_mapToolBar->addAction(tr("START"));
		m_startAction->setCheckable(true);

		// RESET
		m_resetAction = m_mapToolBar->addAction(tr("RESET"));

		// STEP
		m_stepAction = m_mapToolBar->addAction(tr("STEP"));

		// LOOP
		m_loopAction = m_mapToolBar->addAction(tr("LOOP"));
		m_loopAction->setCheckable(true);

		// ---
		m_mapToolBar->addSeparator();

		// Naming.
		m_mapName = new QLabel(tr("<not selected>"), m_mapToolBar);
		m_mapToolBar->addWidget(m_mapName);

		// Right panel: main map.
		m_mapView = new GUIView(splitter);

		splitter->addWidget(m_mapToolBar);
		splitter->addWidget(m_mapView);
		splitter->setStretchFactor(0, 0);
		splitter->setStretchFactor(1, 1);

		windowLayout->addWidget(splitter, 1);
	}

	// Connections from panels.
	connect(m_startAction, SIGNAL(triggered(bool)), SLOT(onStart(bool)));
	connect(m_resetAction, SIGNAL(triggered(bool)), SLOT(onReset()));
	connect(m_stepAction, SIGNAL(triggered(bool)), SLOT(onStep()));
	connect(m_loopAction, SIGNAL(triggered(bool)), SLOT(onLoop(bool)));

	connect(m_createEmptyCircuitAction, SIGNAL(triggered(bool)), SLOT(onCreateEmptyCircuit()));
	connect(m_importCircuitFromFileAction, SIGNAL(triggered(bool)), SLOT(onImportCircuitFromFile()));
	connect(m_importVerilogModuleAction, SIGNAL(triggered(bool)), SLOT(onImportVerilogModule()));
	connect(m_createVerilogModuleFromTextAction, SIGNAL(triggered(bool)), SLOT(onCreateVerilogModuleFromText()));

	m_widget = std::unique_ptr< QWidget >(windowWidget);
}

void MainWindow::initWindowSetup()
{
	// Central widget and sizes.
	setCentralWidget(m_widget.get());
	setMinimumSize(DEFAULT_WINDOW_SIZE);
	// Setup default list.
	for (const QString &name : Library::DefaultList)
	{
		m_designLibraryList->add(name);
	}
}

void MainWindow::initMenu()
{
	initMenuFile();
	initMenuEdit();
	initMenuHelp();
}

void MainWindow::initMenuFile()
{
	QMenu *fileMenu = menuBar()->addMenu(tr("File"));

	// New project..
	m_newProjectAction = fileMenu->addAction(tr("&New project..."));
	m_newProjectAction->setShortcut(QKeySequence::New);
	m_newProjectAction->setToolTip(tr("Create a new LCSM project"));
	connect(m_newProjectAction, SIGNAL(triggered(bool)), SLOT(onNewProject()));

	// Open project...
	m_openProjectAction = fileMenu->addAction(tr("Open project..."));
	m_openProjectAction->setShortcut(QKeySequence::Open);
	m_openProjectAction->setToolTip(tr("Open existing LCSM project"));
	connect(m_openProjectAction, SIGNAL(triggered(bool)), SLOT(onOpenProject()));

	// ---
	fileMenu->addSeparator();

	// Save project
	m_saveProjectAction = fileMenu->addAction(tr("Save project"));
	m_saveProjectAction->setShortcut(QKeySequence::Save);
	m_saveProjectAction->setToolTip(tr("Save LCSM project"));
	connect(m_saveProjectAction, SIGNAL(triggered(bool)), SLOT(onSaveProject()));

	// Close project
	m_closeProjectAction = fileMenu->addAction(tr("Close project"));
	m_closeProjectAction->setShortcut(QKeySequence::Close);
	m_closeProjectAction->setToolTip(tr("Close LCSM project without saving"));
	connect(m_closeProjectAction, SIGNAL(triggered(bool)), SLOT(onCloseProject()));

	// ---
	fileMenu->addSeparator();

	// Exit
	QAction *fileMenuExit = fileMenu->addAction(tr("Exit"));
	fileMenuExit->setShortcut(QKeySequence::Quit);
	fileMenuExit->setToolTip(tr("Close application"));
	connect(fileMenuExit, SIGNAL(triggered(bool)), SLOT(close()));
}

void MainWindow::initMenuEdit()
{
	QMenu *edit = menuBar()->addMenu(tr("Edit"));

	// Undo
	m_undoAction = edit->addAction(tr("Undo"));
	m_undoAction->setShortcut(QKeySequence::Undo);

	// Redo
	m_redoAction = edit->addAction(tr("Redo"));
	m_redoAction->setShortcut(QKeySequence::Redo);
}

void MainWindow::initMenuHelp()
{
	QMenu *helpMenu = menuBar()->addMenu(tr("Help"));

	// About...
	QAction *helpMenuAbout = helpMenu->addAction(tr("About..."));
	connect(helpMenuAbout, SIGNAL(triggered(bool)), SLOT(onAbout()));
}

void MainWindow::setWorkspace(bool enabled)
{
	setEnabledSimulate(enabled);
	setEnabledDesign(enabled);
	setEnabledMenu(enabled);
	setTitle(enabled);
}

void MainWindow::setEnabledSimulate(bool enabled)
{
	if (enabled)
	{
		m_startAction->setEnabled(enabled);
		m_startAction->setChecked(false);
		m_loopAction->setChecked(false);
		m_mapName->setEnabled(enabled);
		return;
	}

	m_startAction->setEnabled(enabled);
	m_resetAction->setEnabled(enabled);
	m_stepAction->setEnabled(enabled);
	m_loopAction->setEnabled(enabled);
	m_mapName->setEnabled(enabled);
}

void MainWindow::setEnabledDesign(bool enabled)
{
	m_designExplorerTab->setEnabled(enabled);
	m_designPropertiesGroupBox->setEnabled(enabled);
}

void MainWindow::setEnabledMenu(bool enabled)
{
	m_saveProjectAction->setEnabled(enabled);
	m_closeProjectAction->setEnabled(enabled);
	m_undoAction->setEnabled(enabled);
	m_redoAction->setEnabled(enabled);
}

void MainWindow::setTitle(bool enabled)
{
	if (enabled)
	{
		const QString &filename = m_project->filename();
		setWindowTitle(QString("%1 [%2]").arg(m_project->name(), (filename.isEmpty() ? tr("<no filename>") : filename)));
	}
	else
	{
		setWindowTitle("LCSM Visual");
	}
}
