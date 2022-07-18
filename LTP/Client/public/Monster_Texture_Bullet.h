#pragma once

#include "Monster_Bullet.h"

BEGIN(Client)
class CMonster_Texture_Bullet  final : public CMonster_Bullet
{
public:
	enum Monster_TextureType {
		JALSURA_BULLET,
		GADASURA_TERRAIN_BULLET,
		MONSTER_TEXUTRE_BULLET_END
	};
public:
	typedef struct tagMonster_Bullet_UniversalDesc {
		_uint			iBulletTextureNumber;

		_float			fSpeedPerSec;
		_float3			fScale;
		_float3			fLook;

		CTransform*		Object_Transform = nullptr;

		_float3			fPositioning;

		void*			Object = nullptr;

		_double			dDuration;

	}MONSTER_TEXTURE_BULLETDESC;
private:
	explicit CMonster_Texture_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMonster_Texture_Bullet(const CMonster_Texture_Bullet& rhs);
	virtual ~CMonster_Texture_Bullet() = default;

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
	HRESULT SetUp_Fire(_double dDeltaTime);


private: //Monster Skill
	HRESULT Jalsura_Bullet(_double dDeltaTime);
	HRESULT	Gadasura_Terrain_Bullet(_double dDeltaTime);

private://Paticle
	HRESULT Gadasura_Terrain_Particle();

private:
	MONSTER_TEXTURE_BULLETDESC m_Monster_Texture_BulletDesc; //Monster_Bullet_Universal Desc;
	ATTACHEDESC			m_AttachedDesc; //Born Desc;
	INSTMESHDESC		m_MeshEffectDesc; //Mesh Effect

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	CTransform*			m_pPlayerTransform = nullptr;

	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
private:
	_float4				m_fTempPos;
	_float3				m_fTempLook;
	_float4				m_fTempPlayerPos;

private:
	_double				m_dDeltaTime = 0;

private:
	_bool				m_bOnceSwtich = false;



private: //Jalsura Dis
	_float				m_fTempDis = 0;
	_bool				m_bHitOn = false;

public:
	static CMonster_Texture_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END