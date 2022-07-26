#pragma once

#include "Boss.h"

BEGIN(Client)

class CCopyMahabalasura final : public CBoss
{

private:
	CCopyMahabalasura(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCopyMahabalasura(const CCopyMahabalasura& rhs);
	virtual ~CCopyMahabalasura() = default;

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

	virtual _float	Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModel = nullptr;
	//CTransform*			m_pTransformCom = nullptr;

	CCollider*			m_pCollider = nullptr;
	CCollider*			m_pAttackCollider = nullptr;


	CModelInstance*		m_pModelInstance = nullptr;
	vector<CTransform*> m_vecInstancedTransform;
	vector<CTransform*> m_vecRenderInstanceTransform;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;
	_uint				m_iAniNum = 0;


	_float3				m_startPos;
	_bool				m_bIsLookAt = true;

	CGameObject*				m_pPlayerObj;
	class CMahabalasura*		m_pBossObj;
	_int						m_iRandomIndex = 0;
	vector<_int>				m_vecColliderIndexs;

	// Effect
	_uint				m_iEffectAdjustIndex = 0;

	//Sound
	_bool				m_bCopyAttackNarration = false;

private:
	HRESULT SetUp_Components();
	HRESULT	Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CCopyMahabalasura* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END