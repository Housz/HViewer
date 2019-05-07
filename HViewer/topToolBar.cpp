#include "topToolBar.h"



topToolBar::topToolBar()
{
	QPushButton *openBtn = new QPushButton(this);
	QPushButton *closeBtn = new QPushButton(this);

	QPushButton *homeBtn = new QPushButton(this);

	QStyle* style = QApplication::style();
	QIcon icon = style->standardIcon(QStyle::SP_FileIcon);

	openBtn->setIcon(icon);
	closeBtn->setIcon(icon);
	homeBtn->setIcon(icon);

	//QHBoxLayout* layout = new QHBoxLayout;
	//layout->setMargin(0);
	//setLayout(layout);
}


topToolBar::~topToolBar()
{

}

void topToolBar::setScene(osg::Group* scene)
{
	this->_scene = scene;
}
