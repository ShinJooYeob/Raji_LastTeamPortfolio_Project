#pragma once

#include "GameObject.h"
#include "Camera_Main.h"

BEGIN(Client)

class CGolu_Bullet final : public CGameObject
{
public:
	enum Golu_BulletType{
		FIREBALL,
		BARRIERBULLET,
		BLACKHOLE,
		NONTEXTURE,
		GOLU_BULLET_END
	};
public:
	typedef struct tagGolu_BulletDesc {
		_uint		iGoluBulletType;
		_uint		iTextureIndex;

		_float3		fScale;
		_float3		fPositioning;
		_float		fSpeed;

		_double		dDuration;

		_float3		fDestinationPos;

		_float3		fColliderScale;
		_bool		bColiiderOn;

		void*		pObject = nullptr;
	}GOLU_BULLETDESC;
private:
	explicit CGolu_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGolu_Bullet(const CGolu_Bullet& rhs);
	virtual ~CGolu_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

private:
	HRESULT SetUp_Components();
	HRESULT	SetUp_Info();
	HRESULT	SetUp_Collider();
	HRESULT	Update_Collider(_double dDeltaTime);

private:
	HRESULT PlayOn(_double dDeltaTime);

private: // ETC function
	HRESULT	Billboard();
	HRESULT Magnet();
	_bool	SrcPosToDestPos(_double dDeltaTime, _float fSpeed);
	HRESULT CreateDestPos();

private:
	HRESULT Initialize_Bullet();

	HRESULT FireBall(_double dDeltaTime);
	HRESULT	BarrierBullet(_double dDeltaTime);
	HRESULT BlackHole(_double dDeltaTime);
	HRESULT	FireRing(_double dDeltaTime);


private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;

	CCamera_Main*		m_pCamera = nullptr;
	CTransform*			m_pCameraTransform = nullptr;
	CTransform*			m_pObjectTransform = nullptr;



private:
	GOLU_BULLETDESC		m_Golu_BulletDesc;

	_double				m_dDurationTime = 0;


	_float				m_fAngle = 0;
	_float3				m_vDefaultPos  ;

public:
	static CGolu_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END