#include <Items/Item.h>
#include <View/PropertiesList.h>

#include <QAbstractSpinBox>
#include <QComboBox>
#include <QDebug>
#include <QHash>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QObject>
#include <QPair>
#include <QSpinBox>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVariant>
#include <QWidget>

PropertiesList::PropertiesList(QWidget *parent) : QTableWidget(0, 2, parent), m_currentItem(nullptr)
{
	const QStringList labels = { tr("Property"), tr("Value") };
	setHorizontalHeaderLabels(labels);
	horizontalHeader()->setStretchLastSection(true);
	verticalHeader()->setVisible(false);
}

PropertiesList::~PropertiesList() noexcept
{
	clearAll();
}

void PropertiesList::setCurrentItem(Item *item)
{
	clearAll();
	if (item != nullptr)
	{
		m_currentItem = item;
		item->setPropertiesList(this);
	}
}

int PropertiesList::addEditableItem(const QString &key, const QString &value)
{
	const int index = addItem();
	QLabel *label = new QLabel(key);
	QLineEdit *edit = new QLineEdit(value);
	setCellWidget(index, 0, label);
	setCellWidget(index, 1, edit);
	m_labels.append(label);
	m_editables[index] = edit;
	const auto editChanged = [this, row = index]()
	{
		this->onEditChanged(row);
	};
	connect(edit, &QLineEdit::returnPressed, editChanged);
	connect(edit, &QLineEdit::editingFinished, editChanged);
	return index;
}

int PropertiesList::addNumericalItem(const QString &key, int value, int minValue, int maxValue)
{
	const int index = addItem();
	QLabel *label = new QLabel(key);
	QSpinBox *spin = new QSpinBox();
	spin->setMinimum(minValue);
	spin->setMaximum(maxValue);
	spin->setValue(value);
	setCellWidget(index, 0, label);
	setCellWidget(index, 1, spin);
	m_labels.append(label);
	m_numericals[index] = spin;
	const auto numericalChanged = [this, row = index](int value)
	{
		this->onNumericalChanged(row, value);
	};
	connect(spin, QOverload< int >::of(&QSpinBox::valueChanged), numericalChanged);
	return index;
}

int PropertiesList::addBooleanItem(const QString &key, const QString &falseString, const QString &trueString, bool value)
{
	const int index = addItem();
	QLabel *label = new QLabel(key);
	QComboBox *combo = new QComboBox();
	combo->addItem(falseString, QVariant(false));
	combo->addItem(trueString, QVariant(true));
	combo->setCurrentIndex(static_cast< int >(value));
	setCellWidget(index, 0, label);
	setCellWidget(index, 1, combo);
	m_labels.append(label);
	m_booleans[index] = combo;
	const auto indexChanged = [this, row = index](int index)
	{
		this->onBooleanChanged(row, index);
	};
	connect(combo, QOverload< int >::of(&QComboBox::currentIndexChanged), indexChanged);
	return index;
}

int PropertiesList::addChoicesItem(const QString &key, const QList< QString > &choiceStrings, const QList< QVariant > &choiceValues, const QVariant &value)
{
	const int index = addItem();
	QLabel *label = new QLabel(key);
	QComboBox *combo = new QComboBox();
	const int n = std::min(choiceStrings.size(), choiceValues.size());
	int found = 0;
	for (int i = 0; i < n; i++)
	{
		combo->addItem(choiceStrings[i], choiceValues[i]);
		if (value == choiceValues[i])
		{
			found = i;
		}
	}
	combo->setCurrentIndex(found);
	setCellWidget(index, 0, label);
	setCellWidget(index, 1, combo);
	m_labels.append(label);
	m_choices[index] = combo;
	const auto indexChanged = [this, row = index]()
	{
		this->onChoicesChanged(row);
	};
	connect(combo, QOverload< int >::of(&QComboBox::currentIndexChanged), indexChanged);
	return index;
}

int PropertiesList::addItem()
{
	insertRow(rowCount());
	return rowCount() - 1;
}

void PropertiesList::clearAll()
{
	const int rows = rowCount();
	for (int row = 0; row < rows; row++)
	{
		QWidget *label = cellWidget(row, 0);
		QWidget *widget = cellWidget(row, 1);
		removeCellWidget(row, 0);
		removeCellWidget(row, 1);
		delete label;
		delete widget;
	}
	m_labels.clear();
	m_editables.clear();
	m_numericals.clear();
	m_booleans.clear();
	m_choices.clear();
	m_currentItem.reset();
	clearContents();
	setRowCount(0);
}

void PropertiesList::onEditChanged(int row)
{
	const QString value = m_editables[row]->text();
	m_currentItem->setProperty(row, value);
}

void PropertiesList::onNumericalChanged(int row, int value)
{
	m_currentItem->setProperty(row, value);
}

void PropertiesList::onBooleanChanged(int row, int index)
{
	const bool value = index == 1;
	m_currentItem->setProperty(row, value);
}

void PropertiesList::onChoicesChanged(int row)
{
	const QVariant value = m_choices[row]->currentData();
	m_currentItem->setProperty(row, value);
}
