#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CTestObject final : public CGameObject
{
private:
	CTestObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTestObject(const CTestObject& rhs);
	virtual ~CTestObject() = default;

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
	CNavigation*		m_pNavigationCom = nullptr;


private:
	HRESULT SetUp_Components();

public:
	static CTestObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
