#include "MainWindow.h"

osgQt::GraphicsWindowQt* createGraphicsWindow(int x, int y, int w, int h)
{
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->windowDecoration = false;
	traits->x = x;
	traits->y = y;
	traits->width = w;
	traits->height = h;
	traits->doubleBuffer = true;

	return new osgQt::GraphicsWindowQt(traits.get());
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	//¹¤¾ßÀ¸
	_toolBar = addToolBar(tr(""));
	_toolBar->setAllowedAreas(Qt::TopToolBarArea);

	//Ðü¸¡À¸
	_treeToolBar = new QToolBar(this);
	this->addToolBar(Qt::LeftToolBarArea, _treeToolBar);
	_treeToolBar->setFloatable(false);
	//²àÀ¸£¨²ãÊ÷£©
	_treeWidget = new TreeWidget;
	_treeToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
	_treeToolBar->addWidget(_treeWidget);


	//×ó×´Ì¬À¸£¨ÎÄ¼þÃû£©
	QStatusBar* status = statusBar();
	setStatusBar(status);
	label_L = new QLabel("");
	status->addWidget(label_L);

	//ÓÒ×´Ì¬À¸£¨½ø¶È£©
	//QLabel* label_L = new QLabel(tr("left info"));
	//status->addWidget(label_L);
	//QLabel* label_R = new QLabel("right info");
	//status->addPermanentWidget(label_R);



	QMenuBar *menuBar = new QMenuBar();
	QMenu* fileMenu = menuBar->addMenu(tr("File"));
	setMenuBar(menuBar);
	QAction* openAction = fileMenu->addAction("Open");
	//fileMenu->addSeparator();
	//QAction* saveAction = fileMenu->addAction("Save");
	//QAction* saveasAction = fileMenu->addAction("Save as");
	QAction* closeAction = fileMenu->addAction("Close");
	fileMenu->addSeparator();
	QAction* resetAction = fileMenu->addAction("Reset");
	fileMenu->addSeparator();
	QAction* captureAction = fileMenu->addAction("Capture");

	QMenu* pathMenu = menuBar->addMenu("Path");
	QAction* palyAction = pathMenu->addAction("Add");
	QAction* recordAction = pathMenu->addAction("Remove");
	//pathMenu->addSeparator();
	//QAction* openPathAction = pathMenu->addAction("Open Path");
	//QAction* savePathAction = pathMenu->addAction("Save Path");

	QMenu* modeMenu = menuBar->addMenu("Mode");
	QAction* surfaceAction = modeMenu->addAction("Surface");
	QAction* lineAction = modeMenu->addAction("Line");

	QMenu* ClipMenu = menuBar->addMenu("Clip");
	QAction* addDraggerAction = ClipMenu->addAction("Clip");
	QAction* removeDraggerAction = ClipMenu->addAction("Remove");
	ClipMenu->addSeparator();
	QAction* clearDraggerAction = ClipMenu->addAction("Clear");

	QMenu* WindowMenu = menuBar->addMenu("Window");
	QAction* showLayerAction = WindowMenu->addAction("Show Layers");
	QAction* hideLayerAction = WindowMenu->addAction("Hide Layers");
	WindowMenu->addSeparator();
	QAction* showToolBarAction = WindowMenu->addAction("Show Tools");
	QAction* hideToolBarAction = WindowMenu->addAction("Hide Tools");

	QMenu *preMenu = menuBar->addMenu("Preferences");
	QAction* backColorAction = preMenu->addAction("Back Color");
	QAction* languageAction = preMenu->addAction("Language");

	QMenu* helpMenu = menuBar->addMenu("Help");
	QAction* aboutAction = helpMenu->addAction("About");

	

	connect(resetAction, &QAction::triggered, [=]() {
		_viewerWidget->removeOperation();
	});

	connect(aboutAction, &QAction::triggered, [=]() {
		QDialog dialog(this);
		dialog.resize(300, 200);
		dialog.setWindowTitle(tr("about"));

		QLabel *label = new QLabel(&dialog);
		label->setText("Powered by Housz.");

		dialog.exec();
	});

	connect(openAction, &QAction::triggered, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
			"/home",
			tr("Scene (*.osg *.3dt *.obj)"));

		QTextCodec *code = QTextCodec::codecForName("GB2312");

		std::string name = code->fromUnicode(fileName).data();

		QFileInfo fileinfo = QFileInfo(fileName);

		if (!fileName.isEmpty())
		{
			_viewerWidget->setScene(osgDB::readNodeFile(name));

			//»ñÈ¡²ãÐÅÏ¢

			if (fileinfo.suffix() == "3dt")
			{
				setLayerList(_viewerWidget->getScene()->asGroup());
				_treeToolBar->show();
			}
			else
			{
				initLayerList(NULL);
				_treeToolBar->hide();
			}
		}

		setStatusText(fileName);
	});

	connect(closeAction, &QAction::triggered, [=]() {
		_viewerWidget->removeScene();
		setStatusText("");
		initLayerList(NULL);
	});

	connect(surfaceAction, &QAction::triggered, [=]() {
		_viewerWidget->changeToSurfaceMode();
	});

	connect(lineAction, &QAction::triggered, [=]() {
		_viewerWidget->changeToLineMode();
	});


	connect(addDraggerAction, &QAction::triggered, [=]() {
		_viewerWidget->createClipDragger();
	});

	connect(removeDraggerAction, &QAction::triggered, [=]() {
		_viewerWidget->removeClipDragger();
	});

	connect(clearDraggerAction, &QAction::triggered, [=]() {
		_viewerWidget->clearClip();
	});


	connect(showLayerAction, &QAction::triggered, [=]() {
		_treeToolBar->show();
	});

	connect(hideLayerAction, &QAction::triggered, [=]() {
		_treeToolBar->hide();
	});

	connect(showToolBarAction, &QAction::triggered, [=]() {
		_toolBar->show();
	});

	connect(hideToolBarAction, &QAction::triggered, [=]() {
		_toolBar->hide();
	});


	connect(backColorAction, &QAction::triggered, [=]() {
		QColorDialog colorDig(this);
		QColor c = colorDig.getRgba();

		_viewerWidget->setBackColor(c.red(), c.green(), c.blue());
	});
	 
	
	//ICON
	QStyle* style = QApplication::style();
	QIcon icon = style->standardIcon(QStyle::SP_DirHomeIcon);
	openAction->setIcon(icon);

	icon = style->standardIcon(QStyle::SP_MessageBoxCritical);
	closeAction->setIcon(icon);

	icon = style->standardIcon(QStyle::SP_BrowserReload);
	resetAction->setIcon(icon);

	icon = style->standardPixmap(QStyle::SP_DialogHelpButton);
	addDraggerAction->setIcon(icon);

	icon = style->standardPixmap(QStyle::SP_FileDialogListView);
	removeDraggerAction->setIcon(icon);

	QIcon lineIcon("./Resources/line_mode.ico");
	lineAction->setIcon(lineIcon);

	QIcon surfaceIcon("./Resources/surface_mode.ico");
	surfaceAction->setIcon(surfaceIcon);
	QIcon clipEnableIcon("./Resources/clip_enable.ico");
	addDraggerAction->setIcon(clipEnableIcon);
	QIcon clipRemoveIcon("./Resources/clip_remove.ico");
	removeDraggerAction->setIcon(clipRemoveIcon);

	osgQt::GraphicsWindowQt* gw = createGraphicsWindow(0,0,0,0);
	_viewerWidget = new ViewerWidget(gw);
	//_viewerWidget->setGeometry(100, 100, 800, 600);

	_viewerWidget->setParent(this);
	this->setCentralWidget(_viewerWidget);

	
	//_toolbar
	_toolBar->addAction(openAction);
	_toolBar->addAction(closeAction);

	_toolBar->addAction(resetAction);

	_toolBar->addAction(addDraggerAction);
	_toolBar->addAction(removeDraggerAction);
	
	_toolBar->addAction(surfaceAction);
	_toolBar->addAction(lineAction);
	
	_toolBar->insertSeparator(resetAction);
	_toolBar->insertSeparator(addDraggerAction);
	_toolBar->insertSeparator(surfaceAction);





	this->resize(1366, 768);
}

MainWindow::~MainWindow()
{
}


void MainWindow::setStatusText(QString str)
{
	label_L->setText(str);
}


void MainWindow::setLayerList(osg::Group* scene)
{
	initLayerList(scene);

	int layerCount = _viewerWidget->getLayerCount();
	_treeWidget->changeItem(layerCount);
}

void MainWindow::initLayerList(osg::Group* scene)
{
	if (scene)
	{
		_treeWidget->setScene(scene);
	}
	
	_treeWidget->changeItem(0);
}

