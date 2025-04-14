#include <View/SimulateDialog.h>
#include <lcsm/LCSM.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Verilog/Bit.h>
#include <lcsm/Verilog/Strength.h>

#include <QComboBox>
#include <QDialog>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QList>
#include <QListView>
#include <QObject>
#include <QPushButton>
#include <QSplitter>
#include <QString>
#include <QStringList>
#include <QTabWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

class PutValueDialog : public QDialog
{
	Q_OBJECT

  public:
	explicit PutValueDialog(lcsm::width_t width, bool *ok, QWidget *parent = nullptr);
	~PutValueDialog() noexcept = default;

	lcsm::DataBits getValue();

  private slots:
	void onOkClicked();
	void onCancelClicked();

  private:
	lcsm::width_t m_width;
	bool *m_ok;
	QList< QComboBox * > m_bitCombos;
	QList< QComboBox * > m_strengthCombos;
};

PutValueDialog::PutValueDialog(lcsm::width_t width, bool *ok, QWidget *parent) : QDialog(parent), m_ok(ok)
{
	const QStringList labels = { tr("#"), tr("Bit"), tr("Strength") };
	const QList< lcsm::verilog::Bit > bits = { lcsm::verilog::Bit::Undefined, lcsm::verilog::Bit::False, lcsm::verilog::Bit::True };
	const int rows = static_cast< int >(width);

	QLayout *layout = new QVBoxLayout(this);
	QSplitter *splitter = new QSplitter(Qt::Orientation::Vertical, this);
	QTableWidget *table = new QTableWidget(rows, 3, splitter);

	QWidget *buttonsWidget = new QWidget(splitter);
	QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsWidget);
	buttonsLayout->setAlignment(Qt::AlignmentFlag::AlignRight);
	QPushButton *okButton = new QPushButton(tr("OK"), buttonsWidget);
	QPushButton *cancelButton = new QPushButton(tr("Cancel"), buttonsWidget);

	connect(okButton, SIGNAL(clicked(bool)), SLOT(onOkClicked()));
	connect(cancelButton, SIGNAL(clicked(bool)), SLOT(onCancelClicked()));

	buttonsLayout->addWidget(okButton);
	buttonsLayout->addWidget(cancelButton);

	table->setHorizontalHeaderLabels(labels);
	table->verticalHeader()->setVisible(false);

	for (int i = 0; i < rows; i++)
	{
		QLabel *nLabel = new QLabel(QString::number(i + 1), table);
		nLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
		QComboBox *bitCombo = new QComboBox(table);
		QComboBox *strengthCombo = new QComboBox(table);
		for (const lcsm::verilog::Bit bit : bits)
		{
			const char *pretty = lcsm::verilog::BitPretty(bit);
			bitCombo->addItem(tr(pretty), bit);
		}
		for (const lcsm::verilog::Strength strength : lcsm::verilog::Strengths)
		{
			const char *pretty = lcsm::verilog::StrengthPretty(strength);
			strengthCombo->addItem(tr(pretty), strength);
		}
		table->setCellWidget(i, 0, nLabel);
		table->setCellWidget(i, 1, bitCombo);
		table->setCellWidget(i, 2, strengthCombo);
		m_bitCombos.append(bitCombo);
		m_strengthCombos.append(strengthCombo);
	}

	table->resizeColumnsToContents();

	splitter->addWidget(table);
	splitter->addWidget(buttonsWidget);

	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 1);

	layout->addWidget(splitter);

	setLayout(layout);

	setWindowTitle(tr("Put value..."));

	m_width = width;
}

lcsm::DataBits PutValueDialog::getValue()
{
	lcsm::DataBits databits;
	databits.setWidth(m_width);
	if (*m_ok)
	{
		for (int i = 0; i < static_cast< int >(m_width); i++)
		{
			const QComboBox *bitCombo = m_bitCombos[i];
			const QComboBox *strengthCombo = m_strengthCombos[i];
			const QVariant b = bitCombo->currentData();
			const QVariant s = strengthCombo->currentData();
			const lcsm::verilog::Bit bit = static_cast< lcsm::verilog::Bit >(b.toUInt());
			const lcsm::verilog::Strength strength = static_cast< lcsm::verilog::Strength >(s.toUInt());
			databits.set(i, bit, strength);
		}
	}
	return databits;
}

void PutValueDialog::onOkClicked()
{
	*m_ok = true;
	close();
}

void PutValueDialog::onCancelClicked()
{
	*m_ok = false;
	close();
}

class ShowValueDialog : public QDialog
{
	Q_OBJECT

  public:
	explicit ShowValueDialog(const lcsm::LCSMState *state, lcsm::Identifier id, QWidget *parent = nullptr);
	~ShowValueDialog() noexcept = default;

  private slots:
	void onCloseClicked();
};

ShowValueDialog::ShowValueDialog(const lcsm::LCSMState *state, lcsm::Identifier id, QWidget *parent) : QDialog(parent)
{
	const QStringList labels = { tr("#"), tr("Bit"), tr("Strength") };

	QLayout *layout = new QVBoxLayout(this);
	QSplitter *splitter = new QSplitter(Qt::Orientation::Vertical, this);
	QTabWidget *tab = new QTabWidget(splitter);

	for (std::size_t i = 0; i < state->valueSize(id); i++)
	{
		const lcsm::DataBits &databits = state->valueOf(id, i);
		const int rows = static_cast< int >(databits.width());
		QTableWidget *table = new QTableWidget(rows, 3, tab);
		table->setHorizontalHeaderLabels(labels);
		table->verticalHeader()->setVisible(false);
		for (int row = 0; row < rows; row++)
		{
			const lcsm::verilog::Bit bit = databits.bit(row);
			const lcsm::verilog::Strength strength = databits.strength(row);
			QLabel *nLabel = new QLabel(QString::number(row + 1), table);
			nLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
			QLabel *bitLabel = new QLabel(tr(lcsm::verilog::BitPretty(bit)), table);
			bitLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
			QLabel *strengthLabel = new QLabel(tr(lcsm::verilog::StrengthPretty(strength)), table);
			strengthLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
			table->setCellWidget(row, 0, nLabel);
			table->setCellWidget(row, 1, bitLabel);
			table->setCellWidget(row, 2, strengthLabel);
		}
		table->resizeColumnsToContents();
		tab->addTab(table, QString("#%1").arg(i + 1));
	}

	QWidget *buttonsWidget = new QWidget(splitter);
	QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsWidget);
	buttonsLayout->setAlignment(Qt::AlignmentFlag::AlignRight);
	QPushButton *closeButton = new QPushButton(tr("Close"), buttonsWidget);

	connect(closeButton, SIGNAL(clicked(bool)), SLOT(onCloseClicked()));

	buttonsLayout->addWidget(closeButton);

	splitter->addWidget(tab);
	splitter->addWidget(buttonsWidget);

	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 1);

	layout->addWidget(splitter);

	setLayout(layout);

	setWindowTitle(tr("Show value..."));
}

void ShowValueDialog::onCloseClicked()
{
	close();
}

lcsm::DataBits SimulateDialog::PutValue(lcsm::width_t width, bool *ok)
{
	PutValueDialog dlg(width, ok);
	dlg.show();
	dlg.raise();
	dlg.exec();
	return dlg.getValue();
}

void SimulateDialog::ShowValue(const lcsm::LCSMState *state, lcsm::Identifier id)
{
	ShowValueDialog dlg(state, id);
	dlg.show();
	dlg.raise();
	dlg.exec();
}

#include "SimulateDialog.moc"
