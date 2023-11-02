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

#pragma once

#include "actiontools/actioninstance.hpp"
#include "tools/stringlistpair.hpp"
#include "actiontools/ifactionvalue.hpp"
#include "actiontools/keyboardkey.hpp"

#include <QList>
#include <QRegularExpression>

class QTimer;

namespace Actions
{
    class KeyboardKeyConditionInstance : public ActionTools::ActionInstance
    {
	Q_OBJECT
    public:
	enum Condition
	{
	    Pressed,
	    NotPressed
	};
	Q_ENUM(Condition)

	KeyboardKeyConditionInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr);

	static Tools::StringListPair conditions;

	void stopExecution() override;
	void startExecution() override;


    private:
	bool areKeysPressed() const;

	QRegularExpression mTitleRegExp;
	ActionTools::IfActionValue mIfTrue;
	Condition mCondition;
	QTimer *mTimer;
	QList<ActionTools::KeyboardKey> mKeyList;

	Q_DISABLE_COPY(KeyboardKeyConditionInstance)
    };
}

