#include "FileInfoMgr.h"
#include "io.h"
#include <sstream>

IMPLEMENT_SINGLETON(CFileInfoMgr);


static int isFileOrDir(_finddatai64_t fd)
{
	/*
	#define _A_NORMAL 0x00 // Normal file - No read/write restrictions
	#define _A_RDONLY 0x01 // Read only file
	#define _A_HIDDEN 0x02 // Hidden file
	#define _A_SYSTEM 0x04 // System file
	#define _A_SUBDIR 0x10 // Subdirectory
	#define _A_ARCH   0x20 // Archive file
	*/

	// 파일인지 디렉토리인지 확인

	if (fd.name[0] == '.')
		return 2;

	if (fd.attrib == 16)
	{
		return 0;
	}
	else if (fd.attrib & 32)
	{
		return 1;
	}

	return 2;
}



HRESULT CFileInfoMgr::FolderFinder(const wstring& FileFolder)
{
	wstring	wstrFilePath = FileFolder + L"\\*.*";
	string strFilePath;
	strFilePath.assign(wstrFilePath.begin(), wstrFilePath.end());
	//	wstrFilePath.assign(strFilePath.begin(), strFilePath.end());

	struct _finddatai64_t fd;

	intptr_t handle = _findfirsti64(strFilePath.c_str(), &fd);

	do
	{
		int checkDirFile = isFileOrDir(fd);//현재 객체 종류 확인(파일 or 디렉토리)

		// 디렉토리라면 재귀
		switch (checkDirFile)
		{
		case 0: // 디렉토리
		{
			string filename = fd.name;
			wstring newFolderPath = FileFolder;
			wstring newDirName;
			newDirName.assign(filename.begin(), filename.end());

			FolderFinder(newFolderPath + L"\\" + newDirName);
		}
		break;
		case 1: // 파일
		{
			MYFILEPATH* myFilePath = NEW MYFILEPATH;

			string filename = fd.name;
			wstring newFullPath = FileFolder;
			wstring Filename;
			Filename.assign(filename.begin(), filename.end());
			SaveFilePathByVector(myFilePath, newFullPath + L"\\" + Filename, Filename);

			mListFilePathData.push_front(myFilePath);
		}
		break;
		case 2: // 불가능
			break;
		}
	} while (_findnexti64(handle, &fd) == 0);

	_findclose(handle);

	return S_OK;
}

void CFileInfoMgr::SaveFilePathByVector(MYFILEPATH * path, wstring filepath, wstring filename)
{
	if (path == nullptr)
		path = NEW MYFILEPATH;

	lstrcpy(path->FullPath, filepath.c_str());
	lstrcpy(path->FileName, filename.c_str());

	wstring exeName = L"";
	for (int i = 0; i < filename.size(); i++)
	{
		if (filename[i] == L'.')
		{
			exeName += filename.substr(i + 1, filename.size() - 1);
			break;
		}
	}

	lstrcpy(path->Extension, exeName.c_str());
	path->FileCount = 1;
}

void CFileInfoMgr::SaveVectorToDat(wstring savetxtName, wstring ExtensionName)
{
	bool isAllFile = true;
	if (ExtensionName.size() > 2)
		isAllFile = false;

	// txt파일에 전체 리소스 정보 저장
	if (mListFilePathData.empty())
		return;

	ofstream fWrite(savetxtName, ios::out);
	if (fWrite.is_open())
	{
		for (auto pathdata : mListFilePathData)
		{
			wstring Fullpath = pathdata->FullPath;
			wstring FileName = pathdata->FileName;
			wstring Extension = pathdata->Extension;

			if (isAllFile == false && Extension != ExtensionName)
				continue;

			_tchar buf[16] = L"";
			_itow_s(pathdata->FileCount, buf, 10);
			wstring FileCount = buf;

			wstring filetxt =
				Fullpath + L'|' +
				FileName + L'|' +
				Extension + L'|' +
				FileCount + L'|';

			// 문서 저장
			string outText = CHelperClass::to_utf8(filetxt);
			fWrite << outText << "\n";
		}
	}
	if (mListFilePathData.empty() == false)
	{
		for (auto pathdata : mListFilePathData)
		{
			Safe_Delete(pathdata);
		}
		mListFilePathData.clear();
	}
	fWrite.close();
}

list<MYFILEPATH*> CFileInfoMgr::Load_ExtensionList(wstring txtfilepath, string exe,bool bflag)
{
	list<MYFILEPATH*> pngPathList;

	ifstream fRead(txtfilepath, ios::in);

	if (false == fRead.is_open())
		return pngPathList;

	string line;
	while (!fRead.eof())
	{
		getline(fRead, line);
		vector<string> vecStr = CHelperClass::StringSplit(line, '|');
		if (vecStr.size() != 4)
			continue;
		if (bflag)
		{
			if (vecStr[2] != exe)
				continue;
		}
		else
		{
			if (vecStr[2] == exe)
				continue;
		}
		MYFILEPATH* path = NEW MYFILEPATH;

		vector<wstring>  vecWStr;
		vecWStr.reserve(vecStr.size());

		for (auto str : vecStr)
		{
			wstring wstr;
			wstr.assign(str.begin(), str.end());
			vecWStr.push_back(wstr);
		}

		lstrcpy(path->FullPath, vecWStr[0].c_str());
		lstrcpy(path->FileName, vecWStr[1].c_str());
		lstrcpy(path->Extension, vecWStr[2].c_str());
		path->FileCount = 1;
		pngPathList.push_front(path);
	}

	fRead.close();

	return pngPathList;
}

wstring CFileInfoMgr::Get_PathData(wstring Fullpath)
{
	wstring returnPath = L"";
	wstring::iterator endIter = Fullpath.end();
	endIter--;
	_uint cnt = (_uint)Fullpath.length();

	for (; endIter != Fullpath.begin(); endIter--)
	{
		if (Fullpath[cnt--] == '\\')
		{
			endIter++;
			break;
		}
	}

	returnPath.assign(Fullpath.begin(), endIter);
	returnPath += '\\';

	return returnPath;
}

void CFileInfoMgr::Free()
{
	if (mListFilePathData.empty() == false)
	{
		for (auto pathdata : mListFilePathData)
		{
			Safe_Delete(pathdata);
		}
		mListFilePathData.clear();
	}
}

//l CFileInfoMgr::FindType(CFileFind& Find, E_FILETYPE type)
//{
//	wstring StrCompare;
//	wstring StrFiletype = Find.GetFileName().Right(3);
//	// 특정 확장자만 찾음
//	switch (type)
//	{
//	case FILETYPE_PNG:
//		StrCompare = L"png";
//		break;
//	case FILETYPE_DDS:
//		StrCompare = L"dds";
//		break;
//	case FILETYPE_XML:
//		StrCompare = L"xml";
//		break;
//	case FILETYPE_DAT:
//		StrCompare = L"dat";
//		break;
//	}
//
//	if (StrFiletype != StrCompare)
//		return  false;
//	return true;
//}