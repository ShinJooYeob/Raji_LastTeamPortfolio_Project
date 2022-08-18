#pragma once
#include "GameObject.h"
#include "MiniGame_Jino_Monster.h"

BEGIN(Client)

class CJumpingMonkey final : public CMiniGame_Jino_Monster
{	
private:
	explicit CJumpingMonkey(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CJumpingMonkey(const CJumpingMonkey& rhs);
	virtual ~CJumpingMonkey() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* pArg)override;
	virtual HRESULT	Initialize_Clone(void* pArg)override;

public:
	virtual _int	Update(_double dDeltaTime)override;
	virtual _int	LateUpdate(_double dDeltaTime)override;
	virtual _int	Render()override;
	virtual _int	LateRender()override;

	virtual void	CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
									_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;


	virtual void			SetUp_MonsterType(EMONSTER_TYPE eMonsterType) override;
	virtual void			Set_Position(_float3 vPos) override;
	virtual void			Dead() override;


public:
	void			Set_Active(_bool bActive);
	void			Set_State_Move();
	void			Set_State_JumpStart();

private:
	void			Update_Move(_double fDeltaTime);
	void			Update_Jump(_double fDeltaTime);

private:
	HRESULT			SetUp_Components();
	HRESULT			SetUp_Colliders();
	HRESULT			SetUp_Etc();

private:
	HRESULT					Update_Collider(_double fDeltaTime);

private:
	CTransform*				m_pPlayerTransform = nullptr;
	_float					m_fAnimSpeed = 0.f;
	_bool					m_bActive = false;

	EANIM_STATE				m_eCurState = ANIM_MOVE;
	_float					m_fJumpStart_Y;
	_float					m_fFallingStart_Y;
	_float					m_fFallingAcc;
	_float					m_fJumpPower;

	_float					m_fCoolTimeJump = 0.f;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CDissolve*				m_pDissolveCom = nullptr;

	CCollider*				m_pCollider = nullptr;
	vector<ATTACHEDESC>		m_vecAttachedDesc;

public:
	static CJumpingMonkey*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free() override;

};

END