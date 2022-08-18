#pragma once

#include "GameObject.h"

BEGIN(Client)
class CMiniGame_Jino_Player;

class CBeachBall final : public CGameObject
{
public:
	enum EBeachBallState { STATE_SPAWN, STATE_RIDE, STATE_LOSE, STATE_LOSE_R, STATE_END };

private:
	explicit CBeachBall(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBeachBall(const CBeachBall& rhs);
	virtual ~CBeachBall() = default;

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

public:
	void				SetUp_PlayerTransform(CTransform* pPlayerTransform);
	void				Set_Active(_bool bActive);
	void				Set_Position(_float3 vPos);

public:
	EBeachBallState		Get_CurState();
	CTransform*			Get_TransformCom();

public:
	void				Set_State_Spawn();
	void				Set_State_Ride();
	void				Set_State_Lose();
	void				Set_State_Lose_R();

private:
	void				Update_Spawn(_double fDeltaTime);
	void				Update_Ride(_double fDeltaTime);
	void				Update_Lose(_double fDeltaTime);
	void				Update_Lose_R(_double fDeltaTime);

	void				Update_Colliders();

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_Colliders();
	HRESULT				SetUp_Etc();

private:
	EBeachBallState			m_eCurState = STATE_RIDE;
	_bool					m_bActive = true;
	_float					m_fDelayTime = 3.f;

private:
	CTransform*				m_pPlayerTransform = nullptr;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;

	CCollider*				m_pCollider = nullptr;
	CCollider*				m_pCollider_RideArea = nullptr;

public:
	static CBeachBall*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
