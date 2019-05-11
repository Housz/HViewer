#pragma once

#include <osgGA/GUIEventHandler>
#include <osgViewer/View>

class PickHandler : public osgGA::GUIEventHandler
{
public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		if (ea.getEventType() != osgGA::GUIEventAdapter::RELEASE ||
			ea.getButton() != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ||
			!(ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_CTRL))
			return false;

		osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
		if (viewer)
		{
			osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
				new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
			osgUtil::IntersectionVisitor iv(intersector.get());
			viewer->getCamera()->accept(iv);

			if (intersector->containsIntersections())
			{
				osgUtil::LineSegmentIntersector::Intersection result = *(intersector->getIntersections().begin());
				doUserOperations(result);
			}
		}
		return false;
	}
	virtual void doUserOperations(osgUtil::LineSegmentIntersector::Intersection& result) = 0;
};
