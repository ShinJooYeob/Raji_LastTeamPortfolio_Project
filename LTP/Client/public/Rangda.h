#pragma once
#include "Boss.h"

BEGIN(Client)
class CRangda final : public CBoss
{
private:
	CRangda(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CRangda(const CRangda& rhs);
	virtual ~CRangda() = default;

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
	virtual _fVector Get_BonePos(const char* pBoneName) override;
	virtual _fMatrix Get_BoneMatrix(const char* pBoneName) override;

	//Left
	CCollider*				m_pHand_L_Collider = nullptr;	
	ATTACHEDESC				m_LeftAttachedDesc;

	//Right
	CCollider*				m_pHand_R_Collider = nullptr;
	ATTACHEDESC				m_RightAttachedDesc;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	
	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0; 
	_uint				m_iAniNum = 0;

	_bool				m_bIsHalf = false;
	_bool				m_bIsLookAt = true;
	_bool				m_bIsNailAttack = false;
	_bool				m_bIsNailHit = false;

	CGameObject*		m_pPlayerObj;

	_float				m_fTestHPIndex = 1.f;

	_int				m_iMaterialCount = 3;

	_float3				m_vFingerPoss[8];

private:
	_float3	Get_FingerPos(_int Num);

private:
	HRESULT SetUp_Components();
	HRESULT	Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CRangda* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END