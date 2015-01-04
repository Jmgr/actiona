/*
    Actiona
    Copyright (C) 2008-2015 Jonathan Mercier-Ganady

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

namespace Tools
{
    const QPair<QStringList, QStringList> languagesName = qMakePair(
                QStringList()   << "" << "en_US" << "fr_FR",
                QStringList()   << QT_TRANSLATE_NOOP("languagesName", "System language (if available)")
                                << QT_TRANSLATE_NOOP("languagesName", "English (US)")
                << QT_TRANSLATE_NOOP("languagesName", "French (France)"));

    QString locale()
    {
        QSettings settings;
        QString locale = settings.value("gui/locale").toString();

        if(locale.isEmpty())
        {
            locale = QLocale::system().name();

    #ifdef Q_OS_WIN
            QString installerLanguage = settings.value("installerLanguage").toString();
            if(!installerLanguage.isEmpty())
            {
                if(installerLanguage == "english")
                    locale = "en_US";
                else if(installerLanguage == "french")
                    locale = "fr_FR";
            }
    #endif
        }

        return locale;
    }

    void installQtTranslator(const QString &locale)
    {
        QTranslator *translator = new QTranslator(QCoreApplication::instance());

    #ifdef Q_OS_WIN
        translator->load(QString("%1/locale/qt_%2").arg(QCoreApplication::applicationDirPath()).arg(locale));
    #else
        translator->load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    #endif

        if(!translator->isEmpty())
            QCoreApplication::installTranslator(translator);
        else
            delete translator;
    }

    void installTranslator(const QString &componentName, const QString &locale)
    {
        QTranslator *translator = new QTranslator(QCoreApplication::instance());
        if(!translator->load(QString("%1/locale/%2_%3").arg(QCoreApplication::applicationDirPath()).arg(componentName).arg(locale)))
        {
            if(!translator->load(QString("%1/locale/%2_%3").arg(QDir::currentPath()).arg(componentName).arg(locale)))
            {
    #ifndef Q_OS_WIN
                translator->load(QString("%1/share/actiona/locale/%2_%3").arg(ACT_PREFIX).arg(componentName).arg(locale));
    #endif
            }
        }

        if(!translator->isEmpty())
            QCoreApplication::installTranslator(translator);
        else
            delete translator;
    }

    int languageNameToIndex(const QString &languageName)
    {
        int index = 0;

        for(const QString &language: languagesName.first)
        {
            if(language == languageName)
                return index;

            ++index;
        }

        return 0;
    }
}
