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

#pragma once

#include "actiontools_global.h"
#include "codetools.h"
#include "codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QByteArray>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT RawData : public CodeClass
	{
		Q_OBJECT
	
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(const RawData &other, QScriptEngine *engine);
		static QScriptValue constructor(const QByteArray &byteArray, QScriptEngine *engine);

		static void registerClass(QScriptEngine *scriptEngine);
		
		RawData();
		RawData(const RawData &other);
		RawData(const QByteArray &byteArray);
		
		RawData &operator=(RawData other);
		RawData &operator=(QByteArray byteArray);
		
		void swap(RawData &other);
		void swap(QByteArray &byteArray);
		
		const QByteArray &byteArray() const;

        int additionalMemoryCost() const override { return mByteArray.size(); }
	
	public slots:
		QScriptValue clone() const;
		bool equals(const QScriptValue &other) const override ;
		QString toString() const override ;
		QScriptValue append(const QVariant &data);
		QScriptValue chop(int n);
		QScriptValue clear();
		bool contains(const QVariant &data);
		int count(const QVariant &data) const;
		bool endsWith(const QVariant &data) const;
		int indexOf(const QVariant &data, int from = 0) const;
		bool isEmpty() const;
		int lastIndexOf(const QVariant &data) const;
		QScriptValue left(int len) const;
		int length() const;
		QScriptValue mid(int pos, int len = -1) const;
		QScriptValue prepend(const QVariant &data);
		QScriptValue remove(int pos, int len);
		QScriptValue replace(const QString &before, const QString &after);
		QScriptValue resize(int size);
		QScriptValue right(int len) const;
		QScriptValue setData(const QVariant &data);
		int size() const;
		bool startsWith(const QVariant &data);
		double convertToNumber() const;
		QString convertToString(Encoding encoding = Native) const;
		QScriptValue truncate(int pos);
		
	private:
		QByteArray mByteArray;
	};
}

