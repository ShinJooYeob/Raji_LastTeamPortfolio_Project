#pragma once

#include "MonsterWeapon.h"


BEGIN(Client)

class CMahabalasura_AttackSpear final : public CMonsterWeapon
{
public:
	typedef struct MBSAtkSprDesc
	{
		_float3 vPosition;
		_float  fTotalTime;

		_uint iKinds = 0;
	}MBSASD;

	typedef struct TransformNTimeNDead
	{
		CTransform*		pTransform = nullptr;
		_float			fPassedTime = 0;
		_bool			bIsDead = false;

	}TTD;


private:
	CMahabalasura_AttackSpear(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMahabalasura_AttackSpear(const CMahabalasura_AttackSpear& rhs);
	virtual ~CMahabalasura_AttackSpear() = default;


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

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModel = nullptr;
	CCollider*			m_pCollider = nullptr;

	CTransform*			m_pPlayerTrs = nullptr;

	CModelInstance*		m_pModelInstance = nullptr;
	vector<CTransform*>		m_vecForRenderTransform;
	vector<TTD>				m_vecInstancedTransform;
	vector<_float4>			m_vecLimLight;
	vector<_float4>			m_vecEmissive;

	_float				m_fPassedTime = 0.f;



	MBSASD				m_tDesc;
private:
	HRESULT		SetUp_Components();
	HRESULT		Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CMahabalasura_AttackSpear* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
