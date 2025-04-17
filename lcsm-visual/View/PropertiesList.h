#ifndef LCSM_VISUAL_VIEW_PROPERTIESLIST_H
#define LCSM_VISUAL_VIEW_PROPERTIESLIST_H

#include <lcsm/Support/PointerView.hpp>

#include <QComboBox>
#include <QHash>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QPair>
#include <QSpinBox>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVariant>
#include <QWidget>
#include <limits>

class Item;

class PropertiesList : public QTableWidget
{
  public:
	explicit PropertiesList(QWidget *parent = nullptr);
	~PropertiesList() noexcept;

	void setCurrentItem(Item *item);

	int addEditableItem(const QString &key, const QString &value = QString());
	int addNumericalItem(const QString &key, int value = 0, int minValue = std::numeric_limits< int >::min(), int maxValue = std::numeric_limits< int >::max());
	int addBooleanItem(const QString &key, const QString &falseString, const QString &trueString, bool value = false);
	int addChoicesItem(const QString &key, const QList< QString > &choiceStrings, const QList< QVariant > &choiceValues, const QVariant &value = QVariant());

	void clearAll();

  private:
	int addItem();

  private slots:
	void onEditChanged(int row);
	void onNumericalChanged(int row, int value);
	void onBooleanChanged(int row, int index);
	void onChoicesChanged(int row);

  private:
	QList< QLabel * > m_labels;
	QHash< int, QLineEdit * > m_editables;
	QHash< int, QSpinBox * > m_numericals;
	QHash< int, QComboBox * > m_booleans;
	QHash< int, QComboBox * > m_choices;
	lcsm::support::PointerView< Item > m_currentItem;
};

#endif /* LCSM_VISUAL_VIEW_PROPERTIESLIST_H */
