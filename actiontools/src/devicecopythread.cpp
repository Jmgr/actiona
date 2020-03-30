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

#include "actiontools/devicecopythread.hpp"

#include <QIODevice>
#include <QSharedPointer>

namespace ActionTools
{
	DeviceCopyThread::DeviceCopyThread(QIODevice *input, QIODevice *output)
		: mInput(input),
		mOutput(output),
		mCopiedData(0),
		mContinue(true)
	{
	}
	
	void DeviceCopyThread::stop()
	{
		disconnect();
		
		QMutexLocker mutexLocker(&mMutex);
		
		mContinue = false;
	}
	
	qint64 DeviceCopyThread::copiedData() const
	{
		QMutexLocker mutexLocker(&mMutex);
		
		return mCopiedData;
	}
		
	void DeviceCopyThread::run()
	{
		//Try to allocate as much as possible
		int bufferSize = 1024 * 1024;
		QSharedPointer<char> buffer;

		while(!buffer)
		{
			try
			{
				buffer = QSharedPointer<char>(new char[bufferSize]);
			}
			catch(const std::exception &)
			{
				buffer.clear();
				bufferSize /= 5;
				continue;
			}
		}

		int size;

		while(!mInput->atEnd())
		{
			size = mInput->read(buffer.data(), bufferSize);
			
			mOutput->write(buffer.data(), size);

			{
				QMutexLocker mutexLocker(&mMutex);
				mCopiedData += size;

				if(!mContinue)
					return;
			}
			
			msleep(1);
		}
	}
}
