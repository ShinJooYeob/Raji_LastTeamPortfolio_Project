#pragma once
#include "Base.h"


// 패스 파인더
BEGIN(Engine)

class CFileInfoMgr
	:public CBase
{
	DECLARE_SINGLETON(CFileInfoMgr);

private:
	CFileInfoMgr() = default;
	~CFileInfoMgr() = default;

public:
	// 폴더를 탐색하면서 모든 파일 리스트 저장
	HRESULT			FolderFinder(const wstring& FileFolder);

	// TXT 파일로 MYFILEPATH 정보 저장 // 경로 파일이름 확장자
	void SaveVectorToDat(wstring savetxtName, wstring ExtensionName = L"");
	void SaveVectorToDat_Particle(wstring savetxtName, wstring ExtensionName = L"");
	
	// 해당 Txt파일을 읽어서 Path 리스트 제작
	list<MYFILEPATH*> Load_ExtensionList(wstring txtfilepath, string exe, bool bFlag =true);

	// Full 패스의 경로만 리턴
	wstring Get_PathData(wstring Fullpath);

private:
	void SaveFilePathByVector(MYFILEPATH * path, wstring filepath, wstring filename);

private:
	list<MYFILEPATH*>		mListFilePathData;

	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};
END


