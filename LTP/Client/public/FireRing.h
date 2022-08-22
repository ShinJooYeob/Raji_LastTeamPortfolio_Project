#pragma once

#include "GameObject.h"

BEGIN(Client)

class CFireRing final : public CGameObject
{
private:
	explicit CFireRing(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CFireRing(const CFireRing& rhs);
	virtual ~CFireRing() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

	virtual void		CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
						_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;
private:
	void				Update_Collider(_double fDeltaTime);

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_Colliders();
	HRESULT				SetUp_Etc();

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CCollider*				m_pCollider = nullptr;


private:
	NONINSTNESHEFTDESC m_tVolcanoDesc;
	_float				m_fCurTime_Duration = 0;
public:
	static CFireRing*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
