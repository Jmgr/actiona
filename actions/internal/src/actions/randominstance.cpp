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

#include "randominstance.hpp"

#include <random>

namespace Actions
{
    struct Generator : public ActionTools::RuntimeStorageItem
    {
        Generator(): gen{std::random_device{}()} {}

        std::mt19937 gen;
    };

    Tools::StringListPair RandomInstance::types =
    {
        {
            QStringLiteral("integer"),
            QStringLiteral("decimal"),
            QStringLiteral("string")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("RandomInstance::types", "Integer")),
            QStringLiteral(QT_TRANSLATE_NOOP("RandomInstance::types", "Decimal")),
            QStringLiteral(QT_TRANSLATE_NOOP("RandomInstance::types", "String"))
        }
    };

    void RandomInstance::startExecution()
    {
        bool ok = true;

        QString variable = evaluateString(ok, QStringLiteral("variable"));
        Type type = evaluateListElement<Type>(ok, types, QStringLiteral("type"));

        if(!ok)
            return;

        auto &generator = storage()->get<Generator>();

        switch(type)
        {
        case Integer:
        {
            int min = evaluateInteger(ok, QStringLiteral("int_minmax"), QStringLiteral("min"));
            int max = evaluateInteger(ok, QStringLiteral("int_minmax"), QStringLiteral("max"));

            if(!ok)
                return;

            if(min > max)
            {
                emit executionException(RangeException, tr("Minimum cannot be greater than maximum"));
                return;
            }

            std::uniform_int_distribution<> distrib(min, max);

            setVariable(variable, distrib(generator.gen));
        }
        break;
        case Decimal:
        {
            double min = evaluateDouble(ok, QStringLiteral("decimal_minmax"), QStringLiteral("min"));
            double max = evaluateDouble(ok, QStringLiteral("decimal_minmax"), QStringLiteral("max"));

            if(!ok)
                return;

            if(min > max)
            {
                emit executionException(RangeException, tr("Minimum cannot be greater than maximum"));
                return;
            }

            std::uniform_real_distribution<double> distrib(min, max);

            setVariable(variable, distrib(generator.gen));
        }
        break;
        case String:
        {
            QString characters = evaluateString(ok, QStringLiteral("string_characters"));
            int length = evaluateInteger(ok, QStringLiteral("string_length"));

            if(!ok)
                return;

            if(!characters.isEmpty())
            {
                QString result;

                for(int i = 0; i < length; i++)
                {
                    std::uniform_real_distribution<double> distrib(0, characters.length() - 1);
                    result += characters[distrib(generator.gen)];
                }

                setVariable(variable, result);
            }
        }
        break;
        }

        executionEnded();
    }

}

Q_DECLARE_METATYPE(Actions::Generator);
