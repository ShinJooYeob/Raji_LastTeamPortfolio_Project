#pragma once
#include "GameObject.h"

BEGIN(Client)

class CElevator final : public CGameObject
{
public:
	typedef struct tagElevatorDesc
	{
		_float			fMoveSpeed;
		_float3			fRotation;
		_float3			fStartPos;
		_float3			fScale;
		_float3			fDestPos;
		_float			fColliderOffset_Y;
		_float			fColliderScale = 0.25f;
		_uint			iMeshType = 0;
	}ELEVATORDESC;

private:
	explicit CElevator(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CElevator(const CElevator& rhs);
	virtual ~CElevator() = default;

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
	ELEVATORDESC			m_tElevatorDesc;
	_bool					m_bActive = false;
	_bool					m_bEndInteract = false;
	class CPlayer*			m_pPlayer = nullptr;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CCollider*				m_pCollider = nullptr;

public:
	static CElevator*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
