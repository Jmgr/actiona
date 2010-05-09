#include "sevenziparchive.h"

extern "C"
{
#include "7z/7zAlloc.h"
#include "7z/7zCrc.h"
#include "7z/7zExtract.h"
}

namespace Tools
{
	bool SevenZipArchive::mIsCRCGenerated = false;

	SevenZipArchive::SevenZipArchive(QObject *parent)
		: QObject(parent),
		mLastError(None),
		mIsOpen(false)
	{
	}

	SevenZipArchive::~SevenZipArchive()
	{
		cleanup();
	}

	bool SevenZipArchive::open(const QString &filename)
	{
		if(InFile_Open(&mArchiveStream.file, filename.toAscii()))
			return false;

		FileInStream_CreateVTable(&mArchiveStream);
		LookToRead_CreateVTable(&mLookStream, False);

		mLookStream.realStream = &mArchiveStream.s;
		LookToRead_Init(&mLookStream);

		mAllocImp.Alloc = SzAlloc;
		mAllocImp.Free = SzFree;

		mAllocTempImp.Alloc = SzAllocTemp;
		mAllocTempImp.Free = SzFreeTemp;

		if(!mIsCRCGenerated)
		{
			mIsCRCGenerated = true;
			CrcGenerateTable();
		}

		SzArEx_Init(&mDb);

		mIsOpen = true;

		SRes res = SzArEx_Open(&mDb, &mLookStream.s, &mAllocImp, &mAllocTempImp);
		switch(res)
		{
		case SZ_OK:
			mLastError = None;
			break;
		case SZ_ERROR_UNSUPPORTED:
			mLastError = Unsupported;
			break;
		case SZ_ERROR_MEM:
			mLastError = Memory;
			break;
		case SZ_ERROR_CRC:
			mLastError = Crc;
			break;
		default:
			mLastError = Unknown;
			break;
		}

		if(res != SZ_OK)
		{
			cleanup();
			return false;
		}

		return true;
	}

	QString SevenZipArchive::filename(int fileIndex) const
	{
		if(fileIndex < 0 || fileIndex >= count())
			return QString();

		CSzFileItem *fileItem = mDb.db.Files + fileIndex;

		return QString::fromAscii(fileItem->Name);
	}

	int SevenZipArchive::fileSize(int fileIndex) const
	{
		if(fileIndex < 0 || fileIndex >= count())
			return 0;

		CSzFileItem *fileItem = mDb.db.Files + fileIndex;

		return fileItem->Size;
	}

	bool SevenZipArchive::extractToFile(int fileIndex, const QString &outputFilename)
	{
		if(fileIndex < 0 || fileIndex >= count())
			return false;

		CSzFileItem *fileItem = mDb.db.Files + fileIndex;

		if(fileItem->IsDir)
			return false;

		UInt32 blockIndex = 0xFFFFFFFF;
		Byte *outBuffer = 0;
		size_t outBufferSize = 0;
		size_t offset;
		size_t outSizeProcessed;

		SRes res = SzAr_Extract(&mDb, &mLookStream.s, fileIndex,
								&blockIndex, &outBuffer, &outBufferSize,
								&offset, &outSizeProcessed,
								&mAllocImp, &mAllocTempImp);
		if(res != SZ_OK)
			return false;

		CSzFile outFile;
		if(OutFile_Open(&outFile, outputFilename.toAscii()))
		{
			IAlloc_Free(&mAllocImp, outBuffer);
			return false;
		}

		size_t processedSize = outSizeProcessed;
		if(File_Write(&outFile, outBuffer + offset, &processedSize) != 0 || processedSize != outSizeProcessed)
		{
			IAlloc_Free(&mAllocImp, outBuffer);
			return false;
		}

		if(File_Close(&outFile))
		{
			IAlloc_Free(&mAllocImp, outBuffer);
			return false;
		}

		IAlloc_Free(&mAllocImp, outBuffer);

		return true;
	}

	bool SevenZipArchive::extractToByteArray(int fileIndex, QByteArray &outputData)
	{
		if(fileIndex < 0 || fileIndex >= count())
			return false;

		CSzFileItem *fileItem = mDb.db.Files + fileIndex;

		if(fileItem->IsDir)
			return false;

		UInt32 blockIndex = 0xFFFFFFFF;
		Byte *outBuffer = 0;
		size_t outBufferSize = 0;
		size_t offset;
		size_t outSizeProcessed;

		SRes res = SzAr_Extract(&mDb, &mLookStream.s, fileIndex,
								&blockIndex, &outBuffer, &outBufferSize,
								&offset, &outSizeProcessed,
								&mAllocImp, &mAllocTempImp);
		if(res != SZ_OK)
			return false;

		char *data = reinterpret_cast<char *>(outBuffer + offset);
		outputData = QByteArray(data, outSizeProcessed);

		IAlloc_Free(&mAllocImp, outBuffer);

		return true;
	}

	void SevenZipArchive::cleanup()
	{
		if(!mIsOpen)
			return;

		SzArEx_Free(&mDb, &mAllocImp);

		File_Close(&mArchiveStream.file);

		mIsOpen = false;
	}
}
