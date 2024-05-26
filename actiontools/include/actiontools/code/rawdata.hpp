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

#include "actiontools/actiontools_global.hpp"
#include "codeclass.hpp"

#include <QJSValue>
#include <QByteArray>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT RawData : public CodeClass
	{
		Q_OBJECT
	
	public:
        Q_INVOKABLE RawData();
        Q_INVOKABLE RawData(const RawData &other);
		RawData(const QByteArray &byteArray);
		
		RawData &operator=(RawData other);
		RawData &operator=(QByteArray byteArray);
		
		void swap(RawData &other);
		void swap(QByteArray &byteArray);
		
		const QByteArray &byteArray() const;

        Q_INVOKABLE QJSValue clone() const;
        Q_INVOKABLE bool equals(const QJSValue &other) const;
        Q_INVOKABLE QString toString() const override;
        Q_INVOKABLE RawData *append(const QVariant &data);
        Q_INVOKABLE RawData *chop(int n);
        Q_INVOKABLE RawData *clear();
        Q_INVOKABLE bool contains(const QVariant &data);
        Q_INVOKABLE int count(const QVariant &data) const;
        Q_INVOKABLE bool endsWith(const QVariant &data) const;
        Q_INVOKABLE int indexOf(const QVariant &data, int from = 0) const;
        Q_INVOKABLE bool isEmpty() const;
        Q_INVOKABLE int lastIndexOf(const QVariant &data) const;
        Q_INVOKABLE QJSValue left(int len) const;
        Q_INVOKABLE int length() const;
        Q_INVOKABLE QJSValue mid(int pos, int len = -1) const;
        Q_INVOKABLE RawData *prepend(const QVariant &data);
        Q_INVOKABLE RawData *remove(int pos, int len);
        Q_INVOKABLE RawData *replace(const QString &before, const QString &after);
        Q_INVOKABLE RawData *resize(int size);
        Q_INVOKABLE QJSValue right(int len) const;
        Q_INVOKABLE RawData *setData(const QVariant &data);
        Q_INVOKABLE int size() const;
        Q_INVOKABLE bool startsWith(const QVariant &data);
        Q_INVOKABLE double convertToNumber() const;
        Q_INVOKABLE QString convertToString(Encoding encoding = Native) const;
        Q_INVOKABLE RawData *truncate(int pos);

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private:
		QByteArray mByteArray;
	};
}

