#pragma once

#include "MapObject.h"


BEGIN(Client)

class CStaticInstanceMapObject :public CMapObject
{
public:
	typedef struct tagInstanceMap
	{
		CModel*						pModel = nullptr;
		CModelInstance*				pModelInstance = nullptr;
		vector<CTransform*>			pvecTransform;

		_bool						bIsOcllusion = false;
		_float						fFrustumRange = -1;
		_uint						iPassIndex = 2;
	}INSTMAP;


private:
	CStaticInstanceMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CStaticInstanceMapObject(const CStaticInstanceMapObject& rhs);
	virtual ~CStaticInstanceMapObject() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;
	  

public:
	HRESULT	Add_InstanceMapObject(OBJELEMENT& tElements);





private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;

	map<wstring, INSTMAP>				m_mapInstancMapObjects;
	typedef map<wstring, INSTMAP>		INSTMAPOBJECTS;


private:
	HRESULT SetUp_Components();



public:
	static CStaticInstanceMapObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};


END