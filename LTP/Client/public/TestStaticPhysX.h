#pragma once
#include "GameObject.h"



BEGIN(Client)
// For PHysX


class CTestStaticPhysX final : public CGameObject
{
public:
	enum E_PHYTYPE
	{
		E_PHYTYPE_TESTBOX,
		E_PHYTYPE_BULLET,
		E_PHYTYPE_Chain,
		E_PHYTYPE_END,
	};
	
	typedef struct Tag_CTestStaticPhysXDesc
	{
		_float3		pos;
		E_PHYTYPE	ePhyType;

	}TESTPHYSXDESC; 


private:
	CTestStaticPhysX(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTestStaticPhysX(const CTestStaticPhysX& rhs);
	virtual ~CTestStaticPhysX() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;
	virtual HRESULT Set_InitPhyType(E_PHYTYPE e);

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;




	

private:
	// 피직스 관련
	CTransform*			m_pTransformCom = nullptr;
//	PxTransform			mTrans;


	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;

	class CCollider_PhysX* m_pPhysX = nullptr;

	E_PHYTYPE mePhyType = E_PHYTYPE_END;

private:
	HRESULT SetUp_Components();

	void Set_StaticBox();
	void Set_DynamicBullet();


public:
	static CTestStaticPhysX* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CTestStaticPhysX* Clone(void* pArg);
	virtual void Free() override;
};
END
