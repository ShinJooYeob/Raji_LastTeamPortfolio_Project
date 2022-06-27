#pragma once
#include "GameObject.h"



BEGIN(Client)

class CTestStaticObject final : public CGameObject
{
private:
	CTestStaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTestStaticObject(const CTestStaticObject& rhs);
	virtual ~CTestStaticObject() = default;

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
	CTransform*			m_pTransformCom = nullptr;


private:
	HRESULT SetUp_Components();

public:
	static CTestStaticObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
