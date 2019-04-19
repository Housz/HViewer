#include "ViewerWidget.h"

ViewerWidget::ViewerWidget(osgQt::GraphicsWindowQt* gw) : QWidget(), _gw(gw), _scene(NULL)
{
	_viewer = new osgViewer::Viewer;
	_gw = gw;

	setScene(NULL);
	//_viewer->setSceneData(osgDB::readNodeFile("C:\\Users\\Housz\\Desktop\\obj\\test.3dt"));

	//_root = new osg::Group;
	//_root->addChild(_scene);

	//osg::ref_ptr<PickHandler> picker = new PickHandler;
	//_root->addChild(picker->getOrCreateSelectionBox());
	//_viewer->addEventHandler(picker.get());

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(_gw->getGLWidget());
	setLayout(layout);
	layout->setContentsMargins(1, 1, 1, 1);

	connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
	_timer.start(10);
}


ViewerWidget::~ViewerWidget()
{

}

void ViewerWidget::setScene(osg::Node* root)
{
	removeScene();

	if (root)
	{
		_scene = root;
		_viewer->setSceneData(_scene);
	}

	const osg::GraphicsContext::Traits* traits = _gw->getTraits();

	osg::Camera* camera = _viewer->getCamera();
	camera->setGraphicsContext(_gw);
	camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective(
		30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 10000.0f);

	_viewer->addEventHandler(new osgViewer::StatsHandler);
	_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	_pathPickHandler = new PathPickHandler(_viewer);
	_viewer->addEventHandler(_pathPickHandler);
}


void ViewerWidget::removeScene()
{
	_scene = NULL;
	_viewer->setSceneData(NULL);
}

void ViewerWidget::removeOperation()
{
	_pathPickHandler->removeBoxes();
	_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
}


void ViewerWidget::changeToLineMode()
{
	_viewer->getSceneData()->asGroup()->getOrCreateStateSet()
		->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
}

void ViewerWidget::changeToSurfaceMode()
{
	_viewer->getSceneData()->asGroup()->getOrCreateStateSet()
		->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL));
}


void ViewerWidget::switchLayer(int index, bool targetStatus)
{
	_viewer->getSceneData()->asGroup()->getChild(index)->asSwitch()
		->setValue(0, targetStatus);
}

void ViewerWidget::switchAllLayer(bool targetStatus)
{
	int num = _viewer->getSceneData()->asGroup()->getNumChildren();
	for (size_t i = 0; i < num; i++)
	{
		switchLayer(i, targetStatus);
	}
}

int ViewerWidget::getLayerCount()
{
	return _scene->asGroup()->getNumChildren();
}
