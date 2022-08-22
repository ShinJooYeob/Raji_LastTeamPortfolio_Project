#pragma once

#include "GameObject.h"

BEGIN(Client)

class CRajiMask final : public CGameObject
{
private:
	explicit CRajiMask(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CRajiMask(const CRajiMask& rhs);
	virtual ~CRajiMask() = default;

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
	void				Set_Appear(_bool bAppear);
	void				Set_AttachDesc(ATTACHEDESC tAttachDesc);

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_Etc();

private:
	_bool					m_bDisappear = false;
	ATTACHEDESC				m_tAttachDesc;
	_uint					m_iCurCutSceneState = 0;
	_bool					m_bAttached = false;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModel = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CDissolve*				m_pDissolveCom = nullptr;

public:
	static CRajiMask*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
