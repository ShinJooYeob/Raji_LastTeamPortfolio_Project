#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;

END

BEGIN(Client)
class CRangda_MagicCircle final : public CGameObject
{
private:
	CRangda_MagicCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CRangda_MagicCircle(const CRangda_MagicCircle& rhs);
	virtual ~CRangda_MagicCircle() = default;

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

public:
	void		ChangeScaledAndTexture();

private:
	CTransform*			m_pTransformCom = nullptr;

	CCollider*			m_pCollider = nullptr;

private:
	_float				m_fAlpha = 0.f;

	_bool				m_bIsVanish = false;
	_float				m_fVanishTime = 0.5f;


	vector<INSTPARTICLEDESC>		m_vecJYTextureParticleDesc;
	vector<NONINSTNESHEFTDESC>		m_vecJYNonInstMeshDesc;

private:
	HRESULT SetUp_Components();

public:
	static CRangda_MagicCircle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END