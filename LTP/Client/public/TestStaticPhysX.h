#pragma once
#include "GameObject.h"



BEGIN(Client)
// For PHysX
class CTestStaticPhysX final : public CGameObject
{
private:
	CTestStaticPhysX(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTestStaticPhysX(const CTestStaticPhysX& rhs);
	virtual ~CTestStaticPhysX() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	PxTransform		Get_Transform() { return mTrans; }


private:
	// 피직스 관련
	CTransform*			m_pTransformCom = nullptr;
	PxTransform			mTrans;


	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;

	class CCollider_PhysX* m_pPhysX = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT Update_Trans2Px();
	HRESULT Update_Px2Trans();

public:
	static CTestStaticPhysX* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
