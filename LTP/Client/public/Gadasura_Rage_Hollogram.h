#pragma once
#include "Monster.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CGadasura_Rage_Hollogram final : public CMonster
{
public:
	typedef struct tagGadasuraDesc
	{
		void*		Object = nullptr;

		_double		dDuration;
	}GADASURA_HOLLOGRAMDESC;
private:
	explicit CGadasura_Rage_Hollogram(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGadasura_Rage_Hollogram(const CGadasura_Rage_Hollogram& rhs);
	virtual ~CGadasura_Rage_Hollogram() = default;

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

public:
	HRESULT				SetUp_Info();
	HRESULT				SetUp_Collider();

	HRESULT				SetUp_Fight(_double dDeltaTime);
	HRESULT				Update_Collider(_double dDeltaTime);

private: //애니메이션
	HRESULT				PlayAnim(_double dDeltaTime);

private:
	GADASURA_HOLLOGRAMDESC m_HollogramDesc;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

private:
	CCollider*			m_pColliderCom = nullptr;
	vector<ATTACHEDESC> m_vecAttachedDesc;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;

private:
	CTransform*			m_pPlayerTransform = nullptr; //플레이어 트랜스폼 정보
	CTransform*			m_pMonsterTransform = nullptr; //몬스터의 트랜스폼 정보

private:
	_bool				m_bLookAtOn = true; //LookAt On/Off
	_bool				m_bOnceSwitch = false;

private:
	_double				m_dDeadTime = 0;

private://Sound
	_uint				m_iSoundIndex = 0;
	_double				m_dSoundTime = 0;

private:
	HRESULT SetUp_Components();
	HRESULT Adjust_AnimMovedTransform(_double dDeltatime);

public:
	static CGadasura_Rage_Hollogram* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END