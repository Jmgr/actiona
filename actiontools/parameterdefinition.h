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

#ifndef PARAMETERDEFINITION_H
#define PARAMETERDEFINITION_H

#include "elementdefinition.h"
#include "actiontools_global.h"
#include "parameter.h"

#include <QList>
#include <QVariantMap>

class QWidget;

namespace ActionTools
{
	class Action;
	class Script;

	class ACTIONTOOLSSHARED_EXPORT ParameterDefinition : public ElementDefinition
	{
		Q_OBJECT

	public:
		ParameterDefinition(Category category, const QString &name, const QString &translatedName, QObject *parent = 0);
		virtual ~ParameterDefinition()												{}

		virtual void buildEditors(Script *script, QWidget *parent);
		virtual void load(const Action *action) = 0;
		virtual void save(Action *action) = 0;
		virtual void setDefaultValues(Parameter &data) = 0;

		QList<QWidget *> editors() const											{ return mEditors; }
		QVariant option(QString name, QVariant defaultValue = QVariant()) const;

		void addEditor(QWidget *editor);
		void setOption(const QString &name, const QVariant &value)					{ mOptions.insert(name, value); }

	private:
		QList<QWidget *> mEditors;
		QVariantMap mOptions;

		Q_DISABLE_COPY(ParameterDefinition)
	};
}

#endif // PARAMETERDEFINITION_H
