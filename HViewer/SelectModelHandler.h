#pragma once

#include "pickHandler.h"


class SelectModelHandler : public PickHandler
{
public:
	SelectModelHandler() : _lastDrawable(0) 
	{
		normalColor = osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f);
		selectedColor = osg::Vec4(1.0f, 0.0f, 0.0f, 0.5f);
	}
	
	virtual void doUserOperations(osgUtil::LineSegmentIntersector::Intersection& result)
	{
		if (_lastDrawable.valid())
		{
			setDrawableColor(_lastDrawable.get(), normalColor);
			_lastDrawable = NULL;
		}

		osg::Geometry* geom = dynamic_cast<osg::Geometry*>(result.drawable.get());

		if (geom)
		{
			setDrawableColor(geom, selectedColor);
			_lastDrawable = geom; 
		}

	}
	//
	void setDrawableColor(osg::Geometry* geom, const osg::Vec4& color)
	{
	//	//osg::Vec4Array* colors = dynamic_cast<osg::Vec4Array*>(geom->getColorArray());

	//	//if (colors && colors->size() > 0)
	//	//{
	//	//	colors->front() = color;
	//	//	colors->dirty();
	//	//}

		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
		(*colors)[0] = selectedColor;

		geom->setColorArray(colors);
	}

protected:
	osg::observer_ptr<osg::Geometry> _lastDrawable;

	osg::Vec4 normalColor;
	osg::Vec4 selectedColor;
};