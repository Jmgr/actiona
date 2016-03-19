/*
	Actiona
    Copyright (C) 2005-2016 Jonathan Mercier-Ganady

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
			  mCaptureCount(0),
			  mTask(new Task)
		{
			qRegisterMetaType<ActionTools::SystemInput::Button>("ActionTools::SystemInput::Button");

			connect(mTask, SIGNAL(mouseMotion(int,int)), this, SLOT(mouseMotion(int,int)));
			connect(mTask, SIGNAL(mouseWheel(int)), this, SLOT(mouseWheel(int)));
			//connect(mTask, SIGNAL(keyboardEvent()), this, SLOT(keyboardEvent()));//TODO
			connect(mTask, SIGNAL(mouseButtonPressed(ActionTools::SystemInput::Button)), this, SLOT(mouseButtonPressed(ActionTools::SystemInput::Button)));
			connect(mTask, SIGNAL(mouseButtonReleased(ActionTools::SystemInput::Button)), this, SLOT(mouseButtonReleased(ActionTools::SystemInput::Button)));
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
