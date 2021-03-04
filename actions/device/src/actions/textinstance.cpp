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

#include "textinstance.hpp"
#include "backend/keyboard.hpp"

#include <QTimer>

namespace Actions
{
	TextInstance::TextInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mTimer(new QTimer(this)),
          mCurrentCharacter(0),
          mNoUnicodeCharacters(false)
	{
        mTimer->setTimerType(Qt::PreciseTimer);
        mTimer->setSingleShot(false);

        connect(mTimer, &QTimer::timeout, this, &TextInstance::pressNextKey);
	}

	void TextInstance::startExecution()
	{
        auto &keyboard = Backend::Instance::keyboard();

		bool ok = true;
	
		mText = evaluateString(ok, QStringLiteral("text"));
		int pause  = evaluateInteger(ok, QStringLiteral("pause"));
		mNoUnicodeCharacters = evaluateBoolean(ok, QStringLiteral("noUnicodeCharacters"));

		if(pause < 0)
			pause = 0;

		mTimer->setInterval(pause);
		mCurrentCharacter = 0;
	
		if(!ok)
			return;
		
        try
        {
            if(pause == 0)
            {
                keyboard.writeText(mText, 0, mNoUnicodeCharacters);

                QTimer::singleShot(1, this, [this]
                {
                    executionEnded();
                });
            }
            else
            {
                pressNextKey();

                mTimer->start();
            }
        }
        catch(const Backend::BackendError &e)
        {
            emit executionException(FailedToSendInputException, e.what());
            return;
        }
	}

	void TextInstance::stopExecution()
	{
		mTimer->stop();
	}

	void TextInstance::pressNextKey()
	{
        auto &keyboard = Backend::Instance::keyboard();

        try
        {
            keyboard.writeText(QString(mText.at(mCurrentCharacter)), 0, mNoUnicodeCharacters);
        }
        catch(const Backend::BackendError &e)
        {
            mTimer->stop();
            emit executionException(FailedToSendInputException, e.what());
            return;
        }

		++mCurrentCharacter;
		if(mCurrentCharacter >= mText.size())
		{
			mTimer->stop();

            QTimer::singleShot(1, this, [this]
            {
                executionEnded();
            });

			return;
		}
	}
}
