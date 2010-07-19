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

#ifndef EXECUTIONALGORITHMS_H
#define EXECUTIONALGORITHMS_H

#include "executer_global.h"

#include <QObject>
#include <QScriptable>

namespace Executer
{
	class EXECUTERSHARED_EXPORT ExecutionAlgorithms : public QObject, protected QScriptable
	{
		Q_OBJECT
		
	public:
		explicit ExecutionAlgorithms(QObject *parent = 0);
	
	public slots:
		QString md4(const QString &data) const;
		QString md5(const QString &data) const;
		QString sha1(const QString &data) const;
		void setRandomSeed(uint seed) const;
		int randomMax() const;
		int randomInt() const;
		int randomInt(int min, int max) const;
		float randomFloat(float min, float max) const;
	};
}

#endif // EXECUTIONALGORITHMS_H
