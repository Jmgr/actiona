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

#include "executer.h"
#include "actionfactory.h"

#include <iostream>
#include <QApplication>
#include <QFile>

Executer::Executer(QObject *parent) :
	QObject(parent),
	mActionFactory(new ActionTools::ActionFactory(this)),
	mActionLoadingFailed(false)
{
	connect(mActionFactory, SIGNAL(actionPackLoadError(QString)), this, SLOT(actionPackLoadError(QString)));
}

Executer::~Executer()
{
}

bool Executer::start(QFile &file)
{
	Q_UNUSED(file)

	mActionFactory->loadActionPacks();
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
