#ifndef SEVENZIPARCHIVE_H
#define SEVENZIPARCHIVE_H

#include "tools_global.h"

extern "C"
{
#include "7z/7zFile.h"
#include "7z/7zIn.h"
}

#include <QObject>

namespace Tools
{
	class TOOLSSHARED_EXPORT SevenZipArchive : public QObject
	{
		Q_OBJECT

	public:
		enum Error
		{
			None,
			Unsupported,
			Memory,
			Crc,
			Unknown
		};

		explicit SevenZipArchive(QObject *parent = 0);
		~SevenZipArchive();

		bool open(const QString &filename);
		int count() const										{ return mDb.db.NumFiles; }
		QString filename(int fileIndex) const;
		int fileSize(int fileIndex) const;
		bool extractToFile(int fileIndex, const QString &outputFilename);
		bool extractToByteArray(int fileIndex, QByteArray &outputData);
		void close()											{ cleanup(); }

		Error lastError() const									{ return mLastError; }

	private:
		void cleanup();

		CFileInStream mArchiveStream;
		CLookToRead mLookStream;
		ISzAlloc mAllocImp;
		ISzAlloc mAllocTempImp;
		CSzArEx mDb;
		Error mLastError;
		bool mIsOpen;

		static bool mIsCRCGenerated;
	};
}

#endif // SEVENZIPARCHIVE_H
