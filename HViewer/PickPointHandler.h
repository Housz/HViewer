#pragma once
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Point>
#include <osg/PolygonOffset>
#include <osgUtil/SmoothingVisitor>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Switch>
#include <osgdb/readfile>
#include <Qstring>
#include <QLabel>
#include <QDebug> 

//#include "MainWindow.h"

#include "ViewerWidget.h"

#include "PickHandler.h"

const osg::Vec4 normalColor(1.0f, 1.0f, 1.0f, 1.0f);
const osg::Vec4 selectedColor(1.0f, 0.0f, 0.0f, 1.0f);

class ViewerWidget;

class PickPointHandler : public PickHandler
{
public:
	PickPointHandler(osg::Camera* camera, ViewerWidget* vw) : _selector(0), _camera(camera), _viewerWidget(vw) {}

	osg::Geode* createPointSelector();

	virtual void doUserOperations(osgUtil::LineSegmentIntersector::Intersection& result);

protected:
	osg::ref_ptr<osg::Geometry> _selector;
	osg::observer_ptr<osg::Camera> _camera;

	ViewerWidget* _viewerWidget;
};