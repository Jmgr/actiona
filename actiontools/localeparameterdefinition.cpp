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

#include "localeparameterdefinition.h"
#include "subparameter.h"
#include "codecombobox.h"
#include "actioninstance.h"

namespace ActionTools
{
    LocaleParameterDefinition::LocaleParameterDefinition(const Name &name, QObject *parent)
        : ParameterDefinition(name, parent)
	{
	}

    void LocaleParameterDefinition::buildEditors(Script *script, QWidget *parent)
	{
		ParameterDefinition::buildEditors(script, parent);

		mComboBox = new CodeComboBox(parent);

        for(const QLocale &locale: mLocales)
        {
            if(locale == QLocale::C)
                continue;

            QString countryName = locale.name().split(QLatin1Char('_')).at(1).toLower();
            QIcon icon{QStringLiteral(":/images/flags/%1.png").arg(countryName)};
            QString label = QStringLiteral("%1 (%2)")
                                    .arg(QLocale::languageToString(locale.language()))
                                    .arg(QLocale::countryToString(locale.country()));

            mComboBox->addItem(icon, label, locale.name());
        }

        mComboBox->model()->sort(0);

        mComboBox->insertItem(0, tr("System language"), QStringLiteral(""));

		addEditor(mComboBox);

		emit editorBuilt();
	}

    void LocaleParameterDefinition::load(const ActionInstance *actionInstance)
	{
		const SubParameter &subParameter = actionInstance->subParameter(name().original(), QStringLiteral("value"));

        int index = mComboBox->findData(subParameter.value());

        if(index != -1)
            mComboBox->setCurrentIndex(index);
        else
        {
            QString localeName = (index != -1) ? mComboBox->itemText(index) : subParameter.value();
            mComboBox->setEditText(localeName);
        }

		mComboBox->setCode(subParameter.isCode());
	}

    void LocaleParameterDefinition::save(ActionInstance *actionInstance)
	{
        int index = mComboBox->findText(mComboBox->currentText());
        QString localeName = (index != -1) ? mComboBox->currentData().toString() : mComboBox->currentText();

        actionInstance->setSubParameter(name().original(), QStringLiteral("value"), mComboBox->isCode(), localeName);
    }
}
