#pragma once

#include "GameObject.h"

BEGIN(Client)

class CRepelWall final : public CGameObject
{
public:
	typedef struct tagRepelWallDesc
	{
		CGameObject*		pAttachTarget;
		_float3				fSpawnPos;
		_float				fRotation;
	}REPELWALLDESC;

private:
	explicit CRepelWall(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CRepelWall(const CRepelWall& rhs);
	virtual ~CRepelWall() = default;

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
	CTransform*			Get_TransformCom();
	void				Set_Appear(_bool bAppear, _float fTargetTime = 0.5f);

private:
	void				Update_Colliders();

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_Colliders();
	HRESULT				SetUp_Etc();

private:
	REPELWALLDESC			m_tRepelWallDesc;
	_bool					m_bDisappear = true;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CCollider*				m_pCollider = nullptr;
	CDissolve*				m_pDissolveCom = nullptr;

public:
	static CRepelWall*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
