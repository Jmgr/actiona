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

#include "highresolutiontimer.h"

namespace Tools
{
	int HighResolutionTimer::mLevel = 0;

	HighResolutionTimer::HighResolutionTimer(const QString &taskName)
		: 
		mTaskName(taskName),
		mTextStream(stdout, QIODevice::WriteOnly)
	{
#ifdef Q_OS_WIN
		QueryPerformanceFrequency(&mFrequency);
		mTimeStart.QuadPart = 0;
		mTimeStop.QuadPart = 0;
#else
		mTimeStart.tv_sec = mTimeStart.tv_usec = 0;
		mTimeStop.tv_sec = mTimeStop.tv_usec = 0;
#endif

		if(!mTaskName.isEmpty())
		{
			for(int i = 0; i < mLevel; ++i)
				mTextStream << "-";
			
			mTextStream << ">Profiling [" << mTaskName << "] -> ";
			
			start();
			++mLevel;
		}
	}

	HighResolutionTimer::~HighResolutionTimer()
	{
		if(!mTaskName.isEmpty())
		{
			mTextStream << elapsedSeconds() << "s\n";
			--mLevel;
		}
	}

	void HighResolutionTimer::start()
	{
		mRunning = true;
#ifdef Q_OS_WIN
		QueryPerformanceCounter(&mTimeStart);
#else
		gettimeofday(&mTimeStart, nullptr);
#endif
	}

	void HighResolutionTimer::stop()
	{
		mRunning = false;
#ifdef Q_OS_WIN
		QueryPerformanceCounter(&mTimeStop);
#else
		gettimeofday(&mTimeStop, nullptr);
#endif
	}

	double HighResolutionTimer::elapsedMicroseconds()
	{
		double startTime, endTime;

		if(mRunning)
			stop();

#ifdef Q_OS_WIN
		startTime = mTimeStart.QuadPart * (1000000.0 / mFrequency.QuadPart);
		endTime = mTimeStop.QuadPart * (1000000.0 / mFrequency.QuadPart);
#else
		startTime = (mTimeStart.tv_sec * 1000000.0) + mTimeStart.tv_usec;
		endTime = (mTimeStop.tv_sec * 1000000.0) + mTimeStop.tv_usec;
#endif

		return endTime - startTime;
	}

	double HighResolutionTimer::elapsedMilliseconds()
	{
		return elapsedMicroseconds() * 0.001;
	}

	double HighResolutionTimer::elapsedSeconds()
	{
		return elapsedMicroseconds() * 0.000001;
	}
}
