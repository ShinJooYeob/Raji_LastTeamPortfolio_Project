#pragma once
#include "MonsterWeapon.h"

BEGIN(Client)

class CMonster_Weapon_Universal final : public CMonsterWeapon
{
public:
	enum Monster_Weapon_UniversalType {
		GADASURA_BLACK_WEAPON,
		GADASURA_RAGE_WEAPON,
		MONSTER_WEAPON_UNIVERSAL_END
	};
	typedef struct tagPlayerWeaponDesc
	{
		_uint			iMonsterWeaponMeshNumber;
		ATTACHEDESC		eAttachedDesc;
		void*			Object = nullptr;
	}Monster_Weapon_UniversalDesc;

private:
	explicit CMonster_Weapon_Universal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Weapon_Universal(const CMonster_Weapon_Universal& rhs);
	virtual ~CMonster_Weapon_Universal() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual	void Set_IsDead()override;

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

public:
	tagPlayerWeaponDesc Get_WeaponDesc()
	{
		return m_Monster_Weapon_UniversalDesc;
	}


private:
	HRESULT SetUp_Components();

private:
	HRESULT	SetUp_Info();
	HRESULT	SetUp_Collider();
	HRESULT	SetUp_BoneMatrix();

	HRESULT	Update_AttachMatrix();
	HRESULT	Update_Collider(_double dDeltaTime);

private:
	HRESULT Update_Weapon(_double dDeltaTime);


	HRESULT	Gadasura_Black_Weapon(_double dDeltaTime);
	HRESULT	Gadasura_Rage_Weapon(_double dDeltaTime);

private:
	Monster_Weapon_UniversalDesc m_Monster_Weapon_UniversalDesc; //Monster_Bullet_Universal Desc;
	ATTACHEDESC			m_AttachedDesc; //Born Desc;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CDissolve*			m_pDissolve = nullptr;

private:
	CGameObject*		m_pMonster_Object = nullptr;
	CModel*				m_pMonster_Model = nullptr;

private:
	CCollider*			m_pColliderCom = nullptr;
	vector<ATTACHEDESC> m_vecAttachedDesc;

private:
	_float4x4			m_fAttachedMatrix;
	CHierarchyNode*		m_pAttachedNode = nullptr;

private:
	_bool				m_WeaponAttackOn = false;

public:
	static CMonster_Weapon_Universal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END