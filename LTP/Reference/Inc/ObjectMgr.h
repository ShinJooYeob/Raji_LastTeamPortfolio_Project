#pragma once
#include "Base.h"

BEGIN(Engine)

class CObjectLayer;
class CGameObject;
class CComponent;

class CObjectMgr final :public CBase
{
DECLARE_SINGLETON(CObjectMgr)
private:
	explicit CObjectMgr();
	virtual ~CObjectMgr() = default;

	 
public:
	HRESULT Reserve_Container(_uint _eSceneNum);

	HRESULT Add_GameObject_Prototype(const _tchar* tagPrototype, CGameObject* pPrototype);
	HRESULT Add_GameObject_Out_of_Manager(CGameObject** ppOutGameObj, _uint eSceneNum, const _tchar* tagPrototype, void* pArg);
	HRESULT Add_GameObject_To_Layer(_uint eSceneNum, const _tchar* tagLayer, const _tchar* tagPrototype, void* pArg);
	HRESULT	Add_GameObject_To_Layer_By_Parameter(_uint eSceneNum, const _tchar* tagLayer, CGameObject* pGameObject);
	HRESULT Delete_GameObject_To_Layer_Index(_uint eSceneNum, const _tchar* tagLayer, int index);
	HRESULT Delete_GameObject_To_Layer_Object(_uint eSceneNum, const _tchar* tagLayer, CGameObject* obj);

	HRESULT Clear_Scene_GameObjects(_uint eSceneNum);
	HRESULT Reset_StaticObject_NowSceneNum(_uint eSceneNum);

	CComponent*		Get_Commponent_By_LayerIndex(_uint eSceneNum, const _tchar* tagLayer, const _tchar* tagComponet, _uint iLayerIndex);
	CGameObject*	Get_GameObject_By_LayerIndex(_uint eSceneNum, const _tchar* tagLayer, _uint iLayerIndex);
	CGameObject*	Get_GameObject_By_LayerLastIndex(_uint eSceneNum, const _tchar* tagLayer);
	
	list<CGameObject*>*			Get_ObjectList_from_Layer(_uint iSceneNum, const _tchar* tagLayer);

public:
	_int Update(_double fDeltaTime);
	_int LateUpdate(_double fDeltaTime);


private:
	map<const _tchar*, CGameObject*>			m_mapPrototypes;
	typedef map<const _tchar*, CGameObject*>	PROTOTYPES;

private:
	map<const _tchar*, CObjectLayer*>*				m_mapLayer = nullptr;
	typedef map<const _tchar*, CObjectLayer*>		LAYERS;

	_uint		m_iMaxSceneNum = 0;


private:
	CGameObject*	Find_Prototype(const _tchar* tagPrototype);
	CObjectLayer*	Find_Layer(_uint iSceneNum, const _tchar* tagLayer);

public:
	virtual void Free() override;
};

END