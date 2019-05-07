#pragma once
#include <QWidget>
#include <QLayout>
#include <QDebug>
#include <QPushButton>
#include <QMenuBar>
#include <QToolBar>
#include <QLabel>
#include <QDialog>
#include <QStatusBar>
#include <QDockWidget>
#include <QTextEdit>
#include <QFileDialog>
#include <QFileInfo>
#include <QAction>
#include <QIcon>
#include <QStyle>
#include <QApplication>
#include <QVector>

#include <osg/Group>

class topToolBar : public QToolBar
{
	Q_OBJECT

public:
	topToolBar();
	~topToolBar();

	void setScene(osg::Group*);

protected:
	osg::Group* _scene;
};

