#include <View/DesignDialog.h>

#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLayout>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QSplitter>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

class ConnectDialog : public QDialog
{
	Q_OBJECT

  public:
	explicit ConnectDialog(const QList< QString > &portStrings, QWidget *parent = nullptr);
	~ConnectDialog() noexcept = default;

	int getValue() const noexcept;

  private slots:
	void onOkClicked();
	void onCancelClicked();

  private:
	QComboBox *m_combo;
	int m_index;
};

ConnectDialog::ConnectDialog(const QList< QString > &portStrings, QWidget *parent) :
	QDialog(parent), m_combo(nullptr), m_index(-1)
{
	QLayout *layout = new QVBoxLayout(this);
	QComboBox *combo = new QComboBox(this);

	for (const QString &portString : portStrings)
	{
		combo->addItem(portString);
	}

	QWidget *buttonsWidget = new QWidget(this);
	QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsWidget);
	buttonsLayout->setAlignment(Qt::AlignmentFlag::AlignRight);
	QPushButton *okButton = new QPushButton(tr("OK"), buttonsWidget);
	QPushButton *cancelButton = new QPushButton(tr("Cancel"), buttonsWidget);

	connect(okButton, SIGNAL(clicked(bool)), SLOT(onOkClicked()));
	connect(cancelButton, SIGNAL(clicked(bool)), SLOT(onCancelClicked()));

	buttonsLayout->addWidget(okButton);
	buttonsLayout->addWidget(cancelButton);

	layout->addWidget(combo);
	layout->addWidget(buttonsWidget);

	setLayout(layout);

	setWindowTitle(tr("Connect..."));

	m_combo = combo;
};

void ConnectDialog::onOkClicked()
{
	m_index = m_combo->currentIndex();
	this->close();
}

void ConnectDialog::onCancelClicked()
{
	m_index = -1;
	this->close();
}

int ConnectDialog::getValue() const noexcept
{
	return m_index;
}

int DesignDialog::Connect(const QList< QString > &portStrings)
{
	ConnectDialog dlg(portStrings);
	dlg.show();
	dlg.raise();
	dlg.exec();
	return dlg.getValue();
}

#include "DesignDialog.moc"
