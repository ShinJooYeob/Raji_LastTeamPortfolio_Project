#pragma once

#include "GameObject.h"
#include "PlayerWeapon.h"
#include "TriggerObject.h"

BEGIN(Client)
class CCamera_Main;

class CRajibalsura final : public CGameObject
{
private:
	explicit CRajibalsura(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CRajibalsura(const CRajibalsura& rhs);
	virtual ~CRajibalsura() = default;

public:
	virtual HRESULT				Initialize_Prototype(void* pArg) override;
	virtual HRESULT				Initialize_Clone(void* pArg) override;


public:
	virtual _int				Update(_double fDeltaTime) override;
	virtual _int				LateUpdate(_double fDeltaTimer) override;
	virtual _int				Render() override;
	virtual _int				LateRender() override;


	virtual void				CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
								_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;
	
public: /* Damage Logic*/
	virtual _float				Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;
	_float						Apply_Damage(CGameObject* pTargetObject, _float fDamageAmount, _bool bKnockback);

public	:
	void						Set_CurState(_uint iState);

private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_EtcInfo();

private:
	_uint					m_iCurState = 0;
	_float					m_fAnimSpeed = 0.f;
	_float					m_fDelayTime = 0.f;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CDissolve*				m_pDissolveCom = nullptr;

public:
	static CRajibalsura*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free() override;
};

END
