#pragma once

#include "MapObject.h"


BEGIN(Client)

class CStaticMapObject :public CMapObject
{
private:
	CStaticMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CStaticMapObject(const CStaticMapObject& rhs);
	virtual ~CStaticMapObject() = default;

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
	CTransform*			m_pTransformCom = nullptr;
	CModel*				m_pModel = nullptr;
#ifdef _DEBUG
	CCollider*			m_pColliderCom = nullptr;
#endif // _DEBUG

private:
	HRESULT SetUp_Components();

public:
	static CStaticMapObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};


END