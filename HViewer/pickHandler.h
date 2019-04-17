#pragma once
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>
#include <osgGA/DriveManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgUtil/LineSegmentIntersector>
#include <osg/ShapeDrawable>
#include <osg/AnimationPath>
#include <osg/Quat>

#include <iostream>
#include <vector>

class PathPickHandler : public osgGA::GUIEventHandler
{
public:
	osg::Geode* createBox(osg::Vec3 center)
	{
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(new osg::ShapeDrawable(new osg::Box(center, 5, 5, 5)));
		return geode.release();
	}

	double getPlaytime(osg::Vec3 res, osg::Vec3 des)
	{
		double distance = sqrt((des.x() - res.x())*(des.x() - res.x()) + (des.y() - res.y())*(des.y() - res.y()));
		return distance * 0.05;
	}

public:
	PathPickHandler(osgViewer::Viewer *vt) : viewer(vt)
	{
		controls = new osg::Vec3Array;
	}

	osg::AnimationPath* createPath()
	{
		osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath;
		animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);

		double time = 0.0;
		double angle = 0.0;
		double roll = 1.57;


		if (controls.valid())
		{
			osg::Vec3Array::iterator iter = controls->begin();
			for (; ; )
			{
				osg::Vec3 position(*iter);
				iter++;

				if (iter != controls->end())
				{
					// 计算角度

					// 目的点在源点右边
					if (iter->x() > position.x())
					{
						angle = 1.57 - atan((iter->y() - position.y()) / (iter->x() - position.x()));
						if (angle < 0)
						{
							angle += 1.57;
						}
					}
					// 目的点在源点左边
					else
					{
						angle = -(1.57 + atan((iter->y() - position.y()) / (iter->x() - position.x())));
						if (angle > 0)
						{
							angle = -(1.57 - angle);
						}
					}
					osg::Quat rotation(osg::Quat(roll, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(-angle, osg::Vec3(0.0, 0.0, 1.0)));

					animationPath->insert(time, osg::AnimationPath::ControlPoint(position, rotation));
					time += getPlaytime(position, *iter);
				}
				else
				{
					break;
				}
			}
		}

		// 导出.path文件
		std::ofstream fout("C:\\Users\\Housz\\Desktop\\1.path");
		animationPath->write(fout);
		fout.close();

		return animationPath.release();
	}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::DOUBLECLICK:
		{
			if (viewer)
			{
				//申请一个存放交点的集合
				osgUtil::LineSegmentIntersector::Intersections inters;
				if (viewer->computeIntersections(ea.getX(), ea.getY(), inters))
				{
					osgUtil::LineSegmentIntersector::Intersections::iterator iter = inters.begin();

					std::cout << iter->getWorldIntersectPoint().x() << "  " << iter->getWorldIntersectPoint().y() << std::endl;

					osg::Vec3 tempP;
					tempP.set(iter->getWorldIntersectPoint().x(), iter->getWorldIntersectPoint().y(), iter->getWorldIntersectPoint().z() + 20);

					controls->push_back(tempP);

					osg::Geode* box = createBox(iter->getWorldIntersectPoint());
					viewer->getSceneData()->asGroup()->addChild(box);

					boxes.push_back(box);
				}
			}
		}
		break;

		case osgGA::GUIEventAdapter::KEYDOWN:
		{
			if (ea.getKey() == 'a' || ea.getKey() == 'A')
			{
				if (viewer)
				{
					osgGA::AnimationPathManipulator* apm = new osgGA::AnimationPathManipulator("C:\\Users\\Housz\\Desktop\\1.path");
					//osgGA::AnimationPathManipulator* apm = new osgGA::AnimationPathManipulator();
					//apm->setAnimationPath(createPath());
					viewer->setCameraManipulator(apm);
				}
			}
			break;
		}
		}

		return false;
	}

	void removeBoxes()
	{
		for (auto iter = boxes.begin(); iter != boxes.end(); iter++)
		{
			viewer->getSceneData()->asGroup()->removeChild(*iter);
		}
	}

private:
	osg::ref_ptr<osg::Vec3Array> controls;
	osgViewer::Viewer *viewer;

	std::vector<osg::Geode*> boxes;
};

//int main()
//{
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//	viewer->setSceneData(osgDB::readNodeFile("C:\\Users\\Housz\\Desktop\\obj\\test.3dt"));
//
//	viewer->addEventHandler(new PickHandler(viewer));
//	viewer->addEventHandler(new osgViewer::WindowSizeHandler);
//
//	//viewer->removeEventHandler(new osgViewer::WindowSizeHandler);
//
//
//	viewer->run();
//
//	return 0;
//}