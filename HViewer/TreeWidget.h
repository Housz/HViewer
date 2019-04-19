#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QStringList>
#include <QCheckBox>
#include <QLayout>
#include <QVector>

class TreeWidget : public QWidget
{
	Q_OBJECT

public:
	TreeWidget();
	~TreeWidget();

	void changeItem(int);

private slots:
	void updateItem(QTreeWidgetItem* item, int column);

private:
	QTreeWidgetItem* root;
	QTreeWidget * tree;
	QVector<QTreeWidgetItem*> itemList;

	void createItem(int);

	void updateRootItem();
};

