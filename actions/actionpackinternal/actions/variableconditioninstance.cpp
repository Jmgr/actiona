/*
	Actiona
	Copyright (C) 2005-2017 Jonathan Mercier-Ganady

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

#include "variableconditioninstance.h"

namespace Actions
{
	ActionTools::StringListPair VariableConditionInstance::comparisons = qMakePair(
			QStringList() << QStringLiteral("equal") << QStringLiteral("different") << QStringLiteral("inferior") << QStringLiteral("superior") << QStringLiteral("inferiorEqual") << QStringLiteral("superiorEqual") << QStringLiteral("contains"),
			QStringList()
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", QStringLiteral("Equal (=)"))
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", QStringLiteral("Different (!=)"))
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", QStringLiteral("Inferior (<)"))
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", QStringLiteral("Superior (>)"))
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", QStringLiteral("Inferior or equal (<=)"))
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", QStringLiteral("Superior or equal (>=)"))
			<< QT_TRANSLATE_NOOP("VariableConditionInstance::comparisons", QStringLiteral("Contains")));
}
