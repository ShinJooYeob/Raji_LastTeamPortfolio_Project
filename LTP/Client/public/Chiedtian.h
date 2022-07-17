#pragma once
#include "Boss.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CChiedtian final : public CBoss
{
public:
	enum AttackNumber{ATTACK_FIRE, ATTACK_SPIN, ATTACK_WHINING, ATTACK_END};
private:
	CChiedtian(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CChiedtian(const CChiedtian& rhs);
	virtual ~CChiedtian() = default;

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

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CNavigation*		m_pNavigationCom = nullptr;

	//¸öÃ¼
	CCollider*				m_pCollider = nullptr;
	vector<ATTACHEDESC>		m_vecAttachedDesc;

	//È­¿°
	CCollider*				m_pFireCollider = nullptr;
	ATTACHEDESC				m_FireAttachedDesc;

	//Á¡ÇÁ
	CCollider*				m_pJumpCollider = nullptr;
	ATTACHEDESC				m_JumpAttachedDesc;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;
	_uint				m_iAniNum = 0;

	_float				m_fAttackCoolTime = 4.f;
	_float				m_fSkillCoolTime = 6.f;
	_bool				m_bIsHalf = false;
	_bool				m_bIsLookAt = true;

	//Jump
	_float				m_fJumpTime = 3.f;
	_bool				m_bIsTurn = false;
	_bool				m_bIsJump = false;



	//Attack
	_bool				m_bIsBasicAttack = false;
	_bool				m_bISkill = false;
	//walk
	_bool				m_bIsWalk = false;
	_float				m_fWalkTime = 3.f;

	//fire Attack
	_bool				m_bIsFireAttack = false;
	_float				m_fFireTime = 10.f;
	//Spin Attack
	_bool				m_bIsSpinAttack = false;
	_float				m_fSpinSpeed = 0.f;
	_float				m_fSpinTime = 14.f;

	class CHpUI*		m_pHPUI = nullptr;


	CGameObject*		m_pPlayerObj;
	_Vector				m_vAngle;

	_float				m_fTestHPIndex = 1.f;
	_int				m_iTest = 0;

	vector<class CChiedtuan_Weapon*>		m_pMainWeapons;
	_bool									m_bIsMainWeaponOff = false;
	vector<class CChiedtuan_Weapon*>		m_pSubWeapons;

private:
	HRESULT SetUp_Components();
	HRESULT	Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CChiedtian* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END