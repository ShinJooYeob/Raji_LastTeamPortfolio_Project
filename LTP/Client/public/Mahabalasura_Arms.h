#pragma once

#include "MonsterWeapon.h"

BEGIN(Client)
class CMahabalasura_Arms final : public CMonsterWeapon
{
private:
	CMahabalasura_Arms(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMahabalasura_Arms(const CMahabalasura_Arms& rhs);
	virtual ~CMahabalasura_Arms() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModel = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	_uint				m_iOldAnimIndex = INT_MAX;
	_uint				m_iAdjMovedIndex = 0;
	_uint				m_iAniNum = 0;

	ATTACHEDESC			m_eAttachedDesc;

	_float4x4			m_fAttachedMatrix;

private:
	void				Update_AttachMatrix();

private:
	HRESULT		SetUp_Components();
	HRESULT		Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CMahabalasura_Arms* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END