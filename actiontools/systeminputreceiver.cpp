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

#include "systeminputreceiver.h"
#include "systeminputtask.h"
#include "systeminputlistener.h"

#include <QDebug>
#include <QThread>

namespace ActionTools
{
	namespace SystemInput
	{
		QSharedPointer<Receiver> Receiver::mInstance;

		Receiver &Receiver::instance()
		{
			if(!mInstance)
				mInstance = QSharedPointer<Receiver>(new Receiver());

			return *mInstance;
		}

		Receiver::Receiver()
			: QObject(),
			  
			  mTask(new Task)
		{
			qRegisterMetaType<ActionTools::SystemInput::Button>("ActionTools::SystemInput::Button");

            connect(mTask, &Task::mouseMotion, this, &Receiver::mouseMotion);
            connect(mTask, &Task::mouseWheel, this, &Receiver::mouseWheel);
            //connect(mTask, &Task::keyboardEvent, this, &Receiver::keyboardEvent);//TODO
            connect(mTask, &Task::mouseButtonPressed, this, &Receiver::mouseButtonPressed);
            connect(mTask, &Task::mouseButtonReleased, this, &Receiver::mouseButtonReleased);
		}

		Receiver::~Receiver()
		{
			delete mTask;
		}

		void Receiver::mouseMotion(int x, int y)
		{
			for(Listener *listener: mListeners)
				listener->mouseMotion(x, y);
		}

		void Receiver::mouseWheel(int intensity)
		{
			for(Listener *listener: mListeners)
				listener->mouseWheel(intensity);
		}

		void Receiver::mouseButtonPressed(ActionTools::SystemInput::Button button)
		{
			for(Listener *listener: mListeners)
				listener->mouseButtonPressed(button);
		}

		void Receiver::mouseButtonReleased(ActionTools::SystemInput::Button button)
		{
			for(Listener *listener: mListeners)
				listener->mouseButtonReleased(button);
		}

		void Receiver::keyboardEvent()
		{
			for(Listener *listener: mListeners)
				listener->keyboardEvent();
		}

		void Receiver::startCapture(Listener *listener)
		{
			if(mCaptureCount == 0)
				QMetaObject::invokeMethod(mTask, "start");

			++mCaptureCount;

			mListeners.insert(listener);
		}

		void Receiver::stopCapture(Listener *listener)
		{
			mListeners.remove(listener);

			if(mCaptureCount == 0)
				return;

			--mCaptureCount;

			if(mCaptureCount == 0)
				QMetaObject::invokeMethod(mTask, "stop");
		}
	}
}
