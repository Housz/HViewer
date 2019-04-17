#pragma once
#include <QWidget>
#include <osgQt/GraphicsWindowQt>
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QVBoxLayout>

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osgQt/GraphicsWindowQt>

#include "pickHandler.h"

class ViewerWidget : public QWidget
{
public:
	ViewerWidget(osgQt::GraphicsWindowQt* gw);
	~ViewerWidget();

	void setScene(osg::Node*);
	void removeScene();
	void removeOperation();
	void changeToLineMode();
	void changeToSurfaceMode();

protected:	
	QTimer _timer;

	osg::ref_ptr<osgQt::GraphicsWindowQt> _gw;

	osg::ref_ptr<osgViewer::Viewer> _viewer;

	osg::ref_ptr<osg::Node> _scene;

	osg::ref_ptr<osg::Group> _root;

	osg::ref_ptr<PathPickHandler> _pathPickHandler;

	virtual void paintEvent(QPaintEvent* event)
	{
		_viewer->frame();
	}
};

