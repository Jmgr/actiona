#include "executionwindow.h"
#include "ui_executionwindow.h"

ExecutionWindow::ExecutionWindow(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::ExecutionWindow)
{
	ui->setupUi(this);

	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	connect(ui->cancelPushButton, SIGNAL(clicked()), this, SIGNAL(canceled()));
}

ExecutionWindow::~ExecutionWindow()
{
	delete ui;
}

void ExecutionWindow::setStopExecutionShortcut(const QKeySequence &keySequence)
{
	ui->stopScriptLabel->setText(tr("Press %1 to stop the script execution").arg(keySequence.toString()));

	adjustSize();
}

void ExecutionWindow::setCurrentActionName(const QString &actionName)
{
	ui->currentActionLabel->setText(actionName);

	adjustSize();
}

void ExecutionWindow::setCurrentActionColor(const QColor &actionColor)
{
	QPalette newPalette = palette();

	if(actionColor.isValid())
	{
		ui->currentActionLabel->setAutoFillBackground(true);
		newPalette.setColor(QPalette::Window, actionColor);
	}
	else
		ui->currentActionLabel->setAutoFillBackground(false);

	ui->currentActionLabel->setPalette(newPalette);
}

void ExecutionWindow::changeEvent(QEvent *event)
{
	QWidget::changeEvent(event);

	switch(event->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
