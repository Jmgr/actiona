/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef SYSTEMINPUTTASK_H
#define SYSTEMINPUTTASK_H

#include <QObject>

#include "systeminput.h"
#include "actiontools_global.h"

class QThread;

#ifdef Q_WS_X11
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
			explicit Task(QObject *parent = 0);
			~Task();

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
#ifdef Q_WS_X11
			void processReplies();
#endif

		private:
			static Task *mInstance;

			QThread *mThread;
			bool mStarted;
#ifdef Q_WS_X11
			QTimer *mProcessRepliesTimer;
#endif
		};
	}
}

#endif // SYSTEMINPUTTASK_H
