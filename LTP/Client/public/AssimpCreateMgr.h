#pragma once

#include "Base.h"

BEGIN(Client)

// Load And Create
class CAssimpCreateMgr:public CBase
{
public:
	DECLARE_SINGLETON(CAssimpCreateMgr)

private:
	explicit CAssimpCreateMgr();
	virtual ~CAssimpCreateMgr() = default;

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

public:
	HRESULT Initalize(ID3D11Device* d,ID3D11DeviceContext* c);
	HRESULT	Load_ALL_Modele();

	const wchar_t* GetName_Iter_Plus();

private:
	HRESULT	Load_Model_DatFile();
	HRESULT	Load_ModelList(const list<MYFILEPATH*>& pathlist, list<MODELDESC*>& List_Modeldesc);
	HRESULT	Load_ModelMap(const list<MYFILEPATH*>& pathlist, map<const wchar_t*,MODELDESC*>& Map_Modeldesc);
	HRESULT Create_ModelCom(map<const wchar_t*, MODELDESC*>& map, SCENEID sceneid, CModel::MODELTYPE type,_Matrix defaultMat);

	

private:
	list<MYFILEPATH*> mFile_StaticList;
	list<MYFILEPATH*> mFile_DynamicList;

	list<const wchar_t*> mList_Name_ModelCom;

	map<const wchar_t*, MODELDESC*> mMap_StaticModelDesc;
	map<const wchar_t*, MODELDESC*> mMap_DynamicModelDesc;

	//list<MYFILEPATH*>::iterator mCurrentIter_Dynamic;
	//list<MYFILEPATH*>::iterator mCurrentIter_Static;

	list<const wchar_t*>::iterator mCurrent_NameIter;

public:
	virtual void Free()override;


};
END