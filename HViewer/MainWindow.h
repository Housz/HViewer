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

#include "ViewerWidget.h"

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
};
