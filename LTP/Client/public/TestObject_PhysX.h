#pragma once
#include "GameObject.h"



BEGIN(Client)
// For PHysX


class CTestObject_PhysX final : public CGameObject
{
public:
	enum E_PHYTYPE
	{
		E_PHYTYPE_STATIC,
		E_PHYTYPE_DYNAMIC,
		E_PHYTYPE_JOINT,
		E_PHYTYPE_END

	};

	enum E_PHYDETAIL_TYPE
	{
		E_PHYDETAIL_STATIC_BOX,
		E_PHYDETAIL_STATIC_MESH,
		E_PHYDETAIL_STATIC_TRIIGER,
		E_PHYDETAIL_DYNAMIC_BULLET,
		E_PHYDETAIL_DYNAMIC_CIRCLE,
		E_PHYDETAIL_JOINT_TEST,
		E_PHYDETAIL_END,
	};
	
	typedef struct Tag_CTestStaticPhysXDesc
	{
		_float3				pos = _float3(0, 0, 0);
		_float3				scale = _float3(0, 0, 0);
		E_PHYTYPE			ePhyType = E_PHYTYPE_STATIC;
		E_PHYDETAIL_TYPE	ePhyDetailType = E_PHYDETAIL_STATIC_BOX;
		_bool				bTrigger = false;

	}TESTPHYSXDESC; 


private:
	CTestObject_PhysX(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTestObject_PhysX(const CTestObject_PhysX& rhs);
	virtual ~CTestObject_PhysX() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual HRESULT Set_ColliderType(E_PHYTYPE e);
	virtual HRESULT Set_InitPhyType(E_PHYDETAIL_TYPE e);

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
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

	E_PHYTYPE			mePhyType = E_PHYTYPE_END;
	E_PHYDETAIL_TYPE	mePhydetailType = E_PHYDETAIL_END;
	bool				mbTrigger = false;

private:
	HRESULT SetUp_Components();
	void Set_StaticBox();
	void Set_DynamicBullet();
	void Set_ChainTest();
	


public:
	static CTestObject_PhysX* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CTestObject_PhysX* Clone(void* pArg);
	virtual void Free() override;
};
END
