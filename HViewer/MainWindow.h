#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QMenuBar>
#include <QToolBar>
#include <QLabel>
#include <QDialog>
#include <QStatusBar>
#include <QDockWidget>
#include <QTextEdit>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QAction>
#include <QIcon>
#include <QApplication>
#include <QColorDialog>

#include "ViewerWidget.h"
#include "treeWidget.h"


#if _MSC_VER >= 1600  
#pragma execution_character_set("utf-8")
#endif

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

	void setStatusText(QString);
private:
	ViewerWidget* _viewerWidget;
	QLabel* label_L;

	QToolBar* _treeToolBar;
	TreeWidget* _treeWidget;

	QToolBar* _toolBar;


	void setLayerList(osg::Group*);
	void initLayerList(osg::Group*);
};
