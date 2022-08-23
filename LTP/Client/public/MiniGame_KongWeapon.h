#pragma once

BEGIN(Client)

class CMiniGame_KongWeapon final : public CGameObject
{
public:
	typedef struct tagKongRajiWeaponDesc
	{
		ATTACHEDESC		eAttachedDesc;
		void*			Object = nullptr;
	}KongRaji_Weapon_UniversalDesc;

private:
	explicit CMiniGame_KongWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMiniGame_KongWeapon(const CMiniGame_KongWeapon& rhs);
	virtual ~CMiniGame_KongWeapon() = default;

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
	vector<ATTACHEDESC>* Get_VecAttachedDesc()
	{
		return &m_vecAttachedDesc;
	}

	CCollider* Get_Collider()
	{
		return m_pColliderCom;
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

private:
	HRESULT MagnetOn(_double dDeltaTime);

private:
	KongRaji_Weapon_UniversalDesc m_KongRaji_Weapon_UniversalDesc; //Monster_Bullet_Universal Desc;
	ATTACHEDESC			m_AttachedDesc; //Born Desc;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CDissolve*			m_pDissolve = nullptr;

private:
	CCollider*			m_pColliderCom = nullptr;
	vector<ATTACHEDESC> m_vecAttachedDesc;

private:
	class CMiniGame_KongRaji* m_pPlayer = nullptr;
	CTransform*			m_pPlayerTransform = nullptr;

private:
	_float4x4			m_fAttachedMatrix;
	CHierarchyNode*		m_pAttachedNode = nullptr;

private:
	_bool				m_WeaponAttackOn = false;

private:
	_bool				m_bMagnet = false;


private:
	_uint				m_iOnceIndex = 0;

	//test!@#@!#$@!$@!$@!$@
//private:
//	_float3				TestPos = _float3(0.f, 0.f, 0.f);

public:
	static CMiniGame_KongWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END