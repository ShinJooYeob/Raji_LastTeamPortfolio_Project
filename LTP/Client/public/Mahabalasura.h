#pragma once
#include "Boss.h"

BEGIN(Client)
class CMahabalasura final : public CBoss
{
public:
	enum SkillAttackName {SKILL_SPEAR, SKILL_HAND, SKILL_SPEAR_RAIN, SKILL_COPY, SKILL_TELEPORT, SKILL_END};
private:
	CMahabalasura(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMahabalasura(const CMahabalasura& rhs);
	virtual ~CMahabalasura() = default;

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

	_bool				m_bIsHalf = false;
	_bool				m_bIsWalk = true;
	_bool				m_bIsLookAt = true;

	CGameObject*		m_pPlayerObj;

private:
	HRESULT SetUp_Components();
	HRESULT	Adjust_AnimMovedTransform(_double fDeltatime);

public:
	static CMahabalasura* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END