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
#include "actiontools/script.hpp"
#include "actiontools/ifactionvalue.hpp"
#include "actiontools/code/color.hpp"
#include "actiontools/code/codeclass.hpp"

#include <QPoint>
#include <QPixmap>
#include <QApplication>
#include <QTimer>
#include <QScreen>

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

		PixelColorInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent), mComparison(Equal) {}

        static Tools::StringListPair comparisons;

		void startExecution() override
		{
			bool ok = true;

			mPixelPosition = evaluatePoint(ok, QStringLiteral("pixel"), QStringLiteral("position"));
			mPixelColorValue = evaluateColor(ok, QStringLiteral("pixel"), QStringLiteral("color"));
			mComparison = evaluateListElement<Comparison>(ok, comparisons, QStringLiteral("comparison"));
			mIfTrue = evaluateIfAction(ok, QStringLiteral("ifTrue"));
			ActionTools::IfActionValue ifFalse = evaluateIfAction(ok, QStringLiteral("ifFalse"));
			mVariable = evaluateVariable(ok, QStringLiteral("variable"));
			int redTolerance = evaluateInteger(ok, QStringLiteral("redTolerance"));
			int greenTolerance = evaluateInteger(ok, QStringLiteral("greenTolerance"));
			int blueTolerance = evaluateInteger(ok, QStringLiteral("blueTolerance"));
			QPoint positionOffset = evaluatePoint(ok, QStringLiteral("positionOffset"));

			if(!ok)
				return;

			redTolerance = (255 * redTolerance) / 100;
			greenTolerance = (255 * greenTolerance) / 100;
			blueTolerance = (255 * blueTolerance) / 100;

			mMinimumColor = QColor(normalizeColor(mPixelColorValue.red() - redTolerance),
								   normalizeColor(mPixelColorValue.green() - greenTolerance),
								   normalizeColor(mPixelColorValue.blue() - blueTolerance));
			mMaximumColor = QColor(normalizeColor(mPixelColorValue.red() + redTolerance),
								   normalizeColor(mPixelColorValue.green() + greenTolerance),
								   normalizeColor(mPixelColorValue.blue() + blueTolerance));

            mPixelPosition += positionOffset;

			if(testPixel())
			{
				setCurrentParameter(QStringLiteral("ifTrue"), QStringLiteral("line"));

				QString line = evaluateSubParameter(ok, mIfTrue.actionParameter());
				if(!ok)
					return;

                bool stopScript = false;
				if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
					setNextLine(line);
				else if(mIfTrue.action() == ActionTools::IfActionValue::CALLPROCEDURE)
				{
					if(!callProcedure(line))
						return;
                }
                else if(mIfTrue.action() == ActionTools::IfActionValue::STOPEXECUTION)
                {
                    stopScript = true;
                }

                executionEnded(stopScript);
			}
			else
			{
				setCurrentParameter(QStringLiteral("ifFalse"), QStringLiteral("line"));

				QString line = evaluateSubParameter(ok, ifFalse.actionParameter());
				if(!ok)
					return;

                bool stopScript = false;
				if(ifFalse.action() == ActionTools::IfActionValue::GOTO)
				{
					setNextLine(line);
				}
				else if(ifFalse.action() == ActionTools::IfActionValue::CALLPROCEDURE)
				{
					if(!callProcedure(line))
						return;
				}
                else if(ifFalse.action() == ActionTools::IfActionValue::STOPEXECUTION)
                {
                    stopScript = true;
                }
				else if(ifFalse.action() == ActionTools::IfActionValue::WAIT)
				{
                    connect(&mTimer, &QTimer::timeout, this, &PixelColorInstance::checkPixel);
					mTimer.setInterval(100);
					mTimer.start();
                    return;
				}

                executionEnded(stopScript);
			}
		}

		void stopExecution() override
		{
			mTimer.stop();
		}

	private slots:
		void checkPixel()
		{
			if(testPixel())
			{
				bool ok = true;

				QString line = evaluateSubParameter(ok, mIfTrue.actionParameter());
				if(!ok)
					return;

                bool stopScript = false;
				if(mIfTrue.action() == ActionTools::IfActionValue::GOTO)
					setNextLine(line);
				else if(mIfTrue.action() == ActionTools::IfActionValue::CALLPROCEDURE)
				{
					if(!callProcedure(line))
						return;
				}
                else if(mIfTrue.action() == ActionTools::IfActionValue::STOPEXECUTION)
                {
                    stopScript = true;
                }

				mTimer.stop();
                executionEnded(stopScript);
			}
		}

	private:
		QPoint mPixelPosition;
		QColor mPixelColorValue;
		Comparison mComparison;
		ActionTools::IfActionValue mIfTrue;
		QString mVariable;
		QTimer mTimer;
		QColor mMinimumColor;
		QColor mMaximumColor;

		bool testPixel()
		{
            auto screen = QGuiApplication::screenAt(mPixelPosition);
            if(!screen)
                return false;

            auto geometry = screen->geometry();
            auto localX = mPixelPosition.x() - geometry.x();
            auto localY = mPixelPosition.y() - geometry.y();

            QPixmap pixel = screen->grabWindow(0, localX, localY, 1, 1);
			QColor pixelColor = pixel.toImage().pixel(0, 0);

            setVariable(mVariable, Code::CodeClass::construct<Code::Color>(pixelColor, *scriptEngine()));

			switch(mComparison)
			{
			case Equal:
				return (pixelColor.red() >= mMinimumColor.red() && pixelColor.red() <= mMaximumColor.red() &&
						pixelColor.green() >= mMinimumColor.green() && pixelColor.green() <= mMaximumColor.green() &&
						pixelColor.blue() >= mMinimumColor.blue() && pixelColor.blue() <= mMaximumColor.blue());
			case Darker:
				return (pixelColor.lightness() < mPixelColorValue.lightness());
			case Lighter:
				return (pixelColor.lightness() > mPixelColorValue.lightness());
			}

			return false;
		}

		static int normalizeColor(int value)
		{
			if(value < 0)
				value = 0;
			if(value > 255)
				value = 255;

			return value;
		}

		Q_DISABLE_COPY(PixelColorInstance)
	};
}

