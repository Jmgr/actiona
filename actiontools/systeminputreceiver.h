/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#ifndef SYSTEMINPUTRECEIVER_H
#define SYSTEMINPUTRECEIVER_H

#include "systeminput.h"

#include <QObject>
#include <QSharedPointer>
#include <QSet>
#include <QMetaType>

namespace ActionTools
{
	namespace SystemInput
	{
		class Task;
		class Listener;

		class Receiver : public QObject
		{
			Q_OBJECT

		public:
			class Recorder
			{
			public:
				Recorder(Listener *listener);
				~Recorder();

			private:
				Listener *mListener;
			};

			static Receiver &instance();

			~Receiver();

		private slots:
			void mouseMotion(int x, int y);
			void mouseWheel(int intensity);
			void mouseButtonPressed(Button button);
			void mouseButtonReleased(Button button);
			void keyboardEvent();

		private:
			typedef QSet<Listener *> ListenerSet;

			Receiver();

			void startCapture(Listener *listener);
			void stopCapture(Listener *listener);

			static QSharedPointer<Receiver> mInstance;

			int mCaptureCount;
			ListenerSet mListeners;
			Task *mTask;
		};
	}
}

Q_DECLARE_METATYPE(ActionTools::SystemInput::Button)

#endif // SYSTEMINPUTRECEIVER_H
