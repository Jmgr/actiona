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

	Contact : jmgr@jmgr.info
*/

#include "progresssplashscreen.h"

#include <QProgressBar>
#include <QTimer>

#ifdef Q_OS_UNIX
#include <QX11Info>
#endif

ProgressSplashScreen::ProgressSplashScreen(const QPixmap &pixmap, Qt::WindowFlags f)
	: QSplashScreen(pixmap, f),
	mProgressBar(new QProgressBar(this)),
	
	mOpacityTimer(new QTimer(this))
{
	init();
}

ProgressSplashScreen::ProgressSplashScreen(QWidget *parent, const QPixmap &pixmap, Qt::WindowFlags f)
	: QSplashScreen(parent, pixmap, f),
	mProgressBar(new QProgressBar(this)),
	mOpacity(1.0f),
	mOpacityTimer(new QTimer(this))
{
	init();
}

void ProgressSplashScreen::setMaximum(int maximum)
{
	mProgressBar->setMaximum(maximum);
}

void ProgressSplashScreen::setMinimum(int minimum)
{
	mProgressBar->setMinimum(minimum);
}

void ProgressSplashScreen::setRange(int minimum, int maximum)
{
	mProgressBar->setRange(minimum, maximum);
}

void ProgressSplashScreen::setValue(int value)
{
	mProgressBar->setValue(value);
}

void ProgressSplashScreen::fadeOut()
{
#ifdef Q_OS_WIN
    mOpacityTimer->start(25);
#endif
#ifdef Q_OS_UNIX
    close();
#endif
}

void ProgressSplashScreen::drawContents(QPainter *painter)
{
	Q_UNUSED(painter)

	mProgressBar->update();
}

void ProgressSplashScreen::onMessageChanged(const QString &message)
{
	mProgressBar->setFormat(message);
}

void ProgressSplashScreen::opacityCloseUpdate()
{
	if(mOpacity > 0.0f)
	{
		mOpacity -= 0.06f;
		setWindowOpacity(mOpacity);
	}
	else
	{
		mOpacityTimer->stop();
		close();
        deleteLater();
	}
}

void ProgressSplashScreen::init()
{
    connect(this, &ProgressSplashScreen::messageChanged, this, &ProgressSplashScreen::onMessageChanged);

	int progressBarHeight = static_cast<int>(mProgressBar->height() * 0.7f);

	resize(width(), height() + progressBarHeight);

	mProgressBar->setStyleSheet(QStringLiteral("QProgressBar { border: 1px solid black; text-align: center; padding: 1px; background: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fff, stop: 0.4999 #eee, stop: 0.5 #ddd, stop: 1 #eee ); color: black; }\n") +
								QStringLiteral("QProgressBar::chunk { background: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #abf, stop: 0.4999 #79d, stop: 0.5 #78d, stop: 1 #56b ); border: 1px solid black; }"));

	mProgressBar->setFormat(QStringLiteral(""));
	mProgressBar->setRange(0, 1);
	mProgressBar->setValue(0);
	mProgressBar->setAlignment(Qt::AlignCenter);
	mProgressBar->setGeometry(0, height() - progressBarHeight, width(), progressBarHeight);

	mOpacityTimer->setSingleShot(false);
    connect(mOpacityTimer, &QTimer::timeout, this, &ProgressSplashScreen::opacityCloseUpdate);
}
