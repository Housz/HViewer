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

#include "ViewerWidget.h"
#include "treeWidget.h"

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

	QToolBar* _toolBar;
	TreeWidget* _treeWidget;

	void setLayerList(osg::Group*);
	void initLayerList(osg::Group*);
};
