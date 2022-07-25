#pragma once

#include "Monster_Bullet.h"

BEGIN(Client)
class CMonster_Bullet_Universal final : public CMonster_Bullet
{
public:
	enum Monster_Bullet_UniversalType {
		VAYUSURA_LEADER_BULLET,
		TEZABSURA_MINION_BULLET,
		TEZABSURA_PURPLE_DEFAULT_BULLET,
		TEZABSURA_PURPLE_PRIMARY_BULLET,
		TEZABSURA_BOMBER_DEFAULT_BULLET,
		TEZABSURA_BOMBER_HOWITZER_BULLET,
		TEZABSURA_LANDMINE_DEFAULT_BULLET,
		TEZABSURA_LANDMINE_INSTALL,
		NINJASURA_KNIFE,
		GADASURA_SINKHOLE,
		MONSTER_BULLET_UNIVERSAL_END,
	};

public:
	typedef struct tagMonster_Bullet_UniversalDesc {
		_uint	iBulletMeshNumber;

		_float			fSpeedPerSec;
		_float3			fScale;
		_float3			fLook;

		CTransform*		Object_Transform = nullptr;

		_float3			fPositioning;

		void*			Object = nullptr;

		_double			dDuration;

		_bool			bBornAttachOn = false;
		const char*		pBoneName = nullptr;

}MONSTER_BULLET_UNIVERSALDESC;
private:
	explicit CMonster_Bullet_Universal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Bullet_Universal(const CMonster_Bullet_Universal& rhs);
	virtual ~CMonster_Bullet_Universal() = default;

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

private:
	HRESULT SetUp_Components();

private:
	HRESULT	SetUp_Info();
	HRESULT	SetUp_BoneMatrix();
	HRESULT SetUp_Fire(_double dDeltaTime);
	HRESULT	SetUp_Collider();
	HRESULT	Update_Collider(_double dDeltaTime);

private:
	_Vector Bezier(_Vector StartPoint,_Vector LastPoint, _double dDeltaTime);

private:
	HRESULT	Vayusura_Leader_Bullet(_double dDeltaTime);
	HRESULT	Tezabsura_Minion_Bullet(_double dDeltaTime);
	HRESULT	Tezabsura_Purple_Default_Bullet(_double dDeltaTime);
	HRESULT Tezabsura_Purple_Primary_Bullet(_double dDeltaTime);
	HRESULT	Tezabsura_Bomber_Default_Bullet(_double dDeltaTime);
	HRESULT Tezabsura_Bomber_Howitzer_Bullet(_double dDeltaTime);
	HRESULT Tezabsura_Landmine_Default_Bullet(_double dDeltaTime);
	HRESULT Tezabsura_Landmine_Install(_double dDeltaTime);
	HRESULT	Ninjasura_Knife(_double dDeltaTime);
	HRESULT	Gadasura_Black(_double dDeltaTime);

private:
	MONSTER_BULLET_UNIVERSALDESC m_Monster_Bullet_UniversalDesc; //Monster_Bullet_Universal Desc;
	ATTACHEDESC			m_AttachedDesc; //Born Desc;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;


private:
	CCollider*			m_pColliderCom = nullptr;


	CTransform*			m_pPlayerTransform = nullptr;

private:
	_float4				m_fTempPos;
	_float3				m_fTempLook;  //본인이 바라보고 있던 룩 방향
	_float4				m_fTempPlayerPos;

private:
	_double				m_dDeltaTime = 0;
	_double				m_PassedTime = 0;
	_bool				m_bIsFired = false;

private:
	_double				m_dBezierTime = 0;


private:
	_bool				m_bOnceSwtich = false;

private:
	HRESULT	Ready_JYParticleDesc();
	HRESULT	Update_JYParticleTransform(_double fDeltaTime);
	_float4 vTargetRimLightColor = _float4(0);
	_float4 vOldRimLightColor = _float4(0);
	_float fRimLightPassedTime = _float(0);
	_float fTransformAngle = 0;
	CTransform*												m_pJYTextureParticleTransform = nullptr;
	CTransform*												m_pJYMeshParticleTransform = nullptr;
	vector<INSTPARTICLEDESC>								m_vecJYTextureParticleDesc;
	vector<INSTMESHDESC>									m_vecJYMeshParticleDesc;
	vector<NONINSTNESHEFTDESC>								m_vecJYNonMeshParticleDesc;
	_float													m_StartY = 0;

public:
	static CMonster_Bullet_Universal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END