/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef KEYBOARDDEVICE_H
#define KEYBOARDDEVICE_H

#include <QSet>
#include <QObject>

class KeyboardDevice : public QObject
{
	Q_OBJECT
	
public:
	enum Action
	{
		Press,
		Release,
		Trigger
	};
	enum Type
	{
		Win32,
		DirectX
	};
	
	KeyboardDevice();
	~KeyboardDevice();

	void reset();
	
	void setType(Type type) { mType = type; }

	bool pressKey(const QString &key);
	bool releaseKey(const QString &key);
	bool triggerKey(const QString &key);
	bool writeText(const QString &text, int delay = 0) const;

private:
	bool doKeyAction(Action action, int nativeKey);
	int stringToNativeKey(const QString &key) const;
	
	QSet<int> mPressedKeys;
	Type mType;
};

#endif // KEYBOARDDEVICE_H
