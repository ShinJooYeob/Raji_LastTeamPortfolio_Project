#pragma once

#include "Base.h"


BEGIN(Engine)

class CGameObject;
class CComponent;

class CObjectLayer :public CBase
{
private:
	explicit CObjectLayer();
	virtual ~CObjectLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);

	_int Update(_double fDeltaTime);
	_int LateUpdate(_double fDeltaTime);
	
public:
	list<CGameObject*>*	Get_ObjectList() { return &m_ObjectList; };

	CComponent* Get_Commponent_By_LayerIndex(const _tchar* tagComponet, _uint iLayerIndex);
	CGameObject* Get_GameObject_By_LayerIndex(_uint iLayerIndex);
	CGameObject* Get_GameObject_By_LayerLastIndex();
	

	HRESULT		Delete_GameObject_By_LayerIndex(_uint iIndex);
	HRESULT		Delete_GameObject_By_LayerObject(CGameObject* obj);


	HRESULT Set_NowSceneNum(_uint eScene);


private:
	list< CGameObject*>			m_ObjectList;
	typedef list< CGameObject*>	OBJECTS;

public:
	static CObjectLayer*	Create();
	virtual void Free() override;
};

END