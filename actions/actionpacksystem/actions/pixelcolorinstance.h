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

#ifndef PIXELCOLORINSTANCE_H
#define PIXELCOLORINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"
#include "script.h"
#include "ifactionvalue.h"

#include <QPoint>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>

namespace Actions
{
	class PixelColorInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum Comparison
		{
			Equal,
			Darker,
			Lighter
		};

		PixelColorInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)											{}

		static ActionTools::StringListPair comparisons;

		void startExecution()
		{
			ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());

			QPoint pixelPosition;
			QColor pixelColorValue;
			Comparison comparison;
			ActionTools::IfActionValue ifTrue;
			ActionTools::IfActionValue ifFalse;
			QString variable;

			if(!actionInstanceExecutionHelper.evaluatePoint(pixelPosition, "pixel", "position") ||
			   !actionInstanceExecutionHelper.evaluateColor(pixelColorValue, "pixel", "color") ||
			   !actionInstanceExecutionHelper.evaluateListElement(comparison, comparisons, "comparison") ||
			   !actionInstanceExecutionHelper.evaluateIfAction(ifTrue, "ifTrue") ||
			   !actionInstanceExecutionHelper.evaluateIfAction(ifFalse, "ifFalse") ||
			   !actionInstanceExecutionHelper.evaluateVariable(variable, "variable"))
				return;

			QString action;
			QString line;
			QColor color = pixelColor(pixelPosition.x(), pixelPosition.y());

			if(!variable.isEmpty())
				actionInstanceExecutionHelper.setVariable(variable, color);

			bool isTrue = true;

			switch(comparison)
			{
			case Equal:
				isTrue = (color == pixelColorValue);
				break;
			case Darker:
				isTrue = (color.lightness() < pixelColorValue.lightness());
				break;
			case Lighter:
				isTrue = (color.lightness() > pixelColorValue.lightness());
				break;
			}

			if(isTrue)
			{
				action = ifTrue.action();
				line = ifTrue.line();
				actionInstanceExecutionHelper.setCurrentParameter("ifTrue", "line");
			}
			else
			{
				action = ifFalse.action();
				line = ifFalse.line();
				actionInstanceExecutionHelper.setCurrentParameter("ifFalse", "line");
			}

			if(action == ActionTools::IfActionValue::GOTO)
				actionInstanceExecutionHelper.setNextLine(line);

			emit executionEnded();
		}

	private:
		QColor pixelColor(int positionX, int positionY) const
		{
			QPixmap pixel = QPixmap::grabWindow(QApplication::desktop()->winId(), positionX, positionY, 1, 1);

			return pixel.toImage().pixel(0, 0);
		}

		Q_DISABLE_COPY(PixelColorInstance)
	};
}

#endif // PIXELCOLORINSTANCE_H
