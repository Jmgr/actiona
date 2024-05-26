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

#include "actiontools/code/algorithms.hpp"

#include <QCryptographicHash>
#include <QRandomGenerator>

namespace Code
{
    QString Algorithms::md4(const QString &input)
    {
        return QString::fromUtf8(QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Md4).toHex());
    }

    QString Algorithms::md5(const QString &input)
    {
        return QString::fromUtf8(QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Md5).toHex());
    }

    QString Algorithms::sha1(const QString &input)
    {
        return QString::fromUtf8(QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Sha1).toHex());
    }

    void Algorithms::setRandomSeed(qint32 seed)
    {
        QRandomGenerator::global()->seed(seed);
    }

    qint32 Algorithms::randomMax()
    {
        return RAND_MAX;
    }

    qint32 Algorithms::randomInteger()
    {
        return randomInteger_(0, RAND_MAX - 1);
    }

    qint32 Algorithms::randomInteger(int min, int max)
    {
        return randomInteger_(min, max);
    }

    double Algorithms::randomFloat(double min, double max)
    {
        return QRandomGenerator::global()->generateDouble() * (max - min) + min;
    }

    QString Algorithms::randomString(const QJSValue &parameters)
    {
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return {};
        }

        QString characters = QStringLiteral("abcdefghijklmnopqrstuvwxyz0123456789");
        int minLength = 5;
        int maxLength = 15;

        if (parameters.hasProperty(QLatin1String("characters")))
            characters = parameters.property(QLatin1String("characters")).toString();
        if (parameters.hasProperty(QLatin1String("minLength")))
            minLength = parameters.property(QLatin1String("minLength")).toInt();
        if (parameters.hasProperty(QLatin1String("maxLength")))
            maxLength = parameters.property(QLatin1String("maxLength")).toInt();

        QString back;
        int charactersLength = characters.length();
        int finalLength = randomInteger_(minLength, maxLength);
        for (int i = 0; i < finalLength; ++i)
        {
            back += characters.at(randomInteger_(0, charactersLength - 1));
        }

        return back;
    }

    void Algorithms::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Algorithms*>("const Algorithms *");

        CodeClass::registerStaticClass<Algorithms>(QStringLiteral("Algorithms"), scriptEngine);
    }

    int Algorithms::randomInteger_(int min, int max)
    {
        return QRandomGenerator::global()->bounded(min, max + 1);
    }
}
