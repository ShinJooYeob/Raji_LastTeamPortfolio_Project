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
	_float		m_fAttackCoolTime = 0.f;
	_float		m_fSkillCoolTime = 0.f;
	_float		m_fAnimmultiple = 1.f; // AnimationPlaySpeed
	_float		m_fRange = 0.f; // 오브젝트간의 거리변수

public:
	virtual void Free() override;
};

END
