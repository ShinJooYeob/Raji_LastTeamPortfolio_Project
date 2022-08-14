#pragma once

#include "GameObject.h"

BEGIN(Client)
class CRepelWall;

class CMahaHead  final : public CGameObject
{
private:
	explicit CMahaHead(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMahaHead(const CMahaHead  & rhs);
	virtual ~CMahaHead() = default;

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
	CTransform*			Get_TransformCom() {
		return m_pTransformCom;
	}

	void				Set_CurState(_uint iState);

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_Collider();
	HRESULT				SetUp_Etc();

private:
	_uint					m_iCurState = 0;
	_float					m_fDelayTime = 3.f;
	_float					m_fDelayTime_2 = 3.f;
	_uint					m_iStateController_0 = 0;
	_float					m_fRotAngle = 0.f;
	_float					m_fRotSpeed = 0.01f;
	_bool					m_bPlayOnceSound = false;
	_float3					m_fTargetLookDir = _float3(0.f);


private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CDissolve*				m_pDissolveCom = nullptr;

public:
	static CMahaHead*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
