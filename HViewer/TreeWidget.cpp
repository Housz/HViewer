#include "treeWidget.h"



TreeWidget::TreeWidget()
{
	_tree = new QTreeWidget;

	_tree->setHeaderLabel("LAYER LIST");

	_root = new QTreeWidgetItem();
	_root->setText(0, "root");

	_tree->addTopLevelItem(_root);

	_root->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	_root->setCheckState(0, Qt::Checked);

	changeItem(0);

	connect(_tree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateItem(QTreeWidgetItem*, int)));

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(_tree);
	layout->setMargin(0);
	setLayout(layout);
}


TreeWidget::~TreeWidget()
{
}

void TreeWidget::setScene(osg::Group* scene)
{
	this->_scene = scene;
}

void TreeWidget::changeItem(int num)
{
	//clear history items
	for (auto iter = _itemList.begin(); iter != _itemList.end(); iter++)
	{
		_root->removeChild(*iter);
	}
	_itemList.clear();

	createItem(num);
}

void TreeWidget::createItem(int num)
{
	if (num == 0)
	{
		_root->setHidden(true);
	}
	else
	{
		_root->setHidden(false);
	}

	for (int i = 0; i < num; i++)
	{
		QTreeWidgetItem* subItem = new QTreeWidgetItem(_root);
		subItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		subItem->setText(0, "subItem");
		subItem->setCheckState(0, Qt::Checked);

		//connect(subItem, SIGNAL(itemChanged(QTreeWidgetItem*, int), );

		_itemList.push_back(subItem);
	}

	_tree->expandAll();
}

void TreeWidget::changeSwitchState(int index, bool targetState)
{
	int count = _scene->getChild(index)->asSwitch()->getNumChildren();
	for (size_t i = 0; i < count; i++)
	{
		_scene->getChild(index)->asSwitch()->setValue(i, targetState);
	}
}


void TreeWidget::updateItem(QTreeWidgetItem* item, int column)
{
	if (Qt::Checked == item->checkState(0))
	{
		int count = item->childCount();
		if (count > 0) //root
		{
			for (int i = 0; i < count; i++)
			{
				item->child(i)->setCheckState(0, Qt::Checked);
				
				// show all layers
				changeSwitchState(i, true);
			}
		}
		else //subItem
		{
			updateRootItem();
			int index = 0;
			for (auto iter = _itemList.begin(); iter != _itemList.end(); iter++)
			{
				if (*iter == item)
				{
					break;
				}
				index++;
			}
			changeSwitchState(index, true);
		}

	}

	if (Qt::Unchecked == item->checkState(0))
	{
		int count = item->childCount();
		if (count > 0) //root
		{
			for (int i = 0; i < count; i++)
			{
				item->child(i)->setCheckState(0, Qt::Unchecked);

				changeSwitchState(i, false);
			}
		}
		else //subItem
		{
			updateRootItem();

			int index = 0;
			for (auto iter = _itemList.begin(); iter != _itemList.end(); iter++)
			{
				if (*iter == item)
				{
					break;
				}
				index++;
			}
			changeSwitchState(index, false);
		}
	}
}

void TreeWidget::updateRootItem()
{
	int childCount = _root->childCount();
	int checkedCount = 0;
	for (int i = 0; i < childCount; i++)
	{
		QTreeWidgetItem* childItem = _root->child(i);
		if (childItem->checkState(0) == Qt::Checked)
		{
			checkedCount++;
		}
	}

	if (checkedCount <= 0)
	{
		_root->setCheckState(0, Qt::Unchecked);
	}
	else if (checkedCount == childCount)
	{
		_root->setCheckState(0, Qt::Checked);
	}
	else
	{
		_root->setCheckState(0, Qt::PartiallyChecked);
	}
}
