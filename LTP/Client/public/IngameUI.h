#pragma once
#include "GameObject.h"

BEGIN(Client)

class CIngameUI final : public CGameObject
{
private:
	CIngameUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CIngameUI(const CIngameUI& rhs);
	virtual ~CIngameUI() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	void	Set_WeaponUIAngle(_int iWeapon);
	void	TurnWeaponUIAngle(_double fDeltaTime);

private:
	CRenderer*		m_pRendererCom = nullptr;

	//UI
	class CUI*		m_pWeaponUI = nullptr;
	class CUI*		m_pWeaponMidleUI = nullptr;

	//Font
	FONTSDESC		m_Skillpercent;

private:
	_bool			m_bIsChangeWeapon = false;

	_int			m_iBeforeWeapon = 0;
	_int			m_iNextWeapon = 0;
	_float			m_fWeaponNowUIAngle = 0.f;
	_float			m_fTurningAngle = 0.f;
	_float			m_fDeltaTime = 0.f;

	_float			m_fSwordSkillCount = 0.f;
	_float			m_fBowCount = 0.f;
	_float			m_fSpearCount = 0.f;


private:
	HRESULT Ready_Layer_UI();
	HRESULT Render_Fonts();

private:
	HRESULT SetUp_Components();

public:
	static CIngameUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

  END