/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "executionwindow.h"
#include "ui_executionwindow.h"

namespace Executer
{
	ExecutionWindow::ExecutionWindow(QWidget *parent)
		: QWidget(parent),
		ui(new Ui::ExecutionWindow),
		mPaused(false)
	{
		ui->setupUi(this);
		
		setProgressEnabled(false);
	
		setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	
		connect(ui->cancelPushButton, SIGNAL(clicked()), this, SIGNAL(canceled()));
		connect(ui->pausePushButton, SIGNAL(clicked()), this, SIGNAL(paused()));
	}
	
	ExecutionWindow::~ExecutionWindow()
	{
		delete ui;
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
		//mPaused = paused;
		
		if(paused)
			ui->pausePushButton->setIcon(QIcon(":/images/play.png"));
		else
			ui->pausePushButton->setIcon(QIcon(":/images/pause.png"));
	}
	
	void ExecutionWindow::onEvaluationResumed()
	{
		setPauseStatus(false);
	}

	void ExecutionWindow::onEvaluationPaused()
	{
		setPauseStatus(true);
	}
	
	void ExecutionWindow::on_pausePushButton_clicked()
	{
		setPauseStatus(!mPaused);
	}
	
	void ExecutionWindow::on_debugPushButton_clicked()
	{
		//TODO
	}
}
