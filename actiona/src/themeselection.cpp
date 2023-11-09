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

    Contact: jmgr@jmgr.info
*/

#include "themeselection.hpp"

#include <QApplication>
#include <QFile>
#include <QDebug>

QString lightThemeFilepath = QStringLiteral(":qdarkstyle/light/lightstyle.qss");
QString darkThemeFilepath = QStringLiteral(":qdarkstyle/dark/darkstyle.qss");

void ThemeSelection::selectTheme(Theme theme)
{
    switch(theme)
    {
    case Theme::Default:
        qApp->setStyleSheet({});
        break;
    case Theme::Light:
        selectTheme(lightThemeFilepath);
        break;
    case Theme::Dark:
        selectTheme(darkThemeFilepath);
        break;
    }
}

ThemeSelection::Theme ThemeSelection::currentTheme()
{
    auto theme = qApp->styleSheet();
    if(theme == lightThemeFilepath)
        return Theme::Light;
    if(theme == darkThemeFilepath)
        return Theme::Dark;
    return Theme::Default;
}

void ThemeSelection::selectTheme(const QString &filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Unable to set stylesheet, file not found: " << filepath;
        return;
    }

    QTextStream stream(&file);
    qApp->setStyleSheet(stream.readAll());
}
