#include "treeWidget.h"



TreeWidget::TreeWidget()
{
	tree = new QTreeWidget;

	tree->setHeaderLabel("LAYER LIST");

	root = new QTreeWidgetItem();
	root->setText(0, "root");

	tree->addTopLevelItem(root);

	root->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	root->setCheckState(0, Qt::Checked);

	changeItem(0);

	connect(tree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateItem(QTreeWidgetItem*, int)));

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(tree);
	layout->setMargin(0);
	setLayout(layout);
}


TreeWidget::~TreeWidget()
{
}

void TreeWidget::changeItem(int num)
{
	itemList.clear();

	createItem(num);
}

void TreeWidget::createItem(int num)
{
	if (num == 0)
	{
		root->setHidden(true);
	}
	else
	{
		root->setHidden(false);
	}

	for (int i = 0; i < num; i++)
	{
		QTreeWidgetItem* subItem = new QTreeWidgetItem(root);
		subItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		subItem->setText(0, "subItem");
		subItem->setCheckState(0, Qt::Checked);

		//connect(subItem, SIGNAL(itemChanged(QTreeWidgetItem*, int), );

		itemList.push_back(subItem);
	}

	tree->expandAll();
}

void TreeWidget::updateItem(QTreeWidgetItem* item, int column)
{
	if (Qt::Checked == item->checkState(0))
	{
		int count = item->childCount();
		if (count > 0)
		{
			for (int i = 0; i < count; i++)
			{
				item->child(i)->setCheckState(0, Qt::Checked);
			}
		}
		else
		{
			updateRootItem();
		}

	}

	if (Qt::Unchecked == item->checkState(0))
	{
		int count = item->childCount();
		if (count > 0)
		{
			for (int i = 0; i < count; i++)
			{
				item->child(i)->setCheckState(0, Qt::Unchecked);
			}
		}
		else
		{
			updateRootItem();
		}
	}
}

void TreeWidget::updateRootItem()
{
	int childCount = root->childCount();
	int checkedCount = 0;
	for (int i = 0; i < childCount; i++)
	{
		QTreeWidgetItem* childItem = root->child(i);
		if (childItem->checkState(0) == Qt::Checked)
		{
			checkedCount++;
		}
	}

	if (checkedCount <= 0)
	{
		root->setCheckState(0, Qt::Unchecked);
	}
	else if (checkedCount == childCount)
	{
		root->setCheckState(0, Qt::Checked);
	}
	else
	{
		root->setCheckState(0, Qt::PartiallyChecked);
	}
}

