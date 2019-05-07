#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QStringList>
#include <QCheckBox>
#include <QLayout>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <osg/Switch>
#include <osg/Group>

class TreeWidget : public QWidget
{
	Q_OBJECT

public:
	TreeWidget();
	~TreeWidget();

	void setScene(osg::Group*);
	void changeItem(int);

private slots:
	void updateItem(QTreeWidgetItem* item, int column);
	//void highLightItem(QTreeWidgetItem* item, int column);

private:
	QTreeWidgetItem* _root;
	QTreeWidget* _tree;
	QVector<QTreeWidgetItem*> _itemList;

	osg::Group* _scene;

	void createItem(int);

	void changeSwitchState(int, bool);

	void updateRootItem();
};

