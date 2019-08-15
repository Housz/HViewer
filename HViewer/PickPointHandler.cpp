#include "PickPointHandler.h"

osg::Geode * PickPointHandler::createPointSelector()
{
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
	(*colors)[0] = selectedColor;

	_selector = new osg::Geometry;
	_selector->setDataVariance(osg::Object::DYNAMIC);
	_selector->setUseDisplayList(false);
	_selector->setUseVertexBufferObjects(true);
	_selector->setVertexArray(new osg::Vec3Array(1));
	_selector->setColorArray(colors.get());
	_selector->setColorBinding(osg::Geometry::BIND_OVERALL);
	_selector->addPrimitiveSet(new osg::DrawArrays(GL_POINTS, 0, 1));

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(_selector.get());
	geode->getOrCreateStateSet()->setAttributeAndModes(new osg::Point(10.0f));
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	return geode.release();
}

void PickPointHandler::doUserOperations(osgUtil::LineSegmentIntersector::Intersection & result)
{
	osg::Geometry* geom = dynamic_cast<osg::Geometry*>(result.drawable.get());
	if (!geom || !_selector || geom == _selector) return;

	osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
	osg::Vec3Array* selVertices = dynamic_cast<osg::Vec3Array*>(_selector->getVertexArray());
	if (!vertices || !selVertices) return;

	osg::Vec3 point = result.getWorldIntersectPoint();
	osg::Matrix matrix = osg::computeLocalToWorld(result.nodePath);

	osg::Matrix vpMatrix;
	if (_camera.valid())
	{
		vpMatrix = _camera->getViewMatrix() * _camera->getProjectionMatrix();
		point = point * vpMatrix;
	}

	const std::vector<unsigned int>& selIndices = result.indexList;
	for (unsigned int i = 0; i < 3 && i < selIndices.size(); ++i)
	{
		unsigned int pos = selIndices[i];
		osg::Vec3 vertex = (*vertices)[pos] * matrix;
		float distance = (vertex * vpMatrix - point).length();
		if (distance < 0.1f)
		{
			selVertices->front() = vertex;

			vertex = osg::Matrix::rotate(osg::Quat(-osg::PI_2, osg::X_AXIS)) * vertex;

			QString strVer = "X: ";
			strVer += QString::number(vertex.x(), 'f', 4);
			strVer += ", Y: ";
			strVer += QString::number(vertex.y(), 'f', 4);
			strVer += ", Z:";
			strVer += QString::number(vertex.z(), 'f', 4);

			qDebug() << vertex.x() << " " << vertex.y() << " " << vertex.z() << endl;

			//_label->setText(strVer);
			//_mainWindow->setVerticeText(strVer);

			emit _viewerWidget->labelRReturn(strVer);

		}
	}
	selVertices->dirty();
	_selector->dirtyBound();
}
