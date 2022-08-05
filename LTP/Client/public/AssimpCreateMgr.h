#pragma once

#include "Base.h"

BEGIN(Client)

// Load And Create
class CAssimpCreateMgr:public CBase
{
public:
	typedef struct tag_AssimpThreadDesc
	{
		map<const wchar_t*, MODELDESC*>* mapdesc = nullptr;
		_float4x4 DefaultMat;

		_uint startPos = 0;
		_uint Count = 0;
		_bool isOk = false;

	}ASTHREADDESC;


public:
	DECLARE_SINGLETON(CAssimpCreateMgr)

private:
	explicit CAssimpCreateMgr();
	virtual ~CAssimpCreateMgr() = default;

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

public:
	HRESULT Initalize(ID3D11Device* d,ID3D11DeviceContext* c);
	HRESULT	Load_ALL_Model(_fMatrix staticDefault = XMMatrixIdentity(), _fMatrix dynamicDefault = XMMatrixIdentity());
	HRESULT	Load_Model_One_ByFBXName(const wchar_t* fbxName, _fMatrix DefaultMat = XMMatrixIdentity());
	
	const wchar_t* GetName_Iter_Plus();

	HRESULT Free_VertexData_STATIC();

	HRESULT Save_To_Effect();

private:
	HRESULT Init_ModelName_FileList();
	HRESULT Load_Model_DatFile_One(const wchar_t* fbxName, _fMatrix DefaultMat = XMMatrixIdentity());
	HRESULT	Load_Model_DatFile_All(_fMatrix staticDefault = XMMatrixIdentity(), _fMatrix dynamicDefault = XMMatrixIdentity());
	HRESULT	Load_Model_DatFile_All_Thread(_fMatrix staticDefault = XMMatrixIdentity(), _fMatrix dynamicDefault = XMMatrixIdentity());


	HRESULT	Load_ModelList(const list<MYFILEPATH*>& pathlist, list<MODELDESC*>& List_Modeldesc);
	HRESULT	Load_ModelMap(const list<MYFILEPATH*>& pathlist, map<const wchar_t*, MODELDESC*>& Map_Modeldesc);

	HRESULT Load_ModelFBXName_CreateModel(const wchar_t * fbxName, _fMatrix Default);
	HRESULT Create_ModelCom(map<const wchar_t*, MODELDESC*>& map, SCENEID sceneid, CModel::MODELTYPE type, _Matrix defaultMat);
	HRESULT Create_ModelCom_StaticThread(map<const wchar_t*, MODELDESC*>& map, SCENEID sceneid, _Matrix defaultMat, _uint start, _uint count, CRITICAL_SECTION* _CriSec);
	
	HRESULT Free_VertexData();
	
private:
	list<MYFILEPATH*> mList_DataFIle_Static;
	list<MYFILEPATH*> mList_DataFIle_Dynamic;

	list<const wchar_t*> mList_CreateModelName;

	map<const wchar_t*, MODELDESC*> mMap_StaticModelDesc;
	map<const wchar_t*, MODELDESC*> mMap_DynamicModelDesc;

	list<const wchar_t*>::iterator mCurrent_NameIter;

private: // Thread
	CRITICAL_SECTION*	m_pCriSec = nullptr;
public:
	HRESULT		Processing_LoadStatic(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec, ASTHREADDESC desc);
	HRESULT		DataFree_TEST();

public:
	virtual void Free()override;


};
END