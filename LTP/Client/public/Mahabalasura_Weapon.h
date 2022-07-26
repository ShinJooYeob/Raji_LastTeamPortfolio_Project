#pragma once
#include "MonsterWeapon.h"

BEGIN(Client)
class CMahabalasura_Weapon final : public CMonsterWeapon
{
public:
	enum CloneType {CLONE_BOSS, CLONE_SKILL, CLONE_INSTANCE, CLONE_END};

	typedef struct WeaponDesc
	{
		CloneType m_CloneType = CLONE_END;
		ATTACHEDESC m_eAttachedDesc;
		CGameObject* MahabalasuraObj = nullptr;
		_float3 Pos = _float3(0.f);
	}WEAPONDESC;
private:
	CMahabalasura_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMahabalasura_Weapon(const CMahabalasura_Weapon& rhs);
	virtual ~CMahabalasura_Weapon() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public:
	HRESULT		Set_InstanceWeapon(_int iCount = 0);
	void		Set_IsStab() { m_bIsStab = true; }
	void		Set_IsDissolveStart() { m_bIsDissolveStart = true; }

	CTransform* Get_Transform()
	{
		return m_pTransformCom;
	}


private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CDissolve*			m_pDissolveCom = nullptr;

	CCollider*			m_pCollider = nullptr;
	
	CGameObject*		m_pPlayerObj = nullptr;
	_float3				m_PlayerPos;

	//ATTACHEDESC			m_eAttachedDesc;
	_float4x4			m_fAttachedMatrix;

	WEAPONDESC			m_WeaponDesc;

	_double				m_dAliveTime = 0.0;
	_float				m_fUpSpeed = 0.f;

	//Dissolve
	_bool				m_bIsDissolveStart = false;


	//InstanceCom
	CShader*			m_pInstanceShaderCom = nullptr;
	CModelInstance*		m_pModelInstance = nullptr;
	vector<CTransform*>	m_vInstanceTransformComs;
	_float3				m_pBossPos;
	_Matrix				m_mBossMatrix;
	_bool				m_bIsStab = false;
	_bool				m_bIsAliveTimeStart = false;
	

private:
	void				Update_AttachMatrix();

private:
	HRESULT				SetUp_Components();

public:
	static CMahabalasura_Weapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END