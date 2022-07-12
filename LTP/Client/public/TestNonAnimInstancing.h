#pragma once

#include "MapObject.h"


BEGIN(Client)

class CTestNonAnimInstancing :public CMapObject
{
private:
	CTestNonAnimInstancing(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTestNonAnimInstancing(const CTestNonAnimInstancing& rhs);
	virtual ~CTestNonAnimInstancing() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;
	  

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;

	CModelInstance*		m_pModelInstance = nullptr;
	vector<CTransform*> m_vecInstancedTransform;

private:
	HRESULT SetUp_Components();

public:
	static CTestNonAnimInstancing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};


END