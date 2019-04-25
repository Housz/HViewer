#pragma once

#include <osg/Callback>
#include <osg/ClipNode>
#include <osg/ComputeBoundsVisitor>
#include <osg/Transform>

class ClipCallback : public osg::NodeCallback
{
public:
	ClipCallback();
	ClipCallback(osg::Transform* _clipTransform, osg::Transform* _boxTransform);


	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

	~ClipCallback();

protected:
	osg::Transform* clipTransform;

	osg::Transform* boxTransform;

};

