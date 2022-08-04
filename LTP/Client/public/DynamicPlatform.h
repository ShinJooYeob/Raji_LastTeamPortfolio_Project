#pragma once

#include "GameObject.h"

BEGIN(Client)


class CDynamicPlatform final : public CGameObject
{
public:
	enum EONTARGET { TARGET_RAJI, TARGET_GOLU, TARGET_END };

	typedef struct tagDynamicPlatformDesc
	{
		_float3			fSpawnPos;
		EONTARGET		eOnTarget;
	}DYNAMICPLATFORMDESC;

private:
	explicit CDynamicPlatform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CDynamicPlatform(const CDynamicPlatform& rhs);
	virtual ~CDynamicPlatform() = default;

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
	_int				Update_OnTarget_Raji(_double fDeltaTime);
	_int				Update_OnTarget_Golu(_double fDeltaTime);

	void				Update_Colliders();

private:
	void				CollisionTriger_OnTarget_Raji(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
						_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType);

	void				CollisionTriger_OnTarget_Golu(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType);

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_Collider();
	HRESULT				SetUp_Etc();


private:
	EONTARGET				m_eOnTarget = TARGET_END;
	_float					m_fDestroyTimer = 3.f;
	_bool					m_bOnPlayer = false;
	_bool					m_bOnceSwitch = false;
	_bool					m_bOnceSwitch2 = false;
	_float					m_fFallingAcc = 0.f;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CCollider*				m_pCollider = nullptr;

public:
	static CDynamicPlatform*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
