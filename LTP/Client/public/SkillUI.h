#pragma once
#include "GameObject.h"

BEGIN(Client)

class CSkillUI final : public CGameObject
{

//public:
//	enum WeaponType {WEAPON_BOW, WEAPON_SPEAR, WEAPON_SWORDSHIELD, WEAPON_END};
private:
	CSkillUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CSkillUI(const CSkillUI& rhs);
	virtual ~CSkillUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	_int		Get_SkillPoint() { return (*m_iSkillPoint); }
	WeaponType	Get_WeaponType() { return m_eWeaponType; }

public:
	void		Set_WeaponType(WeaponType Type) { m_eWeaponType = Type; }

public:
	void	DrawSkillPoint();
	void	ChangeSkillPointImage();
	void	DrawSkillName(_int iSkillNum);
	void	DrawSkillExplanation(_int iSkillNum);

	HRESULT Render_Fonts();

private:
	CRenderer*		m_pRendererCom = nullptr;

	_int*		m_iSkillPoint;

	WeaponType	m_eWeaponType = WEAPON_BOW;

	_int*		m_FireSkillPoint;
	_int*		m_IceSkillPoint;
	_int*		m_LightningSkillPoint;

	_bool		m_bIsSkillExplanation = false;

	FONTSDESC	m_SkillNameFont;
	FONTSDESC	m_SkillExplanationFont;
	

private:
	class CUI*	m_pBackGround = nullptr;
	class CUI*	m_PhoenixUI = nullptr;
	class CUI*	m_pSkillColorCircle = nullptr;
	class CUI*	m_pSkillFace = nullptr;
	class CUI*	m_pWeaponBowUI = nullptr;
	class CUI*	m_pWeaponSpearUI = nullptr;
	class CUI*	m_pSwordShield = nullptr;
	class CUI*	m_pSkillGrass_1 = nullptr;
	class CUI*	m_pSkillGrass_2 = nullptr;
	class CUI*	m_pSkillGrass_3 = nullptr;
	class CUI*	m_pSkill_1 = nullptr;
	class CUI*	m_pSkill_2 = nullptr;
	class CUI*	m_pSkill_3 = nullptr;
	//class CUI* m_MidleCircle = nullptr;

	//basicUI
	vector<class CUI*> m_vBasicUIs;
	vector<class CUI*> m_vSkill_1_Sprouts;
	vector<class CUI*> m_vSkill_2_Sprouts;
	vector<class CUI*> m_vSkill_3_Sprouts;
	vector<class CUI*> m_vSkillPoints;
	vector<class CUI*> m_vSkillPoint_Sprouts1;
	vector<class CUI*> m_vSkillPoint_Sprouts2;
	vector<class CUI*> m_vSkillPoint_Sprouts3;
	vector<class CUI*> m_vSkillExplanations;

private:
	HRESULT Ready_Layer_UI();

private:
	HRESULT SetUp_Components();


public:
	static CSkillUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END