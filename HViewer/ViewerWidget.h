#pragma once
#include <QWidget>
#include <osgQt/GraphicsWindowQt>
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QVBoxLayout>
#include <QDebug>

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osgQt/GraphicsWindowQt>

#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabBoxTrackballDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/Scale1DDragger>
#include <osgManipulator/Scale2DDragger>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/Translate1DDragger>
#include <osgManipulator/Translate2DDragger>
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/TranslatePlaneDragger>
#include <osgManipulator/RotateCylinderDragger>
#include <osgManipulator/AntiSquish>

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/io_utils>
#include <osg/ComputeBoundsVisitor>
#include <osg/MatrixTransform>
#include <osg/ClipNode>
#include <osg/Billboard>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Notify>
#include <osg/Material>
#include <osg/PolygonOffset>
#include <osg/PolygonMode>
#include <osg/LineStipple>
#include <osg/AnimationPath>
#include <osg/LightSource> 
#include <osg/Multisample>


#include "pickHandler.h"

#include "ClipCallback.h"



class ViewerWidget : public QWidget
{
public:
	ViewerWidget(osgQt::GraphicsWindowQt* gw);
	~ViewerWidget();

	void setScene(osg::Node*);
	osg::Node* getScene();

	void removeScene();
	void removeOperation();

	void changeToLineMode();
	void changeToSurfaceMode();

	void switchLayer(int index, bool targetStatus);
	void switchAllLayer(bool targetStatus);

	int getLayerCount();

	void createClipDragger();
	void removeClipDragger();
	void clearClip();

	void createPicker();
	void removePicker();

	void setBackColor(int r, int g, int b);

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

public:
	osg::Node* addDraggerToBox(osg::Node* box);

	osg::Transform* createClipBoxToScene(osg::Node* scene);


protected:
	// dragger for clipping
	osg::ref_ptr<osg::Geode> _boundingBox;
	osg::ref_ptr<osg::ShapeDrawable> _shape;

	osg::ref_ptr<osg::Transform> _clipTransform;

	osg::ref_ptr<osg::Transform> _boxTransform;

	ClipCallback* _clipCallback;
};

