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

	// 抗锯齿
	traits->samples = 16;

	return new osgQt::GraphicsWindowQt(traits.get());
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	// ViewerWidget
	osgQt::GraphicsWindowQt* gw = createGraphicsWindow(0, 0, 0, 0);
	_viewerWidget = new ViewerWidget(gw);
	//_viewerWidget->setGeometry(100, 100, 800, 600);
	_viewerWidget->setParent(this);
	this->setCentralWidget(_viewerWidget);
		
	//工具栏
	_toolBar = addToolBar(tr(""));
	_toolBar->setAllowedAreas(Qt::TopToolBarArea);

	//悬浮栏
	_treeToolBar = new QToolBar(this);
	this->addToolBar(Qt::LeftToolBarArea, _treeToolBar);
	_treeToolBar->setFloatable(false);
	//侧栏（层树）
	_treeWidget = new TreeWidget;
	_treeToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
	_treeToolBar->addWidget(_treeWidget);

	//左状态栏（文件名）
	QStatusBar* status = statusBar();
	setStatusBar(status);
	label_L = new QLabel("");
	status->addWidget(label_L);

	//右状态栏（坐标）
	label_R = new QLabel("");
	status->addPermanentWidget(label_R);
	connect(_viewerWidget, SIGNAL(labelRReturn(QString)), this, SLOT(setLabelR(QString)));

	QMenuBar *menuBar = new QMenuBar();
	QMenu* fileMenu = menuBar->addMenu(tr("文件"));
	setMenuBar(menuBar);
	QAction* openAction = fileMenu->addAction("打开");
	//fileMenu->addSeparator();
	//QAction* saveAction = fileMenu->addAction("Save");
	//QAction* saveasAction = fileMenu->addAction("Save as");
	QAction* closeAction = fileMenu->addAction("关闭");
	fileMenu->addSeparator();
	QAction* resetAction = fileMenu->addAction("重置");
	/*fileMenu->addSeparator();
	QAction* captureAction = fileMenu->addAction("截图");*/

	//QMenu* pathMenu = menuBar->addMenu("路径漫游");
	//QAction* addPathAction = pathMenu->addAction("添加路径");
	//QAction* removePathAction = pathMenu->addAction("移除路径");
	//pathMenu->addSeparator();
	//QAction* playPathAction = pathMenu->addAction("开始");
	
	//pathMenu->addSeparator();
	//QAction* openPathAction = pathMenu->addAction("Open Path");
	//QAction* savePathAction = pathMenu->addAction("Save Path");

	QMenu* modeMenu = menuBar->addMenu("显示模式");
	QAction* surfaceAction = modeMenu->addAction("面模式");
	QAction* lineAction = modeMenu->addAction("线模式");

	QMenu* ClipMenu = menuBar->addMenu("动态剖切");
	QAction* addDraggerAction = ClipMenu->addAction("添加剖切器");
	QAction* removeDraggerAction = ClipMenu->addAction("移除剖切器");
	ClipMenu->addSeparator();
	QAction* clearDraggerAction = ClipMenu->addAction("重置");

	QMenu* WindowMenu = menuBar->addMenu("窗口");
	QAction* showLayerAction = WindowMenu->addAction("显示层次");
	QAction* hideLayerAction = WindowMenu->addAction("隐藏层次");
	WindowMenu->addSeparator();
	QAction* showToolBarAction = WindowMenu->addAction("显示工具");
	QAction* hideToolBarAction = WindowMenu->addAction("隐藏工具");

	QMenu *preMenu = menuBar->addMenu(tr("自定义"));
	QAction* backColorAction = preMenu->addAction("背景颜色");
	//QAction* languageAction = preMenu->addAction("语言");

	QMenu* helpMenu = menuBar->addMenu("帮助");
	QAction* aboutAction = helpMenu->addAction("关于");

	

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

			//获取层信息
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
		setLabelR("");
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
	 

	//connect(addPathAction, &QAction::triggered, [=]() {
	//	_viewerWidget->createPicker();
	//});
	//connect(playPathAction, &QAction::triggered, [=]() {
	//	_viewerWidget->createPicker();
	//});
	//connect(removePathAction, &QAction::triggered, [=]() {
	//	_viewerWidget->createPicker();
	//});

	
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

	//QIcon addPathIcon("./Resources/path_add.ico");
	//addPathAction->setIcon(addPathIcon);
	//QIcon removePathIcon("./Resources/path_remove.ico");
	//removePathAction->setIcon(removePathIcon);
	//QIcon playPathIcon("./Resources/path_play.ico");
	//playPathAction->setIcon(playPathIcon);
	

	//_toolbar
	_toolBar->addAction(openAction);
	_toolBar->addAction(closeAction);

	_toolBar->addAction(resetAction);

	_toolBar->addAction(addDraggerAction);
	_toolBar->addAction(removeDraggerAction);
	
	_toolBar->addAction(surfaceAction);
	_toolBar->addAction(lineAction);
	
	//_toolBar->addAction(addPathAction);
	//_toolBar->addAction(removePathAction);
	//_toolBar->addAction(playPathAction);

	_toolBar->insertSeparator(resetAction);
	_toolBar->insertSeparator(addDraggerAction);
	_toolBar->insertSeparator(surfaceAction);
	//_toolBar->insertSeparator(addPathAction);

	this->resize(1366, 768);
}

MainWindow::~MainWindow()
{
}


void MainWindow::setStatusText(QString str)
{
	label_L->setText(str);
}

//void MainWindow::setVerticeText(QString str)
//{
//	label_R->setText(str);
//}


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

void MainWindow::setLabelR(QString str)
{
	label_R->setText(str);
}
