#include "ClipCallback.h"



ClipCallback::ClipCallback()
{
}

ClipCallback::ClipCallback(osg::Transform * _clipTransform, osg::Transform * _boxTransform) : clipTransform(_clipTransform), boxTransform(_boxTransform)
{
}

ClipCallback::~ClipCallback()
{
}


void ClipCallback::operator()(osg::Node * node, osg::NodeVisitor * nv)
{
	clipTransform->removeChildren(0, clipTransform->getNumChildren());

	osg::ref_ptr<osg::ClipNode> clipnode = new osg::ClipNode;

	osg::ComputeBoundsVisitor boundvisitor;
	boxTransform->accept(boundvisitor);
	osg::BoundingBox bb = boundvisitor.getBoundingBox();


	clipnode->createClipBox(bb);
	clipnode->setCullingActive(false);


	clipTransform->addChild(clipnode);

	node->setStateSet(clipnode->getStateSet());

	traverse(node, nv);
}
