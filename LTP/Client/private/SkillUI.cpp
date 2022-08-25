#include "stdafx.h"
#include "..\public\SkillUI.h"
#include "UI.h"

CSkillUI::CSkillUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CSkillUI::CSkillUI(const CSkillUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkillUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CSkillUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	CUtilityMgr* pUyilityMgr = GetSingle(CUtilityMgr);

	m_iSkillPoint = pUyilityMgr->Get_SkillPoint();

	m_FireSkillPoint = pUyilityMgr->Get_FireSkillPointArry();
	m_IceSkillPoint = pUyilityMgr->Get_IceSkillPointArry();
	m_LightningSkillPoint = pUyilityMgr->Get_LightningSkillPointArry();

	m_eWeaponType = pUyilityMgr->Get_SkillWeaponType();


	SetUp_Components();

	Ready_Layer_UI();


	if (m_eWeaponType == WEAPON_BOW)
	{
		m_pWeaponBowUI->Set_TextureIndex(24);
		m_pWeaponSpearUI->Set_TextureIndex(100);
		m_pSwordShield->Set_TextureIndex(91);
		m_pSkillColorCircle->Set_TextureIndex(84);
		m_pSkillFace->Set_TextureIndex(85);

		if (m_IceSkillPoint[0] <= 0)
		{
			m_pSkill_1->Set_TextureIndex(54);
			m_pSkillGrass_1->Set_TextureIndex(88);
			m_pSkill_1->Set_PassIndex(1);
		}
		else
		{
			m_pSkill_1->Set_TextureIndex(53);
			m_pSkillGrass_1->Set_TextureIndex(87);
			m_pSkill_1->Set_PassIndex(19);
		}

		if (m_IceSkillPoint[1] <= 0)
		{
			m_pSkill_2->Set_TextureIndex(56);
			m_pSkillGrass_2->Set_TextureIndex(134);
			m_pSkill_2->Set_PassIndex(1);
		}
		else
		{
			m_pSkill_2->Set_TextureIndex(52);
			m_pSkillGrass_2->Set_TextureIndex(129);
			m_pSkill_2->Set_PassIndex(19);
		}

		if (m_IceSkillPoint[2] <= 0)
		{
			m_pSkill_3->Set_TextureIndex(55);
			m_pSkillGrass_3->Set_TextureIndex(134);
			m_pSkill_3->Set_PassIndex(1);
		}
		else
		{
			m_pSkill_3->Set_TextureIndex(51);
			m_pSkillGrass_3->Set_TextureIndex(129);
			m_pSkill_3->Set_PassIndex(19);
		}
	}
	else if (m_eWeaponType == WEAPON_SPEAR)
	{
		m_pWeaponBowUI->Set_TextureIndex(25);
		m_pWeaponSpearUI->Set_TextureIndex(99);
		m_pSwordShield->Set_TextureIndex(91);
		m_pSkillColorCircle->Set_TextureIndex(101);
		m_pSkillFace->Set_TextureIndex(102);

		if (m_FireSkillPoint[0] <= 0)
		{
			m_pSkill_1->Set_TextureIndex(43);
			m_pSkillGrass_1->Set_TextureIndex(88);
			m_pSkill_1->Set_PassIndex(1);

		}
		else
		{
			m_pSkill_1->Set_TextureIndex(40);
			m_pSkillGrass_1->Set_TextureIndex(87);
			m_pSkill_1->Set_PassIndex(19);

		}

		if (m_FireSkillPoint[1] <= 0)
		{
			m_pSkill_2->Set_TextureIndex(45);
			m_pSkillGrass_2->Set_TextureIndex(134);
			m_pSkill_2->Set_PassIndex(1);
		}
		else
		{
			m_pSkill_2->Set_TextureIndex(42);
			m_pSkillGrass_2->Set_TextureIndex(129);
			m_pSkill_2->Set_PassIndex(19);
		}

		if (m_FireSkillPoint[2] <= 0)
		{
			m_pSkill_3->Set_TextureIndex(44);
			m_pSkillGrass_3->Set_TextureIndex(134);
			m_pSkill_3->Set_PassIndex(1);
		}
		else
		{
			m_pSkill_3->Set_TextureIndex(41);
			m_pSkillGrass_3->Set_TextureIndex(129);
			m_pSkill_3->Set_PassIndex(19);
		}
	}
	else if (m_eWeaponType == WEAPON_SWORDSHIELD)
	{
		m_pWeaponBowUI->Set_TextureIndex(25);
		m_pWeaponSpearUI->Set_TextureIndex(100);
		m_pSwordShield->Set_TextureIndex(90);
		m_pSkillColorCircle->Set_TextureIndex(37);
		m_pSkillFace->Set_TextureIndex(38);

		if (m_LightningSkillPoint[0] <= 0)
		{
			m_pSkill_1->Set_TextureIndex(95);
			m_pSkillGrass_1->Set_TextureIndex(88);
			m_pSkill_1->Set_PassIndex(1);
		}
		else
		{
			m_pSkill_1->Set_TextureIndex(92);
			m_pSkillGrass_1->Set_TextureIndex(87);
			m_pSkill_1->Set_PassIndex(19);
		}

		if (m_LightningSkillPoint[1] <= 0)
		{
			m_pSkill_2->Set_TextureIndex(97);
			m_pSkillGrass_2->Set_TextureIndex(134);
			m_pSkill_2->Set_PassIndex(1);
		}
		else
		{
			m_pSkill_2->Set_TextureIndex(94);
			m_pSkillGrass_2->Set_TextureIndex(129);
			m_pSkill_2->Set_PassIndex(19);
		}

		if (m_LightningSkillPoint[2] <= 0)
		{
			m_pSkill_3->Set_TextureIndex(97);
			m_pSkillGrass_3->Set_TextureIndex(134);
			m_pSkill_3->Set_PassIndex(1);
		}
		else
		{
			m_pSkill_3->Set_TextureIndex(93);
			m_pSkillGrass_3->Set_TextureIndex(129);
			m_pSkill_3->Set_PassIndex(19);
		}
	}

	ChangeSkillPointImage();

	m_SkillNameFont.fAngle = 0.f;
	m_SkillNameFont.szString = L"";
	m_SkillNameFont.vPosition = _float2(970.f, 145.f);
	m_SkillNameFont.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_SkillNameFont.vFontScale = _float2(0.8f);

	m_SkillExplanationFont.fAngle = 0.f;
	m_SkillExplanationFont.szString = L"";
	m_SkillExplanationFont.vPosition = _float2(965.f, 200.f);
	m_SkillExplanationFont.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_SkillExplanationFont.vFontScale = _float2(0.5f);


	return S_OK;
}

_int CSkillUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUtilityMgr* pUyilityMgr = GetSingle(CUtilityMgr);

	DrawSkillPoint();

	m_bIsSkillExplanation = false;
	m_SkillNameFont.szString = L"";
	m_SkillExplanationFont.szString = L"";
	m_SkillExplanationFont.vPosition = _float2(965.f, 230.f);
	//m_SkillNameFont.vPosition = _float2(965.f, 145.f);

	_float2				MousePos;
	POINT				ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	MousePos.x = (_float)ptMouse.x;
	MousePos.y = (_float)ptMouse.y;

	_float UIPosX;
	_float UIPosY;
	_float2 UISize;

	//m_pSkillGrass_1
	UIPosX = m_pSkillGrass_1->Get_PosX();
	UIPosY = m_pSkillGrass_1->Get_PosY();
	UISize = m_pSkillGrass_1->Get_Size();

	if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
		MousePos.x < UIPosX + (UISize.x*0.5f) &&
		MousePos.y > UIPosY - (UISize.y*0.5f) &&
		MousePos.y < UIPosY + (UISize.y*0.5f))
	{
		m_bIsSkillExplanation = true;

		DrawSkillName(0);
		DrawSkillExplanation(0);
	}
	//m_pSkillGrass_2
	UIPosX = m_pSkillGrass_2->Get_PosX();
	UIPosY = m_pSkillGrass_2->Get_PosY();
	UISize = m_pSkillGrass_2->Get_Size();

	if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
		MousePos.x < UIPosX + (UISize.x*0.5f) &&
		MousePos.y > UIPosY - (UISize.y*0.5f) &&
		MousePos.y < UIPosY + (UISize.y*0.5f))
	{
		m_bIsSkillExplanation = true;

		DrawSkillName(1);
		DrawSkillExplanation(1);
	}
	//m_pSkillGrass_3
	UIPosX = m_pSkillGrass_3->Get_PosX();
	UIPosY = m_pSkillGrass_3->Get_PosY();
	UISize = m_pSkillGrass_3->Get_Size();

	if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
		MousePos.x < UIPosX + (UISize.x*0.5f) &&
		MousePos.y > UIPosY - (UISize.y*0.5f) &&
		MousePos.y < UIPosY + (UISize.y*0.5f))
	{
		m_bIsSkillExplanation = true;

		DrawSkillName(2);
		DrawSkillExplanation(2);
	}


	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
	{
		//BowImage Click
		UIPosX = m_pWeaponBowUI->Get_PosX();
		UIPosY = m_pWeaponBowUI->Get_PosY();
		UISize = m_pWeaponBowUI->Get_Size();
		if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
			MousePos.x < UIPosX + (UISize.x*0.5f) &&
			MousePos.y > UIPosY - (UISize.y*0.5f) &&
			MousePos.y < UIPosY + (UISize.y*0.5f))
		{
			m_eWeaponType = WEAPON_BOW;
			pUyilityMgr->Set_SkillWeaponType(m_eWeaponType);
			m_pWeaponBowUI->Set_TextureIndex(24);
			m_pWeaponSpearUI->Set_TextureIndex(100);
			m_pSwordShield->Set_TextureIndex(91);
			m_pSkillColorCircle->Set_TextureIndex(84);
			m_pSkillFace->Set_TextureIndex(85);
			
			if (m_IceSkillPoint[0] <= 0)
			{
				m_pSkill_1->Set_TextureIndex(54);
				m_pSkillGrass_1->Set_TextureIndex(88);
				m_pSkill_1->Set_PassIndex(1);
			}
			else
			{
				m_pSkill_1->Set_TextureIndex(53);
				m_pSkillGrass_1->Set_TextureIndex(87);
				m_pSkill_1->Set_PassIndex(19);
			}

			if (m_IceSkillPoint[1] <= 0)
			{
				m_pSkill_2->Set_TextureIndex(56);
				m_pSkillGrass_2->Set_TextureIndex(134);
				m_pSkill_2->Set_PassIndex(1);
			}
			else
			{
				m_pSkill_2->Set_TextureIndex(52);
				m_pSkillGrass_2->Set_TextureIndex(129);
				m_pSkill_2->Set_PassIndex(19);
			}

			if (m_IceSkillPoint[2] <= 0)
			{
				m_pSkill_3->Set_TextureIndex(55);
				m_pSkillGrass_3->Set_TextureIndex(134);
				m_pSkill_3->Set_PassIndex(1);
			}
			else
			{
				m_pSkill_3->Set_TextureIndex(51);
				m_pSkillGrass_3->Set_TextureIndex(129);
				m_pSkill_3->Set_PassIndex(19);
			}
			
		}
		//SpearImage Click
		UIPosX = m_pWeaponSpearUI->Get_PosX();
		UIPosY = m_pWeaponSpearUI->Get_PosY();
		UISize = m_pWeaponSpearUI->Get_Size();
		if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
			MousePos.x < UIPosX + (UISize.x*0.5f) &&
			MousePos.y > UIPosY - (UISize.y*0.5f) &&
			MousePos.y < UIPosY + (UISize.y*0.5f))
		{
			m_eWeaponType = WEAPON_SPEAR;
			pUyilityMgr->Set_SkillWeaponType(m_eWeaponType);
			m_pWeaponBowUI->Set_TextureIndex(25);
			m_pWeaponSpearUI->Set_TextureIndex(99);
			m_pSwordShield->Set_TextureIndex(91);
			m_pSkillColorCircle->Set_TextureIndex(101);
			m_pSkillFace->Set_TextureIndex(102);

			if (m_FireSkillPoint[0] <= 0)
			{
				m_pSkill_1->Set_TextureIndex(43);
				m_pSkillGrass_1->Set_TextureIndex(88);
				m_pSkill_1->Set_PassIndex(1);

			}
			else
			{
				m_pSkill_1->Set_TextureIndex(40);
				m_pSkillGrass_1->Set_TextureIndex(87);
				m_pSkill_1->Set_PassIndex(19);

			}

			if (m_FireSkillPoint[1] <= 0)
			{
				m_pSkill_2->Set_TextureIndex(45);
				m_pSkillGrass_2->Set_TextureIndex(134);
				m_pSkill_2->Set_PassIndex(1);
			}
			else
			{
				m_pSkill_2->Set_TextureIndex(42);
				m_pSkillGrass_2->Set_TextureIndex(129);
				m_pSkill_2->Set_PassIndex(19);
			}

			if (m_FireSkillPoint[2] <= 0)
			{
				m_pSkill_3->Set_TextureIndex(44);
				m_pSkillGrass_3->Set_TextureIndex(134);
				m_pSkill_3->Set_PassIndex(1);
			}
			else
			{
				m_pSkill_3->Set_TextureIndex(41);
				m_pSkillGrass_3->Set_TextureIndex(129);
				m_pSkill_3->Set_PassIndex(19);
			}

		}
		//mSwordShield Click
		UIPosX = m_pSwordShield->Get_PosX();
		UIPosY = m_pSwordShield->Get_PosY();
		UISize = m_pSwordShield->Get_Size();
		if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
			MousePos.x < UIPosX + (UISize.x*0.5f) &&
			MousePos.y > UIPosY - (UISize.y*0.5f) &&
			MousePos.y < UIPosY + (UISize.y*0.5f))
		{
			m_eWeaponType = WEAPON_SWORDSHIELD;
			pUyilityMgr->Set_SkillWeaponType(m_eWeaponType);

			m_pWeaponBowUI->Set_TextureIndex(25);
			m_pWeaponSpearUI->Set_TextureIndex(100);
			m_pSwordShield->Set_TextureIndex(90);
			m_pSkillColorCircle->Set_TextureIndex(37);
			m_pSkillFace->Set_TextureIndex(38);

			if (m_LightningSkillPoint[0] <= 0)
			{
				m_pSkill_1->Set_TextureIndex(95);
				m_pSkillGrass_1->Set_TextureIndex(88);
				m_pSkill_1->Set_PassIndex(1);
			}
			else
			{
				m_pSkill_1->Set_TextureIndex(92);
				m_pSkillGrass_1->Set_TextureIndex(87);
				m_pSkill_1->Set_PassIndex(19);
			}

			if (m_LightningSkillPoint[1] <= 0)
			{
				m_pSkill_2->Set_TextureIndex(97);
				m_pSkillGrass_2->Set_TextureIndex(134);
				m_pSkill_2->Set_PassIndex(1);
			}
			else
			{
				m_pSkill_2->Set_TextureIndex(94);
				m_pSkillGrass_2->Set_TextureIndex(129);
				m_pSkill_2->Set_PassIndex(19);
			}

			if (m_LightningSkillPoint[2] <= 0)
			{
				m_pSkill_3->Set_TextureIndex(97);
				m_pSkillGrass_3->Set_TextureIndex(134);
				m_pSkill_3->Set_PassIndex(1);
			}
			else
			{
				m_pSkill_3->Set_TextureIndex(93);
				m_pSkillGrass_3->Set_TextureIndex(129);
				m_pSkill_3->Set_PassIndex(19);
			}
		}

		//m_pSkillGrass_1 Click
		UIPosX = m_pSkillGrass_1->Get_PosX();
		UIPosY = m_pSkillGrass_1->Get_PosY();
		UISize = m_pSkillGrass_1->Get_Size();
		if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
			MousePos.x < UIPosX + (UISize.x*0.5f) &&
			MousePos.y > UIPosY - (UISize.y*0.5f) &&
			MousePos.y < UIPosY + (UISize.y*0.5f))
		{
			if (m_eWeaponType == WEAPON_BOW)
			{
				if (m_IceSkillPoint[0] < 3 && (*m_iSkillPoint) != 0)
				{
					++m_IceSkillPoint[0];

					--(*m_iSkillPoint);
					if (*m_iSkillPoint <= -1)
						*m_iSkillPoint = -1;

					m_pSkill_1->Set_TextureIndex(53);
					m_pSkillGrass_1->Set_TextureIndex(87);

					m_pSkill_1->Set_PassIndex(19);
				}
			}
			else if (m_eWeaponType == WEAPON_SPEAR)
			{
				if (m_FireSkillPoint[0] < 3 && (*m_iSkillPoint) != 0)
				{
					++m_FireSkillPoint[0];

					--(*m_iSkillPoint);
					if ((*m_iSkillPoint) <= -1)
						(*m_iSkillPoint) = -1;

					m_pSkill_1->Set_TextureIndex(40);
					m_pSkillGrass_1->Set_TextureIndex(87);
					m_pSkill_1->Set_PassIndex(19);
				}
			}
			else if (m_eWeaponType == WEAPON_SWORDSHIELD)
			{
				if (m_LightningSkillPoint[0] < 3 && (*m_iSkillPoint) != 0)
				{
					++m_LightningSkillPoint[0];

					--(*m_iSkillPoint);
					if ((*m_iSkillPoint) <= -1)
						(*m_iSkillPoint) = -1;

					m_pSkill_1->Set_TextureIndex(92);
					m_pSkillGrass_1->Set_TextureIndex(87);
					m_pSkill_1->Set_PassIndex(19);
				}
			}
		}

		//m_pSkillGrass_2 Click
		UIPosX = m_pSkillGrass_2->Get_PosX();
		UIPosY = m_pSkillGrass_2->Get_PosY();
		UISize = m_pSkillGrass_2->Get_Size();
		if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
			MousePos.x < UIPosX + (UISize.x*0.5f) &&
			MousePos.y > UIPosY - (UISize.y*0.5f) &&
			MousePos.y < UIPosY + (UISize.y*0.5f))
		{
			if (m_eWeaponType == WEAPON_BOW)
			{
				if (m_IceSkillPoint[1] < 3 && (*m_iSkillPoint) != 0)
				{
					++m_IceSkillPoint[1];

					--(*m_iSkillPoint);
					if ((*m_iSkillPoint) <= -1)
						(*m_iSkillPoint) = -1;

					m_pSkill_2->Set_TextureIndex(52);
					m_pSkillGrass_2->Set_TextureIndex(129);
					m_pSkill_2->Set_PassIndex(19);
				}
			}
			else if (m_eWeaponType == WEAPON_SPEAR)
			{
				if (m_FireSkillPoint[1] < 3 && (*m_iSkillPoint) != 0)
				{
					++m_FireSkillPoint[1];

					--(*m_iSkillPoint);
					if ((*m_iSkillPoint) <= -1)
						(*m_iSkillPoint) = -1;

					m_pSkill_2->Set_TextureIndex(42);
					m_pSkill_2->Set_PassIndex(19);
					m_pSkillGrass_2->Set_TextureIndex(129);
				}
			}
			else if (m_eWeaponType == WEAPON_SWORDSHIELD)
			{
				if (m_LightningSkillPoint[1] < 3 && (*m_iSkillPoint) != 0)
				{
					++m_LightningSkillPoint[1];

					--(*m_iSkillPoint);
					if ((*m_iSkillPoint) <= -1)
						(*m_iSkillPoint) = -1;

					m_pSkill_2->Set_TextureIndex(94);
					m_pSkill_2->Set_PassIndex(19);
					m_pSkillGrass_2->Set_TextureIndex(129);
				}
			}
		}

		//m_pSkillGrass_3 Click
		UIPosX = m_pSkillGrass_3->Get_PosX();
		UIPosY = m_pSkillGrass_3->Get_PosY();
		UISize = m_pSkillGrass_3->Get_Size();
		if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
			MousePos.x < UIPosX + (UISize.x*0.5f) &&
			MousePos.y > UIPosY - (UISize.y*0.5f) &&
			MousePos.y < UIPosY + (UISize.y*0.5f))
		{
			if (m_eWeaponType == WEAPON_BOW)
			{
				if (m_IceSkillPoint[2] < 3 && (*m_iSkillPoint) != 0)
				{
					++m_IceSkillPoint[2];

					--(*m_iSkillPoint);
					if ((*m_iSkillPoint) <= -1)
						(*m_iSkillPoint) = -1;

					m_pSkill_3->Set_TextureIndex(51);
					m_pSkill_3->Set_PassIndex(19);
					m_pSkillGrass_3->Set_TextureIndex(129);
				}
			}
			else if (m_eWeaponType == WEAPON_SPEAR)
			{
				if (m_FireSkillPoint[2] < 3 && (*m_iSkillPoint) != 0)
				{
					++m_FireSkillPoint[2];

					--(*m_iSkillPoint);
					if ((*m_iSkillPoint) <= -1)
						(*m_iSkillPoint) = -1;

					m_pSkill_3->Set_TextureIndex(41);
					m_pSkill_3->Set_PassIndex(19);
					m_pSkillGrass_3->Set_TextureIndex(129);
				}
			}
			else if (m_eWeaponType == WEAPON_SWORDSHIELD)
			{
				if (m_LightningSkillPoint[2] < 3 && (*m_iSkillPoint) != 0)
				{
					++m_LightningSkillPoint[2];

					--(*m_iSkillPoint);
					if ((*m_iSkillPoint) <= -1)
						(*m_iSkillPoint) = -1;

					m_pSkill_3->Set_TextureIndex(93);
					m_pSkill_3->Set_PassIndex(19);
					m_pSkillGrass_3->Set_TextureIndex(129);
				}
			}
		}




		ChangeSkillPointImage();
		DrawSkillPoint();
	}
	else if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_Down)
	{
		//m_pSkillGrass_1 Click
		_float UIPosX = m_pSkillGrass_1->Get_PosX();
		_float UIPosY = m_pSkillGrass_1->Get_PosY();
		_float2 UISize = m_pSkillGrass_1->Get_Size();
		if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
			MousePos.x < UIPosX + (UISize.x*0.5f) &&
			MousePos.y > UIPosY - (UISize.y*0.5f) &&
			MousePos.y < UIPosY + (UISize.y*0.5f))
		{
			if (m_eWeaponType == WEAPON_BOW)
			{
				if (m_IceSkillPoint[0] > 0 && (*m_iSkillPoint) < 18)
				{
					--m_IceSkillPoint[0];

					++(*m_iSkillPoint);
					if ((*m_iSkillPoint) > 18)
						(*m_iSkillPoint) = 18;

					if (m_IceSkillPoint[0] <= 0)
					{
						m_pSkill_1->Set_TextureIndex(54);
						m_pSkillGrass_1->Set_TextureIndex(88);
						m_pSkill_1->Set_PassIndex(1);
					}
				}
			}
			else if (m_eWeaponType == WEAPON_SPEAR)
			{
				if (m_FireSkillPoint[0] > 0 && (*m_iSkillPoint) < 18)
				{
					--m_FireSkillPoint[0];

					++(*m_iSkillPoint);
					if ((*m_iSkillPoint) > 18)
						(*m_iSkillPoint) = 18;

					if (m_FireSkillPoint[0] <= 0)
					{
						m_pSkill_1->Set_TextureIndex(43);
						m_pSkillGrass_1->Set_TextureIndex(88);
						m_pSkill_1->Set_PassIndex(1);
					}
				}
			}
			else if (m_eWeaponType == WEAPON_SWORDSHIELD)
			{
				if (m_LightningSkillPoint[0] > 0 && (*m_iSkillPoint) < 18)
				{
					--m_LightningSkillPoint[0];

					++(*m_iSkillPoint);
					if ((*m_iSkillPoint) > 18)
						(*m_iSkillPoint) = 18;

					if (m_LightningSkillPoint[0] <= 0)
					{
						m_pSkill_1->Set_TextureIndex(95);
						m_pSkillGrass_1->Set_TextureIndex(88);
						m_pSkill_1->Set_PassIndex(1);

					}
				}
			}
		}

		//m_pSkillGrass_2 Click
		UIPosX = m_pSkillGrass_2->Get_PosX();
		UIPosY = m_pSkillGrass_2->Get_PosY();
		UISize = m_pSkillGrass_2->Get_Size();
		if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
			MousePos.x < UIPosX + (UISize.x*0.5f) &&
			MousePos.y > UIPosY - (UISize.y*0.5f) &&
			MousePos.y < UIPosY + (UISize.y*0.5f))
		{
			if (m_eWeaponType == WEAPON_BOW)
			{
				if (m_IceSkillPoint[1] > 0 && (*m_iSkillPoint) < 18)
				{
					--m_IceSkillPoint[1];

					++(*m_iSkillPoint);
					if ((*m_iSkillPoint) > 18)
						(*m_iSkillPoint) = 18;

					if (m_IceSkillPoint[1] <= 0)
					{
						m_pSkill_2->Set_TextureIndex(56);
						m_pSkillGrass_2->Set_TextureIndex(134);
						m_pSkill_2->Set_PassIndex(1);

					}
				}
			}
			else if (m_eWeaponType == WEAPON_SPEAR)
			{
				if (m_FireSkillPoint[1] > 0  && (*m_iSkillPoint) < 18)
				{
					--m_FireSkillPoint[1];

					++(*m_iSkillPoint);
					if ((*m_iSkillPoint) > 18)
						(*m_iSkillPoint) = 18;

					if (m_FireSkillPoint[1] <= 0)
					{
						m_pSkill_2->Set_TextureIndex(45);
						m_pSkill_2->Set_PassIndex(1);
						m_pSkillGrass_2->Set_TextureIndex(134);
					}
				}
			}
			else if (m_eWeaponType == WEAPON_SWORDSHIELD)
			{
				if (m_LightningSkillPoint[1] > 0  && (*m_iSkillPoint)  < 18)
				{
					--m_LightningSkillPoint[1];

					++(*m_iSkillPoint);
					if ((*m_iSkillPoint) > 18)
						(*m_iSkillPoint) = 18;

					if (m_LightningSkillPoint[1] <= 0)
					{
						m_pSkill_2->Set_TextureIndex(97);
						m_pSkill_2->Set_PassIndex(1);
						m_pSkillGrass_2->Set_TextureIndex(134);
					}
				}
			}
		}

		//m_pSkillGrass_3 Click
		UIPosX = m_pSkillGrass_3->Get_PosX();
		UIPosY = m_pSkillGrass_3->Get_PosY();
		UISize = m_pSkillGrass_3->Get_Size();
		if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
			MousePos.x < UIPosX + (UISize.x*0.5f) &&
			MousePos.y > UIPosY - (UISize.y*0.5f) &&
			MousePos.y < UIPosY + (UISize.y*0.5f))
		{
			if (m_eWeaponType == WEAPON_BOW)
			{
				if (m_IceSkillPoint[2] > 0 && (*m_iSkillPoint) < 18)
				{
					--m_IceSkillPoint[2];

					++(*m_iSkillPoint);
					if ((*m_iSkillPoint) > 18)
						(*m_iSkillPoint) = 18;

					if (m_IceSkillPoint[2] <= 0)
					{
						m_pSkill_3->Set_TextureIndex(55);
						m_pSkill_3->Set_PassIndex(1);
						m_pSkillGrass_3->Set_TextureIndex(134);
					}
				}
			}
			else if (m_eWeaponType == WEAPON_SPEAR)
			{
				if (m_FireSkillPoint[2] > 0 && (*m_iSkillPoint) < 18)
				{
					--m_FireSkillPoint[2];

					++(*m_iSkillPoint);
					if ((*m_iSkillPoint) > 18)
						(*m_iSkillPoint) = 18;

					if (m_FireSkillPoint[2] <= 0)
					{
						m_pSkill_3->Set_TextureIndex(44);
						m_pSkill_3->Set_PassIndex(1);
						m_pSkillGrass_3->Set_TextureIndex(134);
					}
				}
			}
			else if (m_eWeaponType == WEAPON_SWORDSHIELD)
			{
				if (m_LightningSkillPoint[2] > 0 && (*m_iSkillPoint) < 18)
				{
					--m_LightningSkillPoint[2];

					++(*m_iSkillPoint);
					if ((*m_iSkillPoint) > 18)
						(*m_iSkillPoint) = 18;

					if (m_LightningSkillPoint[2] <= 0)
					{
						m_pSkill_3->Set_TextureIndex(97);
						m_pSkill_3->Set_PassIndex(1);
						m_pSkillGrass_3->Set_TextureIndex(134);
					}
				}
			}
		}

		ChangeSkillPointImage();
		DrawSkillPoint();
	}
	RELEASE_INSTANCE(CGameInstance);

	//UI
	m_pBackGround->Update(fDeltaTime);
	m_PhoenixUI->Update(fDeltaTime);
	m_pSkillFace->Update(fDeltaTime);
	m_pSkillColorCircle->Update(fDeltaTime);
	m_pWeaponBowUI->Update(fDeltaTime);
	m_pWeaponSpearUI->Update(fDeltaTime);
	m_pSwordShield->Update(fDeltaTime);
	m_pSkillGrass_1->Update(fDeltaTime);
	m_pSkillGrass_2->Update(fDeltaTime);
	m_pSkillGrass_3->Update(fDeltaTime);
	m_pSkill_1->Update(fDeltaTime);
	m_pSkill_2->Update(fDeltaTime);
	m_pSkill_3->Update(fDeltaTime);
	//m_MidleCircle->Update(fDeltaTime);

	for (auto& UI : m_vBasicUIs)
		UI->Update(fDeltaTime);

	for (auto& Sprout : m_vSkill_1_Sprouts)
		Sprout->Update(fDeltaTime);

	for (auto& Sprout2 : m_vSkill_2_Sprouts)
		Sprout2->Update(fDeltaTime);

	for (auto& Sprout3 : m_vSkill_3_Sprouts)
		Sprout3->Update(fDeltaTime);

	for (auto& SkillPoint : m_vSkillPoints)
		SkillPoint->Update(fDeltaTime);

	for (auto& SproutSkillPoint : m_vSkillPoint_Sprouts1)
		SproutSkillPoint->Update(fDeltaTime);

	for (auto& SproutSkillPoint : m_vSkillPoint_Sprouts2)
		SproutSkillPoint->Update(fDeltaTime);

	for (auto& SproutSkillPoint : m_vSkillPoint_Sprouts3)
		SproutSkillPoint->Update(fDeltaTime);

	if (m_bIsSkillExplanation)
	{
		for (auto& SkillExplanation : m_vSkillExplanations)
			SkillExplanation->Update(fDeltaTime);
	}

	for (_uint i = UEI_Sword + 1; i < UEI_END; i++)
		m_vecUIEffect[i]->Update(fDeltaTime);
	m_vecUIEffect[m_eWeaponType]->Update(fDeltaTime);
	
	return _int();
}

_int CSkillUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	//UI
	m_pBackGround->LateUpdate(fDeltaTime);
	m_PhoenixUI->LateUpdate(fDeltaTime);
	m_pSkillFace->LateUpdate(fDeltaTime);
	m_pSkillColorCircle->LateUpdate(fDeltaTime);
	m_pWeaponBowUI->LateUpdate(fDeltaTime);
	m_pWeaponSpearUI->LateUpdate(fDeltaTime);
	m_pSwordShield->LateUpdate(fDeltaTime);
	m_pSkillGrass_1->LateUpdate(fDeltaTime);
	m_pSkillGrass_2->LateUpdate(fDeltaTime);
	m_pSkillGrass_3->LateUpdate(fDeltaTime);
	m_pSkill_1->LateUpdate(fDeltaTime);
	m_pSkill_2->LateUpdate(fDeltaTime);
	m_pSkill_3->LateUpdate(fDeltaTime);
	//m_MidleCircle->LateUpdate(fDeltaTime);

	for (auto& UI : m_vBasicUIs)
		UI->LateUpdate(fDeltaTime);

	for (auto& Sprout : m_vSkill_1_Sprouts)
		Sprout->LateUpdate(fDeltaTime);

	for (auto& Sprout2 : m_vSkill_2_Sprouts)
		Sprout2->LateUpdate(fDeltaTime);

	for (auto& Sprout3 : m_vSkill_3_Sprouts)
		Sprout3->LateUpdate(fDeltaTime);

	for (auto& SkillPoint : m_vSkillPoints)
		SkillPoint->LateUpdate(fDeltaTime);

	for (auto& SproutSkillPoint : m_vSkillPoint_Sprouts1)
		SproutSkillPoint->LateUpdate(fDeltaTime);

	for (auto& SproutSkillPoint : m_vSkillPoint_Sprouts2)
		SproutSkillPoint->LateUpdate(fDeltaTime);

	for (auto& SproutSkillPoint : m_vSkillPoint_Sprouts3)
		SproutSkillPoint->LateUpdate(fDeltaTime);

	if (m_bIsSkillExplanation)
	{
		for (auto& SkillExplanation : m_vSkillExplanations)
			SkillExplanation->LateUpdate(fDeltaTime);
	}

	for (_uint i = UEI_Sword + 1; i < UEI_END; i++)
		m_vecUIEffect[i]->LateUpdate(fDeltaTime);
	m_vecUIEffect[m_eWeaponType]->LateUpdate(fDeltaTime);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	
	return _int();
}

_int CSkillUI::Render()
{
	if (__super::Render() < 0)		return -1;

	if(m_bIsSkillExplanation)
		Render_Fonts();

	return _int();
}

_int CSkillUI::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CSkillUI::DrawSkillPoint()
{
	for (_int i = 0; i < m_vSkillPoints.size(); ++i)
	{
		if (i <= (*m_iSkillPoint) - 1)
		{
			m_vSkillPoints[i]->Set_IsDraw(true);
			m_vSkillPoints[i]->Set_PassIndex(20);
		}
		else
		{
			m_vSkillPoints[i]->Set_IsDraw(false);
			m_vSkillPoints[i]->Set_PassIndex(1);

		}
	}
}

void CSkillUI::ChangeSkillPointImage()
{
	_int* TempArryInt = nullptr;

	if (m_eWeaponType == WEAPON_BOW)
		TempArryInt = m_IceSkillPoint;
	else if (m_eWeaponType == WEAPON_SPEAR)
		TempArryInt = m_FireSkillPoint;
	else if (m_eWeaponType == WEAPON_SWORDSHIELD)
		TempArryInt = m_LightningSkillPoint;

	m_vSkill_1_Sprouts[0]->Set_TextureIndex(71);
	m_vSkill_1_Sprouts[1]->Set_TextureIndex(73);
	m_vSkill_1_Sprouts[2]->Set_TextureIndex(75);
	for (_int i = 0; i < m_vSkillPoint_Sprouts1.size(); ++i)
	{
		m_vSkillPoint_Sprouts1[i]->Set_IsDraw(false);
		m_vSkill_1_Sprouts[i]->Set_PassIndex(1);
		m_vSkillPoint_Sprouts1[i]->Set_PassIndex(1);
	}

	for (_int i = 0; i < TempArryInt[0]; ++i)
	{
		if (i == 0)
		{
			m_vSkill_1_Sprouts[i]->Set_TextureIndex(77);
			m_vSkillPoint_Sprouts1[i]->Set_IsDraw(true);

			m_vSkill_1_Sprouts[i]->Set_PassIndex(19);
			m_vSkillPoint_Sprouts1[i]->Set_PassIndex(19);
		}
		else if (i == 1)
		{
			m_vSkill_1_Sprouts[i]->Set_TextureIndex(78);
			m_vSkillPoint_Sprouts1[i]->Set_IsDraw(true);

			m_vSkill_1_Sprouts[i]->Set_PassIndex(19);
			m_vSkillPoint_Sprouts1[i]->Set_PassIndex(19);
		}
		else if (i == 2)
		{
			m_vSkill_1_Sprouts[i]->Set_TextureIndex(79);
			m_vSkillPoint_Sprouts1[i]->Set_IsDraw(true);

			m_vSkill_1_Sprouts[i]->Set_PassIndex(19);
			m_vSkillPoint_Sprouts1[i]->Set_PassIndex(19);
		}
	}

	m_vSkill_2_Sprouts[0]->Set_TextureIndex(71);
	m_vSkill_2_Sprouts[1]->Set_TextureIndex(73);
	m_vSkill_2_Sprouts[2]->Set_TextureIndex(75);
	for (_int i = 0; i < m_vSkillPoint_Sprouts2.size(); ++i)
	{
		m_vSkillPoint_Sprouts2[i]->Set_IsDraw(false);
		m_vSkillPoint_Sprouts2[i]->Set_PassIndex(1);
		m_vSkill_2_Sprouts[i]->Set_PassIndex(1);
	}

	for (_int i = 0; i < TempArryInt[1]; ++i)
	{
		if (i == 0)
		{
			m_vSkill_2_Sprouts[i]->Set_TextureIndex(77);
			m_vSkillPoint_Sprouts2[i]->Set_IsDraw(true);

			m_vSkill_2_Sprouts[i]->Set_PassIndex(19);
			m_vSkillPoint_Sprouts2[i]->Set_PassIndex(19);
		}
		else if (i == 1)
		{
			m_vSkill_2_Sprouts[i]->Set_TextureIndex(78);
			m_vSkillPoint_Sprouts2[i]->Set_IsDraw(true);

			m_vSkill_2_Sprouts[i]->Set_PassIndex(19);
			m_vSkillPoint_Sprouts2[i]->Set_PassIndex(19);
		}
		else if (i == 2)
		{
			m_vSkill_2_Sprouts[i]->Set_TextureIndex(79);
			m_vSkillPoint_Sprouts2[i]->Set_IsDraw(true);

			m_vSkill_2_Sprouts[i]->Set_PassIndex(19);
			m_vSkillPoint_Sprouts2[i]->Set_PassIndex(19);
		}
	}

	m_vSkill_3_Sprouts[0]->Set_TextureIndex(71);
	m_vSkill_3_Sprouts[1]->Set_TextureIndex(73);
	m_vSkill_3_Sprouts[2]->Set_TextureIndex(75);
	for (_int i = 0; i < m_vSkillPoint_Sprouts3.size(); ++i)
	{
		m_vSkillPoint_Sprouts3[i]->Set_IsDraw(false);
		m_vSkillPoint_Sprouts3[i]->Set_PassIndex(1);
		m_vSkill_3_Sprouts[i]->Set_PassIndex(1);
	}

	for (_int i = 0; i < TempArryInt[2]; ++i)
	{
		if (i == 0)
		{
			m_vSkill_3_Sprouts[i]->Set_TextureIndex(77);
			m_vSkillPoint_Sprouts3[i]->Set_IsDraw(true);

			m_vSkill_3_Sprouts[i]->Set_PassIndex(19);
			m_vSkillPoint_Sprouts3[i]->Set_PassIndex(19);
		}
		else if (i == 1)
		{
			m_vSkill_3_Sprouts[i]->Set_TextureIndex(78);
			m_vSkillPoint_Sprouts3[i]->Set_IsDraw(true);

			m_vSkill_3_Sprouts[i]->Set_PassIndex(19);
			m_vSkillPoint_Sprouts3[i]->Set_PassIndex(19);
		}
		else if (i == 2)
		{
			m_vSkill_3_Sprouts[i]->Set_TextureIndex(79);
			m_vSkillPoint_Sprouts3[i]->Set_IsDraw(true);

			m_vSkill_3_Sprouts[i]->Set_PassIndex(19);
			m_vSkillPoint_Sprouts3[i]->Set_PassIndex(19);
		}
	}

}

void CSkillUI::DrawSkillName(_int iSkillNum)
{
	if (m_eWeaponType == WEAPON_BOW)
	{
		switch (iSkillNum)
		{
		case 0:
			m_SkillNameFont.szString = L"test";
			break;
		case 1:
			m_SkillNameFont.szString = L"test1";
			break;
		case 2:
			m_SkillNameFont.szString = L"test2";
			break;
		}
	}
	if (m_eWeaponType == WEAPON_SPEAR)
	{
		switch (iSkillNum)
		{
		case 0:
			m_SkillNameFont.szString = L"test3";
			break;
		case 1:
			m_SkillNameFont.szString = L"test4";
			break;
		case 2:
			m_SkillNameFont.szString = L"test5";
			break;
		}
	}
	if (m_eWeaponType == WEAPON_SWORDSHIELD)
	{
		switch (iSkillNum)
		{
		case 0:
			m_SkillNameFont.szString = L"test6";
			break;
		case 1:
			m_SkillNameFont.szString = L"test7";
			break;
		case 2:
			m_SkillNameFont.szString = L"test8";
			break;
		}
	}
}

void CSkillUI::DrawSkillExplanation(_int iSkillNum)
{
	if (m_eWeaponType == WEAPON_BOW)
	{
		switch (iSkillNum)
		{
		case 0:
			m_SkillExplanationFont.szString = L"Test YOU1";
			break;
		case 1:
			m_SkillExplanationFont.szString = L"Test YOU2";
			break;
		case 2:
			m_SkillExplanationFont.szString = L"Test YOU3";
			break;
		}
	}
	if (m_eWeaponType == WEAPON_SPEAR)
	{
		switch (iSkillNum)
		{
		case 0:
			m_SkillExplanationFont.szString = L"Test YOU4";
			break;
		case 1:
			m_SkillExplanationFont.szString = L"Test YOU5";
			break;
		case 2:
			m_SkillExplanationFont.szString = L"Test YOU6";
			break;
		}
	}
	if (m_eWeaponType == WEAPON_SWORDSHIELD)
	{
		switch (iSkillNum)
		{
		case 0:
			m_SkillExplanationFont.szString = L"Test YOU7";
			break;
		case 1:
			m_SkillExplanationFont.szString = L"Test YOU8";
			break;
		case 2:
			m_SkillExplanationFont.szString = L"Test YOU9";
			break;
		}
	}
}

HRESULT CSkillUI::Render_Fonts()
{
	CGameInstance* pInstance = g_pGameInstance;

	pInstance->Render_Font(L"VinerFonts", m_SkillNameFont.szString.c_str(), m_SkillNameFont.vPosition,
		m_SkillNameFont.vColor, m_SkillNameFont.fAngle, m_SkillNameFont.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_SkillExplanationFont.szString.c_str(), m_SkillExplanationFont.vPosition,
		m_SkillExplanationFont.vColor, m_SkillExplanationFont.fAngle, m_SkillExplanationFont.vFontScale);
	

	return S_OK;
}

HRESULT CSkillUI::Ready_Layer_UI()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region JB_UI
	CUI* UI = nullptr;
	CUI::SETTING_UI SettingUI;
	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Skill_BackGround
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_BackGround");
	SettingUI.m_fSizeX = 1280.f;
	SettingUI.m_fSizeY = 720.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 360.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 160;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackGround), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);


	//Skill_PhoenixUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_PhoenixUI");
	SettingUI.m_fSizeX = 148.f;
	SettingUI.m_fSizeY = 320.f;
	SettingUI.m_fX = 512.f;
	SettingUI.m_fY = 172.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 81;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_PhoenixUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI),&SettingUI);


	//Skill_MidleCircle
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_MidleCircleUI");
	SettingUI.m_fSizeX = 321.f;
	SettingUI.m_fSizeY = 302.f;
	SettingUI.m_fX = 638.f;
	SettingUI.m_fY = 369.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 59;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back((UI));


	//JB_Texture_48
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_48");
	SettingUI.m_fSizeX = 123.f;
	SettingUI.m_fSizeY = 146.f;
	SettingUI.m_fX = 638.f;
	SettingUI.m_fY = 370.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 48;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back((UI));

	//JB_Texture_84
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_84");
	SettingUI.m_fSizeX = 82.f;
	SettingUI.m_fSizeY = 94.f;
	SettingUI.m_fX = 638.f;
	SettingUI.m_fY = 366.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 84;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pSkillColorCircle), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);

	//JB_Texture_85
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_85");
	SettingUI.m_fSizeX = 62.f;
	SettingUI.m_fSizeY = 61.f;
	SettingUI.m_fX = 638.f;
	SettingUI.m_fY = 365.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 85;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pSkillFace), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSkillFace->Set_RenderSortValue(-1.f);

	//JB_Texture_63_Bow
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_63_Bow");
	SettingUI.m_fSizeX = 79.f;
	SettingUI.m_fSizeY = 102.f;
	SettingUI.m_fX = 642.f;
	SettingUI.m_fY = 201.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 63;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back((UI));

	//JB_Texture_63_Spear
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_63_Spear");
	SettingUI.m_fSizeX = 75.f;
	SettingUI.m_fSizeY = 107.f;
	SettingUI.m_fX = 778.f;
	SettingUI.m_fY = 461.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 63;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back((UI));

	//JB_Texture_63_SwordShield
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_63_SwordShield");
	SettingUI.m_fSizeX = 75.f;
	SettingUI.m_fSizeY = 107.f;
	SettingUI.m_fX = 505.f;
	SettingUI.m_fY = 464.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 63;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back((UI));

	//JB_Texture_64_Bow
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_64_Bow");
	SettingUI.m_fSizeX = 82.f;
	SettingUI.m_fSizeY = 103.f;
	SettingUI.m_fX = 643.f;
	SettingUI.m_fY = 201.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 64;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back((UI));

	//JB_Texture_64_Spear
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_64_Spear");
	SettingUI.m_fSizeX = 82.f;
	SettingUI.m_fSizeY = 103.f;
	SettingUI.m_fX = 779.f;
	SettingUI.m_fY = 462.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 64;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back((UI));

	//JB_Texture_64_SwordShield
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_64_SwordShield");
	SettingUI.m_fSizeX = 82.f;
	SettingUI.m_fSizeY = 103.f;
	SettingUI.m_fX = 505.f;
	SettingUI.m_fY = 464.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 64;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back((UI));

	//Skill_JB_Texture_24_BowUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_24_BowUI");
	SettingUI.m_fSizeX = 80;
	SettingUI.m_fSizeY = 132.f;
	SettingUI.m_fX = 642.f;
	SettingUI.m_fY = 195.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 24;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pWeaponBowUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pWeaponBowUI->Set_RenderSortValue(-1.f);

	//Skill_JB_Texture_99_SpearUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_99_SpearUI");
	SettingUI.m_fSizeX = 90.f;
	SettingUI.m_fSizeY = 107.f;
	SettingUI.m_fX = 776.f;
	SettingUI.m_fY = 463.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 99;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pWeaponSpearUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pWeaponSpearUI->Set_RenderSortValue(-1.f);

	//Skill_JB_Texture_90_SwordShieldUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_90_SwordShieldUI");
	SettingUI.m_fSizeX = 103.f;
	SettingUI.m_fSizeY = 98.f;
	SettingUI.m_fX = 507.f;
	SettingUI.m_fY = 465.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 90;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pSwordShield), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSwordShield->Set_RenderSortValue(-1.f);

	//Skill_JB_Texture_87_SkillGrass_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_87_SkillGrass_1");
	SettingUI.m_fSizeX = 138.f;
	SettingUI.m_fSizeY = 188.f;
	SettingUI.m_fX = 406.f;
	SettingUI.m_fY = 583.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 87;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pSkillGrass_1), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	
	//Skill_JB_Texture_129_SkillGrass_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_129_SkillGrass_2");
	SettingUI.m_fSizeX = 138.f;
	SettingUI.m_fSizeY = 188.f;
	SettingUI.m_fX = 630.f;//609.f;
	SettingUI.m_fY = 626.f;
	SettingUI.fAngle = 356.f;
	SettingUI.iTextureIndex = 129;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pSkillGrass_2), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	
	//Skill_JB_Texture_129_SkillGrass_3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_JB_Texture_129_SkillGrass_3");
	SettingUI.m_fSizeX = 138.f;
	SettingUI.m_fSizeY = 188.f;
	SettingUI.m_fX = 895.f;
	SettingUI.m_fY = 568.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 129;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pSkillGrass_3), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);

	//SkillImage_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillImage_1");
	SettingUI.m_fSizeX = 60.f;
	SettingUI.m_fSizeY = 96.f;
	SettingUI.m_fX = 384.f;
	SettingUI.m_fY = 611.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 40;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pSkill_1), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSkill_1->Set_RenderSortValue(-1.f);

	//SkillImage_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillImage_1");
	SettingUI.m_fSizeX = 62.f;
	SettingUI.m_fSizeY = 104.f;
	SettingUI.m_fX = 649.f;
	SettingUI.m_fY = 654.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 41;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pSkill_2), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSkill_2->Set_RenderSortValue(-1.f);

	//SkillImage_3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillImage_1");
	SettingUI.m_fSizeX = 62.f;
	SettingUI.m_fSizeY = 104.f;
	SettingUI.m_fX = 917.f;
	SettingUI.m_fY = 600.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 42;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pSkill_3), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSkill_3->Set_RenderSortValue(-1.f);

	//Skill_1_Sprout1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_1_Sprout1");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 60.f;
	SettingUI.m_fX = 331.f;
	SettingUI.m_fY = 533.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 77;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkill_1_Sprouts.push_back(UI);

	//Skill_1_Sprout2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_1_Sprout2");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 60.f;
	SettingUI.m_fX = 340.f;
	SettingUI.m_fY = 504.f;
	SettingUI.fAngle = -15.f;
	SettingUI.iTextureIndex = 78;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkill_1_Sprouts.push_back(UI);

	//Skill_1_Sprout3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_1_Sprout3");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 60.f;
	SettingUI.m_fX = 363.f;
	SettingUI.m_fY = 516.f;
	SettingUI.fAngle = -47.f;
	SettingUI.iTextureIndex = 79;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkill_1_Sprouts.push_back(UI);

	//Skill_2_Sprout1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_2_Sprout1");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 60.f;
	SettingUI.m_fX = 648.f + 26.f;
	SettingUI.m_fY = 562.f;
	SettingUI.fAngle = -25.f;
	SettingUI.iTextureIndex = 77;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkill_2_Sprouts.push_back(UI);

	//Skill_2_Sprout2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_2_Sprout2");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 60.f;
	SettingUI.m_fX = 679.f + 26.f;
	SettingUI.m_fY = 559.f;
	SettingUI.fAngle = -70.f;
	SettingUI.iTextureIndex = 78;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkill_2_Sprouts.push_back(UI);

	//Skill_2_Sprout3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_2_Sprout3");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 684.f + 26.f;
	SettingUI.m_fY = 581.f;
	SettingUI.fAngle = -107.f;
	SettingUI.iTextureIndex = 79;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkill_2_Sprouts.push_back(UI);

	//Skill_3_Sprout1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_3_Sprout1");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 925.f;
	SettingUI.m_fY = 505.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 77;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkill_3_Sprouts.push_back(UI);

	//Skill_3_Sprout2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_3_Sprout2");
	SettingUI.m_fSizeX = 60.f;
	SettingUI.m_fSizeY = 60.f;
	SettingUI.m_fX = 968.f;
	SettingUI.m_fY = 511.f;
	SettingUI.fAngle = -78.f;
	SettingUI.iTextureIndex = 78;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkill_3_Sprouts.push_back(UI);

	//Skill_3_Sprout3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_3_Sprout3");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 975.f;
	SettingUI.m_fY = 533.f;
	SettingUI.fAngle = -120.f;
	SettingUI.iTextureIndex = 79;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkill_3_Sprouts.push_back(UI);

	//SkillPoint1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoint1");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 440.f;
	SettingUI.m_fY = 302.f;
	SettingUI.fAngle = -238.f;
	SettingUI.iTextureIndex = 69;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back(UI);

	//SkillPoint2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoint2");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 417.f;
	SettingUI.m_fY = 249.f;
	SettingUI.fAngle = -249.f;
	SettingUI.iTextureIndex = 69;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back(UI);

	//SkillPoint3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoint3");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 404.f;
	SettingUI.m_fY = 190.f;
	SettingUI.fAngle = -265.f;
	SettingUI.iTextureIndex = 69;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back(UI);

	//SkillPoint4
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoint4");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 412.f;
	SettingUI.m_fY = 131.f;
	SettingUI.fAngle = -292.f;
	SettingUI.iTextureIndex = 69;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back(UI);

	//SkillPoint5
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoint5");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 441.f;
	SettingUI.m_fY = 83.f;
	SettingUI.fAngle = -310.f;
	SettingUI.iTextureIndex = 69;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back(UI);

	//SkillPoint6
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoint6");
	SettingUI.m_fSizeX = 50.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 485.f;
	SettingUI.m_fY = 48.f;
	SettingUI.fAngle = -331.f;
	SettingUI.iTextureIndex = 69;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vBasicUIs.push_back(UI);

	//SkillPoints1_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints1_1");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 457.f;
	SettingUI.m_fY = 303.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints1_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints1_2");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 436.f;
	SettingUI.m_fY = 304.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints1_3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints1_3");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 447.f;
	SettingUI.m_fY = 285.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints2_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints2_1");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 434.f;
	SettingUI.m_fY = 253.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints2_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints2_2");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 413.f;
	SettingUI.m_fY = 250.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints2_3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints2_3");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 427.f;
	SettingUI.m_fY = 233.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints3_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints3_1");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 419.f;
	SettingUI.m_fY = 198.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints3_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints3_2");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 400.f;
	SettingUI.m_fY = 189.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints3_3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints3_3");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 417.f;
	SettingUI.m_fY = 178.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints4_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints4_1");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 421.f;
	SettingUI.m_fY = 145.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints4_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints4_2");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 408.f;
	SettingUI.m_fY = 129.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints4_3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints4_3");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 429.f;
	SettingUI.m_fY = 126.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);


	//SkillPoints5_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints5_1");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 445.f;
	SettingUI.m_fY = 99.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints5_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints5_2");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 438.f;
	SettingUI.m_fY = 79.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints5_3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints5_3");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 459.f;
	SettingUI.m_fY = 83.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints6_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints6_1");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 484.f;
	SettingUI.m_fY = 65.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints6_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints6_2");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 484.f;
	SettingUI.m_fY = 44.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//SkillPoints6_3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("SkillPoints6_3");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 501.f;
	SettingUI.m_fY = 55.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillPoints.push_back(UI);

	//Skill_Sprouts1_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Sprouts1_1");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 325.f;
	SettingUI.m_fY = 537.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	UI->Set_RenderSortValue(-1.f);
	m_vSkillPoint_Sprouts1.push_back(UI);

	//Skill_Sprouts1_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Sprouts1_2");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 332.f;
	SettingUI.m_fY = 489.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	UI->Set_RenderSortValue(-1.f);
	m_vSkillPoint_Sprouts1.push_back(UI);

	//Skill_Sprouts1_3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Sprouts1_3");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 372.f;
	SettingUI.m_fY = 513.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	UI->Set_RenderSortValue(-1.f);
	m_vSkillPoint_Sprouts1.push_back(UI);

	//Skill_Sprouts2_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Sprouts2_1");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 18.f;
	SettingUI.m_fX = 642.f + 27.f;
	SettingUI.m_fY = 561.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	UI->Set_RenderSortValue(-1.f);
	m_vSkillPoint_Sprouts2.push_back(UI);

	//Skill_Sprouts2_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Sprouts2_2");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 686.f + 25.f;
	SettingUI.m_fY = 545.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	UI->Set_RenderSortValue(-1.f);
	m_vSkillPoint_Sprouts2.push_back(UI);

	//Skill_Sprouts2_3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Sprouts2_3");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 690.f + 25.f;
	SettingUI.m_fY = 587.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	UI->Set_RenderSortValue(-1.f);
	m_vSkillPoint_Sprouts2.push_back(UI);

	//Skill_Sprouts3_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Sprouts3_1");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 919.f;
	SettingUI.m_fY = 506.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	UI->Set_RenderSortValue(-1.f);
	m_vSkillPoint_Sprouts3.push_back(UI);

	//Skill_Sprouts3_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Sprouts3_2");
	SettingUI.m_fSizeX = 16.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 976.f;
	SettingUI.m_fY = 496.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	UI->Set_RenderSortValue(-1.f);
	m_vSkillPoint_Sprouts3.push_back(UI);
	
	//Skill_Sprouts3_3
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Sprouts3_3");
	SettingUI.m_fSizeX = 15.f;
	SettingUI.m_fSizeY = 15.f;
	SettingUI.m_fX = 980.f;
	SettingUI.m_fY = 541.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 130;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	UI->Set_RenderSortValue(-1.f);
	m_vSkillPoint_Sprouts3.push_back(UI);

	//Skill_Explanation1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Explanation1");
	SettingUI.m_fSizeX = 77.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 850.f;
	SettingUI.m_fY = 161.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 20;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillExplanations.push_back(UI);

	//Skill_Explanation2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Explanation2");
	SettingUI.m_fSizeX = 77.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 1157.f;
	SettingUI.m_fY = 161.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 133;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillExplanations.push_back(UI);

	//Skill_Name
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_Name");
	SettingUI.m_fSizeX = 202;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 1007.f;
	SettingUI.m_fY = 167.f;
	SettingUI.fAngle = 0;
	SettingUI.iTextureIndex = 19;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vSkillExplanations.push_back(UI);

#pragma endregion JB_UI

#pragma region JY_EFFECT

	CUI::SETTING_UI tEffectSettingDesc;
	ZeroMemory(&tEffectSettingDesc, sizeof(tEffectSettingDesc));


	{
		//BowEffect
		tEffectSettingDesc.bClick = false;
		tEffectSettingDesc.bMove = false;
		tEffectSettingDesc.bDraw = true;
		tEffectSettingDesc.bColl = false;
		tEffectSettingDesc.iLevelIndex = m_eNowSceneNum;
		tEffectSettingDesc.pUI_Name = TEXT("UI_Effect");
		tEffectSettingDesc.m_fSizeX = 100.f;
		tEffectSettingDesc.m_fSizeY = 100.f;
		tEffectSettingDesc.m_fX = 640.f;
		tEffectSettingDesc.m_fY = 365.f;
		tEffectSettingDesc.fAngle = 0;

		tEffectSettingDesc.eUIKindsID = CUI::UIID_EFFECT;
		tEffectSettingDesc.iTextureIndex = 300;
		tEffectSettingDesc.iNoiseTextureIndex = 381;
		tEffectSettingDesc.iMaskTextureIndex = 96;


		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &tEffectSettingDesc);
		m_vecUIEffect.push_back(UI);
	}
	{
		//FireEffect
		tEffectSettingDesc.bClick = false;
		tEffectSettingDesc.bMove = false;
		tEffectSettingDesc.bDraw = true;
		tEffectSettingDesc.bColl = false;
		tEffectSettingDesc.iLevelIndex = m_eNowSceneNum;
		tEffectSettingDesc.pUI_Name = TEXT("UI_Effect");
		tEffectSettingDesc.m_fSizeX = 100.f;
		tEffectSettingDesc.m_fSizeY = 100.f;
		tEffectSettingDesc.m_fX = 640.f;
		tEffectSettingDesc.m_fY = 365.f;
		tEffectSettingDesc.fAngle = 0;

		tEffectSettingDesc.eUIKindsID = CUI::UIID_EFFECT;
		tEffectSettingDesc.iTextureIndex = 299;
		tEffectSettingDesc.iNoiseTextureIndex = 381;
		tEffectSettingDesc.iMaskTextureIndex = 96;


		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &tEffectSettingDesc);
		m_vecUIEffect.push_back(UI);
	}
	{
		//SwordEffect
		tEffectSettingDesc.bClick = false;
		tEffectSettingDesc.bMove = false;
		tEffectSettingDesc.bDraw = true;
		tEffectSettingDesc.bColl = false;
		tEffectSettingDesc.iLevelIndex = m_eNowSceneNum;
		tEffectSettingDesc.pUI_Name = TEXT("UI_Effect");
		tEffectSettingDesc.m_fSizeX = 100.f;
		tEffectSettingDesc.m_fSizeY = 100.f;
		tEffectSettingDesc.m_fX = 640.f;
		tEffectSettingDesc.m_fY = 365.f;
		tEffectSettingDesc.fAngle = 0;

		tEffectSettingDesc.eUIKindsID = CUI::UIID_EFFECT;
		tEffectSettingDesc.iTextureIndex = 271;
		tEffectSettingDesc.iNoiseTextureIndex = 381;
		tEffectSettingDesc.iMaskTextureIndex = 96;


		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &tEffectSettingDesc);
		m_vecUIEffect.push_back(UI);
	}


#pragma endregion JY_EFFECT

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CSkillUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	return S_OK;
}

CSkillUI * CSkillUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CSkillUI*	pInstance = NEW CSkillUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CSkillUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSkillUI::Clone(void * pArg)
{
	CSkillUI*	pInstance = NEW CSkillUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CSkillUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkillUI::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);

	Safe_Release(m_PhoenixUI);
	Safe_Release(m_pSkillColorCircle);
	Safe_Release(m_pSkillFace);
	Safe_Release(m_pWeaponBowUI);
	Safe_Release(m_pWeaponSpearUI);
	Safe_Release(m_pSwordShield);
	Safe_Release(m_pBackGround);
	Safe_Release(m_pSkillGrass_1);
	Safe_Release(m_pSkillGrass_2);
	Safe_Release(m_pSkillGrass_3);
	Safe_Release(m_pSkill_1);
	Safe_Release(m_pSkill_2);
	Safe_Release(m_pSkill_3);
	//Safe_Release(m_MidleCircle);

	for (auto& UI : m_vBasicUIs)
		Safe_Release(UI);

	for (auto& Sprout : m_vSkill_1_Sprouts)
		Safe_Release(Sprout);

	for (auto& Sprout2 : m_vSkill_2_Sprouts)
		Safe_Release(Sprout2);

	for (auto& Sprout3 : m_vSkill_3_Sprouts)
		Safe_Release(Sprout3);
	for (auto& SkillPoint : m_vSkillPoints)
		Safe_Release(SkillPoint);

	for (auto& SproutSkillPoint : m_vSkillPoint_Sprouts1)
		Safe_Release(SproutSkillPoint);

	for (auto& SproutSkillPoint : m_vSkillPoint_Sprouts2)
		Safe_Release(SproutSkillPoint);

	for (auto& SproutSkillPoint : m_vSkillPoint_Sprouts3)
		Safe_Release(SproutSkillPoint);

	for (auto& SkillExplanation : m_vSkillExplanations)
		Safe_Release(SkillExplanation);

	for (auto& UIEffect : m_vecUIEffect)
		Safe_Release(UIEffect);
	

	m_vBasicUIs.clear();
	m_vSkill_1_Sprouts.clear();
	m_vSkill_2_Sprouts.clear();
	m_vSkill_3_Sprouts.clear();
	m_vSkillPoints.clear();
	m_vSkillPoint_Sprouts1.clear();
	m_vSkillPoint_Sprouts2.clear();
	m_vSkillPoint_Sprouts3.clear();
	m_vSkillExplanations.clear();
	m_vecUIEffect.clear();
	
}
