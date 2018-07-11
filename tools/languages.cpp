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

#include "languages.h"

#include <QSettings>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>
#include <QLibraryInfo>
#include <QDir>

#include <utility>

namespace Tools
{
    StringListPair Languages::m_languagesName =
    {
        {
            QStringLiteral(""),
            QStringLiteral("en_US"),
            QStringLiteral("fr_FR")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("languagesName", "System language (if available)")),
            QStringLiteral(QT_TRANSLATE_NOOP("languagesName", "English (US)")),
            QStringLiteral(QT_TRANSLATE_NOOP("languagesName", "French (France)"))
        }
    };

    bool Languages::m_areLanguagesNameTranslated = false;

    QString Languages::locale()
    {
        QSettings settings;
		QString locale = settings.value(QStringLiteral("gui/locale")).toString();

        if(locale.isEmpty())
        {
            locale = QLocale::system().name();

	#ifdef Q_OS_WIN
			QString installerLanguage = settings.value(QStringLiteral("installerLanguage")).toString();
            if(!installerLanguage.isEmpty())
            {
				if(installerLanguage == QLatin1String("english"))
					locale = QStringLiteral("en_US");
				else if(installerLanguage == QLatin1String("french"))
					locale = QStringLiteral("fr_FR");
            }
    #endif
        }

        return locale;
    }

    void Languages::installTranslator(const QString &componentName, const QString &locale)
    {
        auto translator = new QTranslator(QCoreApplication::instance());
		if(!translator->load(QStringLiteral("%1/locale/%2_%3").arg(QCoreApplication::applicationDirPath()).arg(componentName).arg(locale)))
        {
			if(!translator->load(QStringLiteral("%1/locale/%2_%3").arg(QDir::currentPath()).arg(componentName).arg(locale)))
            {
    #ifndef Q_OS_WIN
				translator->load(QStringLiteral("%1/share/actiona/locale/%2_%3").arg(QLatin1String(ACT_PREFIX)).arg(componentName).arg(locale));
    #endif
            }
        }

        if(!translator->isEmpty())
            QCoreApplication::installTranslator(translator);
        else
            delete translator;
    }

    StringListPair Languages::languagesName()
    {
        if(!m_areLanguagesNameTranslated)
        {
            m_areLanguagesNameTranslated = true;

            for(int index = 0; index < m_languagesName.second.size(); ++index)
                m_languagesName.second[index] = QCoreApplication::translate("languagesName", m_languagesName.second.at(index).toLatin1().constData());
        }

        return m_languagesName;
    }

    int Languages::languageNameToIndex(const QString &languageName)
    {
        int index = 0;

        for(const QString &language: m_languagesName.first)
        {
            if(language == languageName)
                return index;

            ++index;
        }

		return 0;
	}
}
