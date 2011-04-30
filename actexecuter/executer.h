/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef EXECUTER_H
#define EXECUTER_H

#include <QObject>

class QFile;

namespace ActionTools
{
	class ActionFactory;
}

class Executer : public QObject
{
    Q_OBJECT
	
public:
    explicit Executer(QObject *parent = 0);
	virtual ~Executer();
	
	virtual bool start(QFile &file);
	
protected:
	ActionTools::ActionFactory *actionFactory() const;

private slots:
	void actionPackLoadError(const QString &error);

private:
	ActionTools::ActionFactory *mActionFactory;
	bool mActionLoadingFailed;
};

#endif // EXECUTER_H
