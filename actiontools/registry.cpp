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

#include "registry.h"

#include <QtEndian>
#include <QStringList>

#ifdef Q_OS_WIN

namespace ActionTools
{
	Registry::ReadResult Registry::read(QVariant &result, Key key, const QString &subkey, const QString &value)
	{
		HKEY hKey;

		if(RegOpenKeyEx(enumToKey(key), subkey.toStdWString().c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
			return ReadCannotFindSubKey;

		DWORD size;
		DWORD type;
		std::wstring wideValue = value.toStdWString();
		if(RegQueryValueEx(hKey, wideValue.c_str(), 0, &type, 0, &size) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return ReadCannotFindValue;
		}

		switch(type)
		{
		case REG_DWORD:
		case REG_DWORD_BIG_ENDIAN:
			{
				qint32 value;
				if(RegQueryValueEx(hKey, wideValue.c_str(), 0, 0, reinterpret_cast<LPBYTE>(&value), &size) != ERROR_SUCCESS)
				{
					RegCloseKey(hKey);
					return ReadCannotFindValue;
				}

				if(type == REG_DWORD_BIG_ENDIAN)
					value = qFromBigEndian(value);

				result = value;
			}
			break;
		case REG_SZ:
		case REG_EXPAND_SZ:
		case REG_LINK:
		case REG_MULTI_SZ:
			{
                std::vector<wchar_t> buffer(size);
                if(RegQueryValueEx(hKey, wideValue.c_str(), 0, 0, reinterpret_cast<LPBYTE>(buffer.data()), &size) != ERROR_SUCCESS)
				{
					RegCloseKey(hKey);
					return ReadCannotFindValue;
				}

				if(type == REG_MULTI_SZ)
				{
                    QStringList stringList = QString::fromWCharArray(buffer.data(), size / 2).split(QChar(L'\0'), QString::SkipEmptyParts);

					if(stringList.last().isEmpty())
						stringList.removeLast();

					result = stringList;
				}
				else
                    result = QString::fromWCharArray(buffer.data(), size / 2);
			}
			break;
		case REG_BINARY:
			{
                std::vector<char> buffer(size);
                if(RegQueryValueEx(hKey, wideValue.c_str(), 0, 0, reinterpret_cast<LPBYTE>(buffer.data()), &size) != ERROR_SUCCESS)
				{
					RegCloseKey(hKey);
					return ReadCannotFindValue;
				}

                result = QByteArray::fromRawData(buffer.data(), size);
			}
			break;
		case REG_QWORD:
			{
				qint64 value;
				if(RegQueryValueEx(hKey, wideValue.c_str(), 0, 0, reinterpret_cast<LPBYTE>(&value), &size) != ERROR_SUCCESS)
				{
					RegCloseKey(hKey);
					return ReadCannotFindValue;
				}

				result = value;
			}
			break;
		case REG_NONE:
		default:
			RegCloseKey(hKey);

			return ReadInvalidValueType;
			break;
		}

		RegCloseKey(hKey);

		return ReadOk;
	}

	Registry::WriteResult Registry::write(const QVariant &data, Key key, const QString &subkey, const QString &value)
	{
		HKEY hKey;

		if(RegOpenKeyEx(enumToKey(key), subkey.toStdWString().c_str(), 0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
			return WriteCannotFindSubKey;

		std::wstring wideValue = value.toStdWString();

		switch(data.type())
		{
		case QVariant::Int:
		case QVariant::UInt:
			{
				int intData = data.toInt();
				if(RegSetValueEx(hKey, wideValue.c_str(), 0, REG_DWORD, reinterpret_cast<LPBYTE>(&intData), sizeof(int)) != ERROR_SUCCESS)
				{
					RegCloseKey(hKey);
					return WriteCannotWriteValue;
				}
			}
			break;
		case QVariant::LongLong:
		case QVariant::ULongLong:
			{
				long long intData = data.toLongLong();
				if(RegSetValueEx(hKey, wideValue.c_str(), 0, REG_QWORD, reinterpret_cast<LPBYTE>(&intData), sizeof(long long)) != ERROR_SUCCESS)
				{
					RegCloseKey(hKey);
					return WriteCannotWriteValue;
				}
			}
			break;
		case QVariant::StringList:
			{
				const QStringList &stringList = data.toStringList();
				std::wstring wideData;

				for(const QString &string: stringList)
				{
					wideData += string.toStdWString();
					wideData += L'\0';
				}

				if(RegSetValueEx(hKey, wideValue.c_str(), 0, REG_MULTI_SZ, reinterpret_cast<LPBYTE>(const_cast<wchar_t*>(wideData.c_str())), static_cast<DWORD>(wideData.size() * sizeof(wchar_t))) != ERROR_SUCCESS)
				{
					RegCloseKey(hKey);
					return WriteCannotWriteValue;
				}
			}
			break;
		case QVariant::ByteArray:
			{
				QByteArray byteArray = data.toByteArray();
				if(RegSetValueEx(hKey, wideValue.c_str(), 0, REG_BINARY, reinterpret_cast<LPBYTE>(byteArray.data()), byteArray.size()) != ERROR_SUCCESS)
				{
					RegCloseKey(hKey);
					return WriteCannotWriteValue;
				}
			}
			break;
		default:
			{
				if(data.type() == QVariant::String || data.canConvert(QVariant::String))
				{
					std::wstring wideData = data.toString().toStdWString();
					if(RegSetValueEx(hKey, wideValue.c_str(), 0, REG_SZ, reinterpret_cast<LPBYTE>(const_cast<wchar_t*>(wideData.c_str())), static_cast<DWORD>(wideData.size() * sizeof(wchar_t))) != ERROR_SUCCESS)
					{
						RegCloseKey(hKey);
						return WriteCannotWriteValue;
					}
				}
				else
				{
					RegCloseKey(hKey);
					return WriteCannotWriteValue;
				}
			}
			break;
		}

		RegCloseKey(hKey);

		return WriteOk;
	}

	HKEY Registry::enumToKey(Key key)
	{
		switch(key)
		{
		case ClassesRoot:
			return HKEY_CLASSES_ROOT;
		case CurrentConfig:
			return HKEY_CURRENT_CONFIG;
		case CurrentUser:
			return HKEY_CURRENT_USER;
		case Users:
			return HKEY_USERS;
		case LocalMachine:
			return HKEY_LOCAL_MACHINE;
		default:
			return HKEY_CURRENT_USER;
		}
	}
}

#endif
