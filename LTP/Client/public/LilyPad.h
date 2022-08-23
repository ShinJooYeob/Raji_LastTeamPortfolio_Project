#pragma once
#include "GameObject.h"

BEGIN(Client)

class CLilyPad final : public CGameObject
{
private:
	explicit CLilyPad(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CLilyPad(const CLilyPad& rhs);
	virtual ~CLilyPad() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

private:
	void				Update_Colliders();

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_Collider();
	HRESULT				SetUp_Etc();

private:
	_float					m_fDuaton = 10.f;
	_bool					m_bScaling = true;
	_float					m_fFallingAcc = 0.f;
	_bool					m_bPlayOnceSound = false;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CCollider*				m_pCollider = nullptr;

public:
	static CLilyPad*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
