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

#define UNICODE
#define _UNICODE

#ifdef _WIN64
#error "This program should be compiled as a 32 bit executable"
#endif

#include <windows.h>
#include <Shellapi.h>

enum
{
	LANG_DEFAULT,
	LANG_FR_FR,

	LANG_COUNT
};

enum
{
	TEXT_TITLE_ERROR,
	TEXT_USAGE,
	TEXT_NO_64BIT,
	TEXT_NOT_FOUND,
	TEXT_GENERIC_ERROR,

	TEXT_COUNT
};

static const wchar_t * const texts[TEXT_COUNT][LANG_COUNT] =
{
	{
		L"Error",
		L"Erreur"
	},
	{
		L"Usage: runner language program parameters",
		L"" //This doesn't get translated
	},
	{
		L"This SFX script can only be run on a 64 bit operating system.",
		L"Ce script SFX ne peut être exécuté que sur un système d'exploitation 64 bit."
	},
	{
		L"Please install Actionaz (www.actionaz.org) before executing this SFX script.",
		L"Merci d'installer Actionaz (www.actionaz.org) avant d'exécuter ce script SFX."
	},
	{
		L"Unable to start SFX execution.",
		L"Impossible d'exécuter le script SFX."
	}
};

void showMessageBox(int language, int message)
{
	MessageBox(0, texts[message][language], texts[TEXT_TITLE_ERROR][language], MB_OK | MB_ICONERROR);
}

int wmain(int argc, wchar_t **argv)
{
	const wchar_t *language = 0;
	int languageIndex = LANG_DEFAULT;
	int resultCode;

	if(argc != 4)
	{
		showMessageBox(LANG_DEFAULT, TEXT_USAGE);

		return -1;
	}

	language = argv[1];

	if(wcscmp(language, L"fr_FR") == 0)
		languageIndex = LANG_FR_FR;
	// Add other languages here

	resultCode = (int)ShellExecute(0, L"open", argv[2], argv[3], 0, SW_SHOWDEFAULT);
	if(resultCode <= 32)
	{
		switch(resultCode)
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			showMessageBox(languageIndex, TEXT_NOT_FOUND);
			return -1;

		case ERROR_BAD_FORMAT:
			showMessageBox(languageIndex, TEXT_NO_64BIT);
			return -1;

		default:
			showMessageBox(languageIndex, TEXT_GENERIC_ERROR);
			return -1;
		}
	}

	return 0;
}
