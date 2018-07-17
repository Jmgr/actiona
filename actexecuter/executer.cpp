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

#include "executer.h"
#include "actionfactory.h"

#include <iostream>
#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QLocale>

Executer::Executer(QObject *parent) :
	QObject(parent),
	mActionFactory(new ActionTools::ActionFactory(this))
	
{
    connect(mActionFactory, &ActionTools::ActionFactory::actionPackLoadError, this, &Executer::actionPackLoadError);
}

Executer::~Executer() = default;

bool Executer::start(QIODevice *device, const QString &filename)
{
	Q_UNUSED(device)
	Q_UNUSED(filename)

	QSettings settings;
	QString locale = settings.value(QStringLiteral("gui/locale"), QLocale::system().name()).toString();

	mActionFactory->loadActionPacks(QApplication::applicationDirPath() + QStringLiteral("/actions/"), locale);
#ifndef Q_OS_WIN
	if(mActionFactory->actionPackCount() == 0)
		mActionFactory->loadActionPacks(QStringLiteral("%1/%2/actiona/actions/").arg(QLatin1String(ACT_PREFIX)).arg(QLatin1String(ACT_LIBDIR)), locale);
#endif

	if(mActionLoadingFailed)
		return false;

	return true;
}

ActionTools::ActionFactory *Executer::actionFactory() const
{
	return mActionFactory;
}

void Executer::actionPackLoadError(const QString &error)
{
	mActionLoadingFailed = true;

	std::wcerr << error.toStdWString() << std::endl;
}
