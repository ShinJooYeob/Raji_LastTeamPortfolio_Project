#pragma once
#include "MonsterWeapon.h"


BEGIN(Engine)
class CVIBuffer_Rect;

END

BEGIN(Client)

class CflameTerrain final : public CMonsterWeapon
{
private:
	CflameTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CflameTerrain(const CflameTerrain& rhs);
	virtual ~CflameTerrain() = default;

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
	virtual _float Compute_RenderSortValue()override;
private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CCollider*			m_pCollider = nullptr;

private:
	_float				m_fAliveTime = 7.f;
	_float				m_fPassedTime = 0;
	_float				m_fTotalTargetTime = 7.f;
	_float				m_fDisApearTime = 1.5f;
	_float				m_fApearTime = 0.35f;

	_uint				m_iTextureIndex = 0;
	_bool				m_bCreatedParticle = false;

private:
	HRESULT		SetUp_Components();
	INSTPARTICLEDESC m_FireCrackParticle;

public:
	static CflameTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END