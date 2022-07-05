#pragma once
#include "GameObject.h"
#include "Collider_PhysX_Base.h"



BEGIN(Client)
// For PHysX


class CTestObject_PhysX final : public CGameObject
{

private:
	CTestObject_PhysX(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTestObject_PhysX(const CTestObject_PhysX& rhs);
	virtual ~CTestObject_PhysX() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	HRESULT	SetUp_ColliderDesc(CCollider_PhysX_Base::PHYSXDESC desc);
	void	Set_Trigger(bool b) { mbTrigger = b; };

private:
	// 피직스 관련
	CTransform*			m_pTransformCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;

	class CCollider_PhysX_Base*	m_Com_ColliderBase = nullptr;


//	class CCollider_PhysX_Static*	m_Com_ColliderStatic = nullptr;
//	class CCollider_PhysX_Dynamic*	m_Com_ColliderDynamic = nullptr;
//	class CCollider_PhysX_Joint*	m_Com_ColliderJoint = nullptr;

	bool				mbTrigger = false;

private:
	HRESULT SetUp_Components();

	// SetActor
	void SetUp_Physx_Collider();
//	void Set_Actor_Joint();


	// 각각 모양에 따라 정의

	void Set_ChainTest();
	


public:
	static CTestObject_PhysX* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CTestObject_PhysX* Clone(void* pArg);
	virtual void Free() override;
};
END
