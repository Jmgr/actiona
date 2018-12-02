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

#include <QObject>

#include "systeminput.h"
#include "actiontools_global.h"

class QThread;

#ifdef Q_OS_UNIX
class QTimer;
#endif

namespace ActionTools
{
	namespace SystemInput
	{
		class ACTIONTOOLSSHARED_EXPORT Task : public QObject
		{
			Q_OBJECT

		public:
			explicit Task(QObject *parent = nullptr);
            ~Task() override;

			static Task *instance()													{ return mInstance; }

			void emitMouseMotion(int x, int y)										{ emit mouseMotion(x, y); }
			void emitMouseWheel(int intensity)										{ emit mouseWheel(intensity); }
			void emitMouseButtonPressed(ActionTools::SystemInput::Button button)	{ emit mouseButtonPressed(button); }
			void emitMouseButtonReleased(ActionTools::SystemInput::Button button)	{ emit mouseButtonReleased(button); }

		signals:
			void mouseMotion(int x, int y);
			void mouseWheel(int intensity);
			void mouseButtonPressed(ActionTools::SystemInput::Button button);
			void mouseButtonReleased(ActionTools::SystemInput::Button button);
			void keyPressed(int key);
			void keyReleased(int key);

		public slots:
			void start();
			void stop();

		private slots:
#ifdef Q_OS_UNIX
			void processReplies();
#endif

		private:
			static Task *mInstance;

			QThread *mThread;
			bool mStarted{false};
#ifdef Q_OS_UNIX
			QTimer *mProcessRepliesTimer;
#endif
		};
	}
}

