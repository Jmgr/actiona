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

#include "textinstance.h"

#include <QTimer>

namespace Actions
{
	TextInstance::TextInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mTimer(new QTimer(this)),
          mCurrentCharacter(0),
          mNoUnicodeCharacters(false)
	{
        connect(mTimer, &QTimer::timeout, this, &TextInstance::pressNextKey);

		mTimer->setSingleShot(false);
	}

	void TextInstance::startExecution()
	{
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
		
		if(pause == 0)
		{
            if(!mKeyboardDevice.writeText(mText, 0, mNoUnicodeCharacters))
			{
				emit executionException(FailedToSendInputException, tr("Unable to write the text"));
				return;
			}

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

	void TextInstance::stopExecution()
	{
		mTimer->stop();
	}

	void TextInstance::stopLongTermExecution()
	{
		mKeyboardDevice.reset();
	}

	void TextInstance::pressNextKey()
	{
        if(!mKeyboardDevice.writeText(QString(mText.at(mCurrentCharacter)), 0, mNoUnicodeCharacters))
		{
			mTimer->stop();
			emit executionException(FailedToSendInputException, tr("Unable to write the text"));
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
