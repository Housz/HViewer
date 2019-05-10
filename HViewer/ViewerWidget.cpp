#include "ViewerWidget.h"

ViewerWidget::ViewerWidget(osgQt::GraphicsWindowQt* gw) : QWidget(), _gw(gw), _scene(NULL)
{
	_viewer = new osgViewer::Viewer;
	_gw = gw;

	setScene(NULL);
	//_viewer->setSceneData(osgDB::readNodeFile("C:\\Users\\Housz\\Desktop\\obj\\test.3dt"));

	_root = new osg::Group;
	_clipCallback = NULL;
	//_root->addChild(_scene);

	//osg::ref_ptr<PickHandler> picker = new PickHandler;
	//_root->addChild(picker->getOrCreateSelectionBox());
	//_viewer->addEventHandler(picker.get());

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(_gw->getGLWidget());
	setLayout(layout);
	layout->setContentsMargins(1, 1, 1, 1);

	connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
	_timer.start(10);
}

//osg::Node* createLightSource()
//{
//	osg::ref_ptr<osg::Light> light = new osg::Light;
//	light->setLightNum(0);
//	light->setDiffuse(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
//	light->setPosition(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
//
//	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource;
//	lightSource->setLight(light);
//
//	return lightSource.release();
//
//	osg::ref_ptr<osg::MatrixTransform> sourceTrans = new osg::MatrixTransform;
//	sourceTrans->setMatrix(osg::Matrix::translate(osg::Vec3(-20.0f, 0.0f, 0.0f)));
//	sourceTrans->addChild(lightSource.get());
//
//	return sourceTrans.release();
//}


ViewerWidget::~ViewerWidget()
{

}

void ViewerWidget::setScene(osg::Node* root)
{
	removeScene();

	if (root)
	{
		_scene = root;

		_root->removeChild(0, _root->getNumChildren());

		_root->addChild(_scene);

		//light
		//_root->addChild(createLightSource());

		_viewer->setSceneData(_root);
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
}

osg::Node* ViewerWidget::getScene()
{
	return _scene;
}


void ViewerWidget::removeScene()
{
	_scene = NULL;
	_viewer->setSceneData(NULL);

	//removeClipDragger();
}

void ViewerWidget::removeOperation()
{
	_pathPickHandler->removeBoxes();
	_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
}


void ViewerWidget::changeToLineMode()
{
	if (_scene)
	{
		_viewer->getSceneData()->asGroup()->getOrCreateStateSet()
			->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
	}
}

void ViewerWidget::changeToSurfaceMode()
{
	if (_scene)
	{
		_viewer->getSceneData()->asGroup()->getOrCreateStateSet()
			->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL));
	}
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


osg::Node * ViewerWidget::addDraggerToBox(osg::Node * box)
{
	osg::ref_ptr<osg::Group> root = new osg::Group;

	box->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

	osg::MatrixTransform* transform = new osg::MatrixTransform;
	transform->addChild(box);

	osgManipulator::Dragger* dragger = new osgManipulator::TabBoxDragger();
	dragger->setupDefaultGeometry();

	root->addChild(transform);
	root->addChild(dragger);

	osg::ComputeBoundsVisitor boundvisitor;
	box->accept(boundvisitor);
	osg::BoundingBox bb = boundvisitor.getBoundingBox();

	double x = bb.xMax() - bb.xMin();
	double y = bb.yMax() - bb.yMin();
	double z = bb.zMax() - bb.zMin();
	osg::Vec3 center = osg::Vec3((bb.xMax() + bb.xMin()) / 2, (bb.yMax() + bb.yMin()) / 2, (bb.zMax() + bb.zMin()) / 2);

	//const double scale = 1.155;
	const double offset = 0.001;
	dragger->setMatrix(osg::Matrix::scale(x + offset, y + offset, z + offset) * osg::Matrix::translate(center));

	dragger->addTransformUpdating(transform);
	dragger->setHandleEvents(true);

	dragger->setActivationModKeyMask(osgGA::GUIEventAdapter::MODKEY_CTRL);
	//dragger->setActivationKeyEvent('a');

	return root.release();
}

osg::Transform * ViewerWidget::createClipBoxToScene(osg::Node * scene)
{
	_boundingBox = new osg::Geode;
	osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform;

	osg::ComputeBoundsVisitor boundvisitor;
	scene->accept(boundvisitor);
	osg::BoundingBox bb = boundvisitor.getBoundingBox();

	double x = bb.xMax() - bb.xMin();
	double y = bb.yMax() - bb.yMin();
	double z = bb.zMax() - bb.zMin();
	osg::Vec3 center = osg::Vec3((bb.xMax() + bb.xMin()) / 2, (bb.yMax() + bb.yMin()) / 2, (bb.zMax() + bb.zMin()) / 2);

	_shape = new osg::ShapeDrawable(new osg::Box(center, x, y, z));
	_shape->setColor(osg::Vec4(0.5f, 0.5f, 0.7f, 0.3f));
	_shape->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);//ÉèÖÃÍ¸Ã÷¶È

	_boundingBox->addDrawable(_shape.get());

	transform->addChild(addDraggerToBox(_boundingBox.get()));

	return transform.release();
}


void ViewerWidget::createClipDragger()
{
	if (_root->getNumChildren() == 1)
	{
		_clipTransform = new osg::Transform;
		_root->addChild(_clipTransform);

		_boxTransform = createClipBoxToScene(_scene);
		_root->addChild(_boxTransform);

		if (_clipCallback)
		{
			_scene->removeUpdateCallback(_clipCallback);
		}

		_clipCallback = new ClipCallback(_clipTransform, _boxTransform);
		_scene->setUpdateCallback(_clipCallback);
	}
	else
	{
		return;
	}
}

void ViewerWidget::removeClipDragger()
{
	if (_root->getNumChildren() > 1)
	{
		_scene->removeUpdateCallback(_clipCallback);

		_root->removeChild(_root->getChildIndex(_boxTransform));
		_root->removeChild(_root->getChildIndex(_clipTransform));

		//_root->removeChild(1, _root->getNumChildren() - 1);

		_boundingBox = NULL;
		_shape = NULL;
		_clipTransform = NULL;
		_boxTransform = NULL;
	}
	else
	{
		return;
	}
}

void ViewerWidget::clearClip()
{
	_root->removeChild(0, _root->getNumChildren());

	_root->addChild(_scene);
}


void ViewerWidget::createPicker()
{
	_viewer->addEventHandler(_pathPickHandler);
}

void ViewerWidget::removePicker()
{
	_viewer->removeEventHandler(_pathPickHandler);
}

void ViewerWidget::setBackColor(int r, int g, int b)
{
	osg::Camera* camera = _viewer->getCamera();
	camera->setClearColor(osg::Vec4(r, g, b, 1.0));
}
