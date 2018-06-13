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

#pragma once

#include "parameterdefinition.h"
#include "actiontools_global.h"

#include <QPoint>
#include <QColor>

namespace ActionTools
{
	class PositionEdit;
	class ColorEdit;

	class ACTIONTOOLSSHARED_EXPORT ColorPositionParameterDefinition : public ParameterDefinition
	{
		Q_OBJECT

	public:
        ColorPositionParameterDefinition(const Name &name, QObject *parent);

		void buildEditors(Script *script, QWidget *parent) override;
		void load(const ActionInstance *actionInstance) override;
		void save(ActionInstance *actionInstance) override;
		void setDefaultValues(ActionInstance *actionInstance) override;
		Qt::Orientation editorsOrientation() const override								{ return Qt::Vertical; }
		
		void setDefaultPosition(const QPoint &position)							{ mDefaultPosition = position; }
		QPoint defaultPosition() const											{ return mDefaultPosition; }
		
		void setDefaultColor(const QColor &color)								{ mDefaultColor = color; }
		QColor defaultColor() const												{ return mDefaultColor; }
		
	private:
        QString defaultValue(QString defaultValue = {}) const override                   { Q_UNUSED(defaultValue); return {}; }
        void setDefaultValue(const QString &defaultValue) override						{ Q_UNUSED(defaultValue); }
		
		PositionEdit *mPositionEdit;
		ColorEdit *mColorEdit;
		QPoint mDefaultPosition;
		QColor mDefaultColor;

		Q_DISABLE_COPY(ColorPositionParameterDefinition)
	};
}

