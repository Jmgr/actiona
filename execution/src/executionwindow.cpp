/*
	Actiona
	Copyright (C) 2005 Jonathan Mercier-Ganady

	Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actiona is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact: jmgr@jmgr.info
*/

#include "executionwindow.hpp"
#include "ui_executionwindow.h"

namespace Execution
{
	ExecutionWindow::ExecutionWindow(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::ExecutionWindow)
	{
		ui->setupUi(this);
		
        setProgressEnabled(false);

        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Tool);
        ui->debugPushButton->setEnabled(false);
	
        connect(ui->cancelPushButton, &QPushButton::clicked, this, &ExecutionWindow::canceled);
        connect(ui->pausePushButton, &QPushButton::clicked, this, &ExecutionWindow::paused);
        connect(ui->debugPushButton, &QPushButton::clicked, this, &ExecutionWindow::debug);
	}
	
	ExecutionWindow::~ExecutionWindow()
	{
		delete ui;
	}
	
    void ExecutionWindow::setCurrentActionName(const QString &actionName, const QString &line)
	{
		ui->currentActionLabel->setText(actionName);
        ui->currentLineLabel->setText(line);
	
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
	
	void ExecutionWindow::setProgressEnabled(bool enabled)
	{
		ui->timeoutProgressBar->setVisible(enabled);
		if(!enabled)
		{
			ui->timeoutProgressBar->setMaximum(100);
			ui->timeoutProgressBar->setValue(100);
		}
		
		adjustSize();
	}

	void ExecutionWindow::setProgressValue(int value)
	{
		ui->timeoutProgressBar->setValue(value);
	}

	void ExecutionWindow::setProgressMinimum(int minimum)
	{
		ui->timeoutProgressBar->setMinimum(minimum);
	}

	void ExecutionWindow::setProgressMaximum(int maximum)	
	{
		ui->timeoutProgressBar->setMaximum(maximum);
	}
	
	void ExecutionWindow::setPauseStatus(bool paused)
	{
		if(paused)
			ui->pausePushButton->setIcon(QIcon(QStringLiteral(":/images/play.png")));
		else
			ui->pausePushButton->setIcon(QIcon(QStringLiteral(":/images/pause.png")));
	}
	
	void ExecutionWindow::onEvaluationResumed()
	{
		setPauseStatus(false);
	}

	void ExecutionWindow::onEvaluationPaused()
	{
		setPauseStatus(true);
	}

	void ExecutionWindow::enableDebug()
	{
		ui->debugPushButton->setEnabled(true);
	}

	void ExecutionWindow::disableDebug()
	{
		ui->debugPushButton->setEnabled(false);
	}

	void ExecutionWindow::on_pausePushButton_clicked()
	{
		setPauseStatus(true);
	}

	void ExecutionWindow::on_debugPushButton_clicked()
	{
		setPauseStatus(true);
	}
}
