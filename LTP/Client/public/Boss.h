#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CBoss abstract : public CGameObject
{
protected:
	CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CBoss(const CBoss& rhs);
	virtual ~CBoss() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

protected:
	_bool		m_bIsHit = false;
	_bool		m_bIsAttack = false;
	_float		m_fAttackCoolTime = 3.f;
	_float		m_fSkillCoolTime = 10.f;



//protected:
//	CShader*			m_pShaderCom = nullptr;
//	CRenderer*			m_pRendererCom = nullptr;
//	CModel*				m_pModel = nullptr;
//	CTransform*			m_pTransformCom = nullptr;
//
//	_uint				m_iOldAnimIndex = INT_MAX;
//	_uint				m_iAdjMovedIndex = 0;

protected:
	//virtual HRESULT SetUp_Components();
	//virtual HRESULT	Adjust_AnimMovedTransform(_double fDeltatime);

public:
	virtual void Free() override;
};

END
