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

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CCollider*			m_pCollider = nullptr;

private:
	_float				m_fAliveTime = 3.f;

private:
	HRESULT		SetUp_Components();


public:
	static CflameTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END