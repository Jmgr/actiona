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

#include "backend/areachooser-x11.hpp"

#include <QProcess>
#include <QRegularExpression>

namespace Backend
{
    AreaChooserX11::AreaChooserX11(QObject *parent):
        AreaChooser(parent),
        mXRectSelProcess(new QProcess(this))
    {
        mXRectSelProcess->setProgram(QStringLiteral("backend-xrectsel"));
        connect(mXRectSelProcess, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error)
        {
            if(error == QProcess::FailedToStart)
            {
                qDebug("failed to start");
                emit canceled(); // TODO: add error signal
            }
        });
        connect(mXRectSelProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this](int exitCode, QProcess::ExitStatus exitStatus)
        {
            static QRegularExpression resultRegex(QStringLiteral(R"((\d+)x(\d+)\+(\d+)\+(\d+))"));

            if(exitStatus != QProcess::NormalExit || exitCode != EXIT_SUCCESS)
            {
                emit canceled(); // TODO: add error signal
                return;
            }

            auto result = QString::fromLocal8Bit(mXRectSelProcess->readAllStandardOutput());
            auto match = resultRegex.match(result);
            if(!match.isValid())
            {
                emit canceled();
                return;
            }

            bool ok;
            int w = match.captured(1).toInt(&ok);
            int h = match.captured(2).toInt(&ok);
            int x = match.captured(3).toInt(&ok);
            int y = match.captured(4).toInt(&ok);
            if(!ok)
            {
                emit canceled();
                return;
            }

            emit done(QRect(x, y, w, h));
        });
    }

    AreaChooserX11::~AreaChooserX11()
    {
        mXRectSelProcess->kill();
    }

    void AreaChooserX11::choose()
    {
        mXRectSelProcess->start();
    }
}
