#pragma once
#include "Boss.h"

BEGIN(Client)
class CMahabalasura final : public CBoss
{
public:
	enum SkillAttackName {SKILL_SPEAR, SKILL_HAND, SKILL_SPEAR_RAIN, SKILL_COPY, SKILL_END};
private:
	CMahabalasura(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMahabalasura(const CMahabalasura& rhs);
	virtual ~CMahabalasura() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual void CollisionTriger(_uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public:
	void		Set_CopyOff(_bool State) 
	{
		m_bIsTeleport = State; 
		m_bIsCopySkill = State;
	}

	void		Set_Hit() { m_bIsHit = true; }

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	//¸öÃ¼
	CCollider*				m_pCollider = nullptr;
	vector<ATTACHEDESC>		m_vecAttachedDesc;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;
	_uint				m_iAniNum = 0;

	_bool				m_bIsHalf = false;
	_bool				m_bIsWalk = true;
	_bool				m_bIsLookAt = true;

	CGameObject*		m_pPlayerObj = nullptr;
	CTransform*			m_pPlayerTransform = nullptr;
	_bool				m_bIsSpearTurn = false;

	_int				TestNumber = 0;

	//Weapon
	class CMahabalasura_Weapon*				m_pSpear = nullptr;

	//Arm
	_float									m_fArmAttackTime = 10.f;
	_bool									m_bIsArmAttack = false;
	vector<class CMahabalasura_Arms*>		m_pArms;

	//COPY
	_bool									m_bIsCopySkill = false;

	//TELEPORT
	_bool									m_bIsStartTeleport = true;
	_bool									m_bIsTeleport = false;
	_float									m_fTeleportDelay = 0.5f;
	_float									m_fTeleporCoolTime = 7.f;

	//HPBar
	class CHpUI*							m_pHPUI = nullptr;




private:
	HRESULT SetUp_Components();
	HRESULT	Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CMahabalasura* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END