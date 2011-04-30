/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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
#include <QTimer>

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
			: ActionTools::ActionInstance(definition, parent), mComparison(Equal) {}

		static ActionTools::StringListPair comparisons;

		void startExecution()
		{
			ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());

			ActionTools::IfActionValue ifFalse;

			if(!actionInstanceExecutionHelper.evaluatePoint(mPixelPosition, "pixel", "position") ||
			   !actionInstanceExecutionHelper.evaluateColor(mPixelColorValue, "pixel", "color") ||
			   !actionInstanceExecutionHelper.evaluateListElement(mComparison, comparisons, "comparison") ||
			   !actionInstanceExecutionHelper.evaluateIfAction(mIfTrue, "ifTrue") ||
			   !actionInstanceExecutionHelper.evaluateIfAction(ifFalse, "ifFalse") ||
			   !actionInstanceExecutionHelper.evaluateVariable(mVariable, "variable"))
				return;

			if(testPixel())
			{
				actionInstanceExecutionHelper.setCurrentParameter("ifTrue", "line");

				QString line;
				if(!actionInstanceExecutionHelper.evaluateSubParameter(line, mIfTrue.actionParameter()))
					return;

				if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
					actionInstanceExecutionHelper.setNextLine(line);

				emit executionEnded();
			}
			else
			{
				actionInstanceExecutionHelper.setCurrentParameter("ifFalse", "line");

				QString line;
				if(!actionInstanceExecutionHelper.evaluateSubParameter(line, ifFalse.actionParameter()))
					return;

				if(ifFalse.action() == ActionTools::IfActionValue::GOTO)
					actionInstanceExecutionHelper.setNextLine(line);
				else if(ifFalse.action() == ActionTools::IfActionValue::WAIT)
				{
					connect(&mTimer, SIGNAL(timeout()), this, SLOT(checkPixel()));
					mTimer.setInterval(100);
					mTimer.start();
				}
				else
					emit executionEnded();
			}
		}

		void stopExecution()
		{
			mTimer.stop();
		}

	private slots:
		void checkPixel()
		{
			if(testPixel())
			{
				ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());

				QString line;
				if(!actionInstanceExecutionHelper.evaluateSubParameter(line, mIfTrue.actionParameter()))
					return;

				if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
					actionInstanceExecutionHelper.setNextLine(line);

				mTimer.stop();
				emit executionEnded();
			}
		}

	private:
		QPoint mPixelPosition;
		QColor mPixelColorValue;
		Comparison mComparison;
		ActionTools::IfActionValue mIfTrue;
		QString mVariable;
		QTimer mTimer;

		bool testPixel()
		{
			QPixmap pixel = QPixmap::grabWindow(QApplication::desktop()->winId(), mPixelPosition.x(), mPixelPosition.y(), 1, 1);
			QColor pixelColor = pixel.toImage().pixel(0, 0);

			ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
			actionInstanceExecutionHelper.setVariable(mVariable, pixelColor);

			switch(mComparison)
			{
			case Equal:
				return (pixelColor == mPixelColorValue);
			case Darker:
				return (pixelColor.lightness() < mPixelColorValue.lightness());
			case Lighter:
				return (pixelColor.lightness() > mPixelColorValue.lightness());
			}

			return false;
		}

		Q_DISABLE_COPY(PixelColorInstance)
	};
}

#endif // PIXELCOLORINSTANCE_H
