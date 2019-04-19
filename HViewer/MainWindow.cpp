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
	QMenuBar *bar = menuBar();
	QMenu* fileMenu = bar->addMenu("File");
	setMenuBar(bar);
	QAction* openAction = fileMenu->addAction("Open");
	//fileMenu->addSeparator();
	//QAction* saveAction = fileMenu->addAction("Save");
	//QAction* saveasAction = fileMenu->addAction("Save as");
	fileMenu->addSeparator();
	QAction* closeAction = fileMenu->addAction("Close");
	fileMenu->addSeparator();
	QAction* homeAction = fileMenu->addAction("Home");


	QMenu* pathMenu = bar->addMenu("Path");
	QAction* palyAction = pathMenu->addAction("Play");
	QAction* recordAction = pathMenu->addAction("Record");
	pathMenu->addSeparator();
	QAction* openPathAction = pathMenu->addAction("Open Path");
	QAction* savePathAction = pathMenu->addAction("Save Path");

	QMenu* modeMenu = bar->addMenu("Mode");
	QAction* surfaceAction = modeMenu->addAction("Surface");
	QAction* lineAction = modeMenu->addAction("Line");

	QMenu* helpMenu = bar->addMenu("Help");
	QAction* aboutAction = helpMenu->addAction("About");


	connect(homeAction, &QAction::triggered, [=]() {
		_viewerWidget->removeOperation();
	});

	connect(aboutAction, &QAction::triggered, [=]() {
		qDebug() << "pop dialog";

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
			}
			else
			{
				initLayerList(NULL);
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



	//悬浮栏
	QToolBar* toolBar = new QToolBar(this);
	this->addToolBar(Qt::LeftToolBarArea, toolBar);
	toolBar->setFloatable(false);

	_treeWidget = new TreeWidget;
	toolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
	toolBar->addWidget(_treeWidget);

	//左状态栏（文件名）
	QStatusBar* status = statusBar();
	setStatusBar(status);
	label_L = new QLabel("");
	status->addWidget(label_L);

	//右状态栏（进度）
	//QLabel* label_L = new QLabel(tr("left info"));
	//status->addWidget(label_L);
	//QLabel* label_R = new QLabel("right info");
	//status->addPermanentWidget(label_R);


	osgQt::GraphicsWindowQt* gw = createGraphicsWindow(0,0,0,0);
	_viewerWidget = new ViewerWidget(gw);
	//_viewerWidget->setGeometry(100, 100, 800, 600);

	_viewerWidget->setParent(this);
	this->setCentralWidget(_viewerWidget);

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

