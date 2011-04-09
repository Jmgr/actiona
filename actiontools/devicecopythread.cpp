/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#include "devicecopythread.h"

#include <QIODevice>

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
		const int bufferSize = 1024 * 100;//Read 100KB each time
		char buffer[bufferSize];
		int size;

		while(!mInput->atEnd())
		{
			size = mInput->read(buffer, bufferSize);
			
			mOutput->write(buffer, size);

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
