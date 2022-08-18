#include "stdafx.h"
#include "..\public\Chiedtian.h"
#include "Chiedtuan_Weapon.h"
#include "Chiedtuan_2Page_Weapon.h"
#include "Camera_Main.h"
#include "HpUI.h"
#include "Volcano.h"
#include "Player.h"
#include "ParticleCollider.h"
#include "Particle_ColliderInOut.h"

#include "InstanceMonsterBatchTrigger.h"

CChiedtian::CChiedtian(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CBoss(pDevice, pDeviceContext)
{
}

CChiedtian::CChiedtian(const CChiedtian & rhs)
	: CBoss(rhs)
{
}

HRESULT CChiedtian::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CChiedtian::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());



	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));

	m_pTransformCom->Scaled_All(_float3(1.5f, 1.5f, 1.5f));

	m_pModel->Change_AnimIndex(1);

	m_fAttackCoolTime = 2.f;
	m_fSkillCoolTime = 5.f;
	m_bIsHit = false;
	Set_IsOcllusion(true);
	Set_LimLight_N_Emissive(_float4(0), _float4(0.00f, 0.2f, 1.f,0.f));

	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));

	CChiedtuan_Weapon::WEAPOPNDESC WeaponDesc;

	////TR
	//WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_r", XMVectorSet(10.f,10.f,10.f, 0.f), XMVectorSet(170.f, 0.f, 0.f, 0.f), XMVectorSet(-3.00f *10.f, -0.250f*10.f, -4.320f*10.f, 1.f));
	//WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_TR;
	//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Weapon"), TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);

	////RL
	//WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_l", XMVectorSet(10.f, 10.f, 10.f, 0.f), XMVectorSet(170.f, -90.f, 0.f, 0.f), XMVectorSet(3.120f * 10.f, -0.670f * 10.f, -4.240f * 10.f, 1.f));
	//WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_TL;
	//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Weapon"), TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);

	////BR
	////WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_r", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(0.f, -90.f, 0.f, 0.f), XMVectorSet(-1.250f * 8.f, -2.140f * 8.f, -3.180f * 8.f, 1.f));
	//WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_r", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(-55.f, -80.f, 50.f, 0.f), XMVectorSet(-1.050f * 8.f,-1.430f* 8.f,-3.230f* 8.f,0));
	//WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BR;
	//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Weapon"), TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	//CChiedtuan_Weapon* Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Weapon"));
	//Weapon->Set_WaistgirdScal();

	////BL
	//WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_l", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(-55.f, -75.f, 50.f, 0.f), XMVectorSet(1.050f * 8.f, -1.430f* 8.f, -3.230f* 8.f, 0));
	//WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BL;
	//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Weapon"), TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	//Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Weapon"));
	//Weapon->Set_WaistgirdScal();


	CChiedtuan_Weapon* ChiedtuanWeapon = nullptr;

	//주무기
	WeaponDesc.BossObj = this;
	WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_r", XMVectorSet(10.f, 10.f, 10.f, 0.f), XMVectorSet(170.f, 0.f, 0.f, 0.f), XMVectorSet(-3.00f *10.f, -0.250f*10.f, -4.320f*10.f, 1.f));
	WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_TR;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&ChiedtuanWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	m_pMainWeapons.push_back(ChiedtuanWeapon);

	WeaponDesc.BossObj = this;
	WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_l", XMVectorSet(10.f, 10.f, 10.f, 0.f), XMVectorSet(170.f, -90.f, 0.f, 0.f), XMVectorSet(3.120f * 10.f, -0.670f * 10.f, -4.240f * 10.f, 1.f));
	WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_TL;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&ChiedtuanWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	m_pMainWeapons.push_back(ChiedtuanWeapon);

	//서브무기
	WeaponDesc.BossObj = this;
	WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_r", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(-55.f, -80.f, 50.f, 0.f), XMVectorSet(-1.050f * 8.f, -1.430f* 8.f, -3.230f* 8.f, 0));
	WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BR;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&ChiedtuanWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	m_pSubWeapons.push_back(ChiedtuanWeapon);

	WeaponDesc.BossObj = this;
	WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_l", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(-55.f, -75.f, 50.f, 0.f), XMVectorSet(1.050f * 8.f, -1.430f* 8.f, -3.230f* 8.f, 0));
	WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BL;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&ChiedtuanWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	m_pSubWeapons.push_back(ChiedtuanWeapon);


	//2Page_Weapon
	CChiedtuan_2Page_Weapon::WEAPOPNDESC SecondPageWeaponDesc;
	CChiedtuan_2Page_Weapon* SecondPageWeapon = nullptr;

	SecondPageWeaponDesc.BossObj = this;
	SecondPageWeaponDesc.KatanaPOSType = CChiedtuan_2Page_Weapon::KATANA_TR;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&SecondPageWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon_2Page), &SecondPageWeaponDesc);
	m_pSecondPageWeapons.push_back(SecondPageWeapon);

	SecondPageWeaponDesc.BossObj = this;
	SecondPageWeaponDesc.KatanaPOSType = CChiedtuan_2Page_Weapon::KATANA_TL;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&SecondPageWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon_2Page), &SecondPageWeaponDesc);
	m_pSecondPageWeapons.push_back(SecondPageWeapon);

	SecondPageWeaponDesc.BossObj = this;
	SecondPageWeaponDesc.KatanaPOSType = CChiedtuan_2Page_Weapon::KATANA_BR;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&SecondPageWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon_2Page), &SecondPageWeaponDesc);
	m_pSecondPageWeapons.push_back(SecondPageWeapon);

	SecondPageWeaponDesc.BossObj = this;
	SecondPageWeaponDesc.KatanaPOSType = CChiedtuan_2Page_Weapon::KATANA_BL;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&SecondPageWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon_2Page), &SecondPageWeaponDesc);
	m_pSecondPageWeapons.push_back(SecondPageWeapon);



	CHpUI::HPDesc HpDesc;
	HpDesc.m_HPType = CHpUI::HP_MONSTER;
	HpDesc.m_pObjcect = this;
	HpDesc.m_vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	HpDesc.m_Dimensions = 3.f;

	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pHPUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_HpUI), &HpDesc);



	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(0, 0, 0));
	m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

	FAILED_CHECK(Ready_ParticleDesc());

	//m_bIsHit = true;

	// JH
	/*_int iRandom = rand() % 3 + 1;

	wstring teampString;
	teampString = L"JJB_Chieftain_Intro100%_" + to_wstring(iRandom) + L".wav";

	g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);*/
	m_bBlockUpdate = true;
	m_pTransformCom->Rotation_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(1.177f, 40.21f, 322.647f));
	m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	m_fAttachCamPos.y += 5.f;
	m_fAttachCamPos.z -= 10.f;
	m_fDelayTime = 3.f;
	m_pModel->Change_AnimIndex(14);
	m_bIsMainWeaponOff = true;
	//

	//m_bBlockUpdate = false;
	//m_bIsHit = true;
	return S_OK;
}

_int CChiedtian::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;


	// JH
	m_pDissolve->Update_Dissolving(fDeltaTime);
	if (true == m_bBlockUpdate)
	{
		Update_Direction(fDeltaTime);

		_Vector vTestPos = GetSingle(CUtilityMgr)->Get_MainCamera()->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_POS);

		if (false == m_bBlockAnim)
		{
			FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple* m_fTestHPIndex), m_bIsOnScreen));
		}

		return _int();
	}
	//

	m_fAttackCoolTime -= (_float)fDeltaTime;
	m_fSkillCoolTime -= (_float)fDeltaTime;
	m_fJumpTime -= (_float)fDeltaTime;

	m_fNarration -= (_float)fDeltaTime;


	if (m_fNarration <= 0)
	{
		m_fNarration = 15.f;

		_int iRandom = rand() % 11;

		wstring teampString;
		teampString = L"JJB_Chieftain_Naration_" + to_wstring(iRandom) + L".wav";

		g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

	}
	if (m_fHP <= 0)
	{
		m_bIsAttack = true;
		m_pModel->Change_AnimIndex(3, 1.f);

		// Jino
		static_cast<CPlayer*>(m_pPlayerObj)->Set_Targeting(nullptr);
		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
		m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		m_fAttachCamPos.y += 5.f;
		m_fAttachCamPos.z -= 10.f;
		GetSingle(CUtilityMgr)->Get_MainCamera()->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 0.f));
		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_FocusTarget(this);
		/*static_cast<CPlayer*>(m_pPlayerObj)->Set_State_StopActionStart();*/
		//

		if (m_bInstanceMonsterDieSwitch == false)
		{
			CInstanceMonsterBatchTrigger* pMonsterBatchTrigger = static_cast<CInstanceMonsterBatchTrigger*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_InstanceMonsterTrigger)));
			pMonsterBatchTrigger->Set_MonsterAllDie(true);
			m_bInstanceMonsterDieSwitch = true;
		}

	}

	if (!m_ActivateSecondPage)
	{
		if (g_pGameInstance->Get_DIKeyState(DIK_J)& DIS_Down)
		{
			m_ActivateSecondPage = true;
			if(m_bIsAtaackAimEnd)
				m_bIsHit = true;
		}

		if (m_fHP <= m_fHP / 2.f)
		{
			if (m_bIsAtaackAimEnd)
				m_bIsHit = true;
		}

		if (m_bIsFireAttack)
			m_fFireTime -= (_float)fDeltaTime;

		if (m_fFireTime <= 0 && m_bIsFireAttack)
		{
			m_fFireTime = 10.f;
			m_bIsFireAttack = false;

			g_pGameInstance->Stop_ChannelSound(CHANNEL_MONSTER);
			g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Flamethrower_END.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
			m_pModel->Change_AnimIndex_ReturnTo(14, 1);
		}

		if (m_bIsSpinAttack)
		{
			m_fSpinTime -= (_float)fDeltaTime;

		
			static _double ParticleTimer = 0;
			ParticleTimer += fDeltaTime;
			if (ParticleTimer > 0.2f)
			{
				ParticleTimer = 0;


				CUtilityMgr*	pUtil = GetSingle(CUtilityMgr);
				INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_17");

				tDesc.FollowingTarget = nullptr;
				tDesc.PowerRandomRange = _float2(-tDesc.PowerRandomRange.y, tDesc.PowerRandomRange.y);

				m_vecNonInstMeshDesc[6].vLookDir = pUtil->RandomFloat3(-1, 1).Get_Nomalize();
				m_vecNonInstMeshDesc[7].vLookDir = m_vecNonInstMeshDesc[8].vLookDir =
					m_vecNonInstMeshDesc[6].vLookDir.Get_Cross(m_vecNonInstMeshDesc[6].vLookDir.Get_Cross(XMVectorSet(0, 1, 0, 0)));

				m_vecNonInstMeshDesc[6].SizeSpeed = 1.8f;
				m_vecNonInstMeshDesc[7].SizeSpeed = 9.36f;
				m_vecNonInstMeshDesc[8].SizeSpeed = 5.4f;

				tDesc.vFixedPosition = m_vecNonInstMeshDesc[6].vPosition = m_vecNonInstMeshDesc[7].vPosition =
					m_vecNonInstMeshDesc[8].vPosition =

					m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
					XMVector3Normalize(
						m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1.f, 1.f) +
						m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * pUtil->RandomFloat(0.25f, 0.5f) +
						m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1.f, 1.f)) * 10.f;

				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[6]);
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[7]);
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[8]);


				CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
				CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

				CParticleCollider::SETTINGCOLLIDER ColliderDesc;
				ColliderDesc.ColliderType = COLLIDER_SPHERE;
				ColliderDesc.ColliderDesc.vScale = _float3(8.5f, 8.5f, 8.5f);
				ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
				ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
				ColliderDesc.pTargetTransform = Transform;

				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
					TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);


				pUtil->Create_TextureInstance(m_eNowSceneNum, tDesc);


				m_vecNonInstMeshDesc[6].vLookDir = _float3(0, 0, 1);
				m_vecNonInstMeshDesc[7].vLookDir = _float3(0, 1, 0);
				m_vecNonInstMeshDesc[8].vLookDir = _float3(0, 1, 0);
			}

			if (m_fSpinTime > 4.f)
			{
				if (m_fSpinSpeed > 10.f)
				{
					m_fSpinSpeed = 10.f;
					m_fAnimmultiple = 1.4f;
				}
				else
				{
					m_fSpinSpeed += (_float)fDeltaTime* 5.f;
					m_fAnimmultiple = (m_fSpinSpeed *0.1f) + 0.4f;
				}

			

			}
			else if (m_fSpinTime < 4.f)
			{
				if (m_fSpinSpeed <= 0.f)
				{
					m_fSpinSpeed = 0.1f;
					m_fAnimmultiple = 0.2f;
				}
				else
				{
					m_fSpinSpeed -= (_float)fDeltaTime*3.f;
					m_fAnimmultiple = (m_fSpinSpeed *0.1f) + 0.1f;
				}


				_float Value = max(min(g_pGameInstance->Easing(TYPE_Linear, 2.f, 0.f, (_float)4.f - m_fSpinTime, 4.f), 1.f), 0) * 0.4f;

				m_pMainWeapons[1]->Set_LimLight_N_Emissive(_float4(0.60546875f, 0.06640625f, 0.1171875f, 1.f).XMVector() * Value,
					_float4(1.f, 0.5f, 1.f, 0.f).XMVector() *Value);
				m_pMainWeapons[0]->Set_LimLight_N_Emissive(_float4(0.60546875f, 0.06640625f, 0.1171875f, 1.f).XMVector() * Value,
					_float4(1.f, 0.5f, 1.f, 0.f).XMVector() *Value);

				m_pSubWeapons[1]->Set_LimLight_N_Emissive(_float4(0.60546875f, 0.06640625f, 0.1171875f, 1.f).XMVector() * Value,
					_float4(1.f, 0.5f, 1.f, 0.f).XMVector() *Value);
				m_pSubWeapons[0]->Set_LimLight_N_Emissive(_float4(0.60546875f, 0.06640625f, 0.1171875f, 1.f).XMVector() * Value,
					_float4(1.f, 0.5f, 1.f, 0.f).XMVector() *Value);

			}
		}

		if (m_fSpinTime <= 0 && m_bIsSpinAttack)
		{
			m_fSpinTime = 14.f;
			m_bIsSpinAttack = false;

			m_fAttackCoolTime = 1.f;
			m_fSkillCoolTime = 5.f;


			for (auto& Weapon : m_pMainWeapons) Weapon->SwordTrailOnOff(false);
			for (auto& Weapon : m_pSubWeapons) Weapon->SwordTrailOnOff(false);

			for (_int i = 0; i < m_pMainWeapons.size(); ++i)
				m_pMainWeapons[i]->Set_IsAttackState(false);

			for (_int i = 0; i < m_pSubWeapons.size(); ++i)
				m_pSubWeapons[i]->Set_IsAttackState(false);

			CChiedtuan_Weapon::WEAPOPNDESC WeaponDesc;

			//CChiedtuan_Weapon* Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Weapon"), 2);
			CChiedtuan_Weapon* Weapon = m_pSubWeapons[0];
			WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_r", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(0.f, -90.f, 0.f, 0.f), XMVectorSet(-1.250f * 8.f, -2.140f * 8.f, -3.180f * 8.f, 1.f));
			WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BR;
			Weapon->Set_WeaponDesc(WeaponDesc);
			Weapon->Set_WaistgirdScal();

			//Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Weapon"), 3);
			Weapon = m_pSubWeapons[1];
			WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_l", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(-55.f, -80.f, 50.f, 0.f), XMVectorSet(1.070f * 8.f, -1.160f * 8.f, -2.590f * 8.f, 1.f));
			WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BL;
			Weapon->Set_WeaponDesc(WeaponDesc);
			Weapon->Set_WaistgirdScal();

			m_fAnimmultiple = 1.f;
			m_pModel->Change_AnimIndex(1, 0.8f);

			g_pGameInstance->Stop_ChannelSound(CHANNEL_MONSTER);
			g_pGameInstance->Play3D_Sound(L"JJB_Chief_Tornedo_Wind_End.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

		}

		CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
		_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);
		PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;

		_float3 TargetDir = XMVector3Normalize(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		m_vAngle = XMVector3Dot(XMLoadFloat3(&TargetDir), XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)));

		m_fRange = XMVectorGetX(XMVector3Length(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));

		if (m_bIsLookAt)
		{
			_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			Dir = XMVector3Normalize(PlayerPos.XMVector() - XMVectorSetY(Dir, PlayerPos.y));
			m_pTransformCom->Turn_Dir(Dir, 0.90f);
		}

		if (!m_bIsAttack && m_fRange > 8.f && !m_bIsHit)
		{
			m_bIsLookAt = true;
			m_pModel->Change_AnimIndex(2);
		}

		if (g_pGameInstance->Get_DIKeyState(DIK_N)& DIS_Down)
		{
			m_fTestHPIndex += 0.2f;
		}
		if (m_fTestHPIndex >= 1.4f)
			m_bIsHalf = true;


		//맞았을때
		if (m_bIsHit)
		{
			m_bIsHit = false;
			m_bIsAttack = true;

			// JH
			if (m_fHP > 0.f)
			{
				m_pModel->Change_AnimIndex(4);
			}
		}
		//점프
		if (!m_bIsHit && !m_bIsAttack && m_fRange < 8.f &&m_fJumpTime <= 0)
		{
			m_bIsAttack = true;
			m_pModel->Change_AnimIndex_ReturnTo(10, 1);
		}
		//일반 공격
		else if (m_fAttackCoolTime <= 0 && m_fRange > 2.f && m_fRange < 8.f && !m_bIsAttack && !m_bIsHit)
		{
			m_bIsAttack = true;
			m_bIsBasicAttack = true;

			m_pModel->Change_AnimIndex_ReturnTo(5, 1);

		}
		/////////////////////////스킬 공격
		else if (m_fSkillCoolTime <= 0 && !m_bIsAttack && !m_bIsHit)
		{
			_int iRandom = (_int)(GetSingle(CUtilityMgr)->RandomFloat(0.f, 299.f) * 0.01f);
			m_bIsAttack = true;
			m_bISkill = true;

			switch (m_iTest)
			{
			case ATTACK_FIRE:
			{
				m_pModel->Change_AnimIndex_ReturnTo(12, 13);
				for (auto& Weapon : m_pMainWeapons) Weapon->SwordTrailOnOff(false);
				for (auto& Weapon : m_pSubWeapons) Weapon->SwordTrailOnOff(false);
			}
			break;
			case ATTACK_SPIN:
			{
				m_pModel->Change_AnimIndex_UntilNReturn(7, 8, 9);
				//for (auto& Weapon : m_pSubWeapons) Weapon->SwordTrailOnOff(false);
				//for (auto& Weapon : m_pSubWeapons) Weapon->SwordTrailOnOff(false);
			}
			break;

			case ATTACK_WHINING:
			{
				m_pModel->Change_AnimIndex_ReturnTo(6, 1);
				for (auto& Weapon : m_pMainWeapons) Weapon->SwordTrailOnOff(false);
				for (auto& Weapon : m_pSubWeapons) Weapon->SwordTrailOnOff(false);
			}
			break;

			}
			++m_iTest;

			if (m_iTest > 2)
				m_iTest = 0;
		}

	}
	//SecondPage3
	else
	{
		CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
		_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);
		PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;

		_float3 TargetDir = XMVector3Normalize(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		m_vAngle = XMVector3Dot(XMLoadFloat3(&TargetDir), XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)));

		m_fRange = XMVectorGetX(XMVector3Length(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));

		//맵 중앙으로 이동
		if (!m_bMiddlepointPos)
		{
			m_bIsMainWeaponOff = true;

			for (_int i = 0; i < m_pMainWeapons.size(); ++i)
				m_pMainWeapons[i]->Set_IsAttackState(false);

			for (_int i = 0; i < m_pSubWeapons.size(); ++i)
				m_pSubWeapons[i]->Set_IsAttackState(false);

			m_bMiddlepointPos = true;
			m_pModel->Change_AnimIndex(10);
			m_bIsLookAt = true;
			m_fAttackCoolTime = 3.f;
			m_fSkillCoolTime = 8.f;

			m_bIsSpinAttack = false;
			m_bIsFireAttack = false;
			m_bIsBasicAttack = false;
			m_bIsJump = false;

			m_fAnimmultiple = 1.f;

			for (auto& SecondPageWeapon : m_pSecondPageWeapons)
			{
				SecondPageWeapon->Set_AttackfinishOff(true);
			}

		}

		if (m_bIsLookAt)
		{
			_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			Dir = XMVector3Normalize(PlayerPos.XMVector() - XMVectorSetY(Dir, PlayerPos.y));
			m_pTransformCom->Turn_Dir(Dir, 0.90f);
		}

		if (m_bIsSpinAttack)
		{
			m_fSpinTime -= (_float)fDeltaTime;



			if (m_fSpinTime > 4.f)
			{
				if (m_fSpinSpeed > 10.f)
				{
					m_fSpinSpeed = 10.f;
					m_fAnimmultiple = 1.4f;
				}
				else
				{
					m_fSpinSpeed += (_float)fDeltaTime* 5.f;
					m_fAnimmultiple = (m_fSpinSpeed *0.1f) + 0.4f;
				}
			}
			else if (m_fSpinTime < 4.f)
			{
				if (m_fSpinSpeed <= 0.f)
				{
					m_fSpinSpeed = 0.1f;
					m_fAnimmultiple = 0.2f;

					for (auto& SecondPageWeapon : m_pSecondPageWeapons)
					{
						SecondPageWeapon->Set_Dissolve(false);
					}
				}
				else
				{
					m_fSpinSpeed -= (_float)fDeltaTime*3.f;
					m_fAnimmultiple = (m_fSpinSpeed *0.1f) + 0.1f;
				}



			}
		}

		if (m_fSpinTime <= 0 && m_bIsSpinAttack)
		{
			m_fSpinTime = 14.f;
			m_bIsSpinAttack = false;

			m_fAttackCoolTime = 3.f;
			m_fSkillCoolTime = 12.f;
			m_bISVolcanoAttackStart = true;

			m_fAnimmultiple = 1.f;
			m_pModel->Change_AnimIndex(1, 0.8f);

		/*	g_pGameInstance->Stop_ChannelSound(CHANNEL_MONSTER);
			g_pGameInstance->Play3D_Sound(L"JJB_Chief_Tornedo_Wind_End.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);*/
			for (auto& SecondPageWeapon : m_pSecondPageWeapons)
			{
				SecondPageWeapon->SpinAttackOff();
			}

			CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_ParticleColliderInOut));
			Obj->Set_IsDead();

		}

		if (m_bIsVolcanoAttack)
		{
			m_fVolcanoTime -= (_float)fDeltaTime;
		}

		if (m_fVolcanoTime <= 0 && m_bIsVolcanoAttack)
		{
			m_bIsVolcanoAttack = false;
			m_fVolcanoTime = 10.f;

			m_fAttackCoolTime = 3.f;
			m_fSkillCoolTime = 12.f;

			m_fAnimmultiple = 1.f;
			m_pModel->Change_AnimIndex(1, 0.8f);


			for (auto& SecondPageWeapon : m_pSecondPageWeapons)
			{
				SecondPageWeapon->VolcanoAttackOff();
			}
		}
		
		//일반 공격
		if (m_fAttackCoolTime <= 0 && !m_bIsAttack && !m_bIsHit)
		{
			m_bIsAttack = true;
			m_bIsBasicAttack = true;

			m_pModel->Change_AnimIndex(5);
			for (auto& SecondPageWeapon : m_pSecondPageWeapons)
			{
				SecondPageWeapon->Set_InitialPosDessolve(false);
			}

		}
		//스킬 공격
		else if (m_fSkillCoolTime <= 0 && !m_bIsAttack && !m_bIsHit)
		{
			m_bIsAttack = true;
			_int iRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.f, 1.9f);
			m_bIsAttack = true;
			m_bISkill = true;
			//특정 스킬 다시하기
			//iRandom = 0;

			switch (iRandom)
			{
			case SECONDPAGEATTACK_SPIN:
			{
				m_pModel->Change_AnimIndex_ReturnTo(7, 8);
			}
			break;

			case SECONDPAGEATTACK_VOLCANO:
			{
				m_pModel->Change_AnimIndex_ReturnTo(10, 0);
				CVolcano::VolcanoCount2Zero();
				m_bISVolcanoAttackStart = true;
			}
			break;

			}
		}

	}

	
	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), 10);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple* m_fTestHPIndex), m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	m_pCollider->Update_ConflictPassedTime(fDeltaTime);

	_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();

	for (_uint i = 0; i < iNumCollider; i++)
		m_pCollider->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	//FireSkill_Collider
	m_pFireCollider->Update_ConflictPassedTime(fDeltaTime);
	m_pFireCollider->Update_Transform(0, m_FireAttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed());

	//Jump_Collider
	m_pJumpCollider->Update_ConflictPassedTime(fDeltaTime);
	m_pJumpCollider->Update_Transform(0, m_JumpAttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed());

	if (m_bIsFireAttack)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pFireCollider));
	}

	if (m_bIsJump)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pJumpCollider));
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pCollider));
	FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_pTransformCom, 1.5f, m_pNavigationCom));

	if (!m_ActivateSecondPage)
	{
		if (!m_bIsMainWeaponOff)
		{
			for (auto& Weapon : m_pMainWeapons)
				Weapon->Update(fDeltaTime);
		}

		for (auto& SubWeapon : m_pSubWeapons)
			SubWeapon->Update(fDeltaTime);
	}
	else
	{
		for (auto& SecondPageWeapon : m_pSecondPageWeapons)
			SecondPageWeapon->Update(fDeltaTime);
	}

	if (m_pHPUI != nullptr)
		m_pHPUI->Update(fDeltaTime);

	return _int();
}

_int CChiedtian::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	//FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pFireCollider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pJumpCollider));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	//g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);
 
	if (!m_ActivateSecondPage)
	{
		if (!m_bIsMainWeaponOff)
		{
			for (auto& Weapon : m_pMainWeapons)
				Weapon->LateUpdate(fDeltaTime);

			for (auto& SubWeapon : m_pSubWeapons)
				SubWeapon->LateUpdate(fDeltaTime);
		}
	}
	else
	{
		for (auto& SecondPageWeapon : m_pSecondPageWeapons)
			SecondPageWeapon->LateUpdate(fDeltaTime);
	}

	// JH
	if(false == m_bBlockUpdate)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));
	
	if (m_pHPUI != nullptr)
		m_pHPUI->LateUpdate(fDeltaTime);

	return _int();
}

_int CChiedtian::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolve->Render(7));

	//_uint NumMaterial = m_pModel->Get_NumMaterial();

	//for (_uint i = 0; i < NumMaterial; i++)
	//{
	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	//}
#ifdef _DEBUG
	//m_pNavigationCom->Render(m_pTransformCom);
#endif // _DEBUG
	return _int();
}

_int CChiedtian::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CChiedtian::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		if (m_pFireCollider == pMyCollider)
		{
			_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
			pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
			pConflictedCollider->Set_Conflicted(1.f);
		}
		else if (m_pJumpCollider == pMyCollider)
		{
			_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
			pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
			pConflictedCollider->Set_Conflicted(1.f);
		}
	}
}

_float CChiedtian::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	m_fHP -= fDamageAmount;

	m_pHPUI->Set_ADD_HitCount();

	return _float();
}

void CChiedtian::Change_Animation(_uint iAnimIndex)
{
	switch (iAnimIndex)
	{
	case 14:
		m_bBlockAnim = false;
		m_bReady = false;
		break;
	default:
		m_pModel->Change_AnimIndex(iAnimIndex);
		break;
	}
}

void CChiedtian::Set_Dissolve_InOut(_bool bIsIn)
{
	if (bIsIn == true)
	{
		m_pDissolve->Set_DissolveOn(true, 0.2f);
	}
	else
	{
		m_pDissolve->Set_DissolveOn(false, 0.2f);
	}
}

void CChiedtian::Activate_SecondPage(_double fDeltaTime)
{

}

void CChiedtian::Update_Direction(_double fDeltaTime)
{
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();

	switch (m_pModel->Get_NowAnimIndex())
	{
	case 1:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0 >= m_fDelayTime)
		{
			m_pModel->Change_AnimIndex(10);
			m_bOnceSwitch = true;
		}
	}
		break;
	case 14:
	{
		if (true == m_bReady)
		{
			if (false == m_bOnceSwitch && 0.0379f <= fAnimPlayRate && 0.045f > fAnimPlayRate)
			{
				m_bOnceSwitch = true;
				m_bBlockAnim = true;
			}
		}
		else if (0.98f <= fAnimPlayRate)
		{
			m_pModel->Change_AnimIndex(1);
			m_fDelayTime = 3.f;
			m_bOnceSwitch = true;
		}
		else if (false == m_bOnceSwitch && 0.75f <= fAnimPlayRate)
		{
			_int iRandom = rand() % 3 + 1;

			wstring teampString;
			teampString = L"JJB_Chieftain_Intro100%_" + to_wstring(iRandom) + L".wav";

			g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
			m_bOnceSwitch = true;
		}
		else if (0.045f <= fAnimPlayRate && 0.75f > fAnimPlayRate)
		{
			if (true == m_bOnceSwitch)
			{
				m_bBlockAnim = true;

				m_fDelayTime -= (_float)fDeltaTime;
				if (m_fDelayTime <= 0.f)
				{
					m_fDelayTime = 0.f;
					m_bOnceSwitch = false;
					m_bBlockAnim = false;
				}
			}
			_float fCurDof = (m_pRendererCom->Get_DofLength() >= 150.f ? 150.f : m_pRendererCom->Get_DofLength() + 0.5f);
			m_pRendererCom->Set_DofLength(fCurDof);

			if (0.656f <= fAnimPlayRate)
			{
				m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				m_fAttachCamPos.y += 5.f;
				m_fAttachCamPos.z -= 10.f;
				GetSingle(CUtilityMgr)->Get_MainCamera()->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 0.f));
				GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->LookDir(_float3(0.f, 0.f, 1.f).XMVector());
				GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(1.17699897f, 45.2099724f, 312.646881f));
			}
			else if (0.506f <= fAnimPlayRate)
			{
				m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				m_fAttachCamPos.y += 8.5f;
				m_fAttachCamPos.z -= 3.5f;
				m_fAttachCamPos.x += 0.25f;
				GetSingle(CUtilityMgr)->Get_MainCamera()->Lock_CamLook(true, XMVectorSet(0.f, -0.5f, 0.5f, 0.f));
				GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->LookDir(_float3(0.f, -0.5f, 0.5f).XMVector());
				GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(1.42699993f, 48.7100258f, 319.146820f));

				if (0.512f <= fAnimPlayRate && 0.566f > fAnimPlayRate && true == m_bOncePlaySound)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
					m_bOncePlaySound = false;
					GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(57.f, 1.f, 0.2f, false);
				}
				else if (0.566f <= fAnimPlayRate && false == m_bOncePlaySound)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
					m_bOncePlaySound = true;
					GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(57.f, 1.f, 0.2f, false);
				}

			}
			else if (0.316f <= fAnimPlayRate)
			{
				m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				m_fAttachCamPos.y += 9.f;
				m_fAttachCamPos.z += 2.5f;
				GetSingle(CUtilityMgr)->Get_MainCamera()->Lock_CamLook(true, XMVectorSet(0.f, -0.6f, -0.5f, 0.f));
				GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->LookDir(_float3(0.f, -0.6f, -0.5f).XMVector());
				GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(1.17699897f, 49.2099838f, 325.147156f));

				if (0.36f <= fAnimPlayRate && 0.446f > fAnimPlayRate && true == m_bOncePlaySound)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
					m_bOncePlaySound = false;
					GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(57.f, 1.f, 0.2f, false);
				}
				else if (0.446f <= fAnimPlayRate && false == m_bOncePlaySound)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
					m_bOncePlaySound = true;
					GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(57.f, 1.f, 0.2f, false);
				}
			}
			else if (0.253f <= fAnimPlayRate)
			{
				m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				m_fAttachCamPos.y += 8.f;
				m_fAttachCamPos.z -= 3.f;
				m_fAttachCamPos.x += 3.f;
				GetSingle(CUtilityMgr)->Get_MainCamera()->Lock_CamLook(true, XMVectorSet(-0.5f, -0.5f, 0.5f, 0.f));
				GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->LookDir(_float3(-0.5f, -0.5f, 0.5f).XMVector());
				GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(4.17699862f, 48.2100296f, 319.647278f));

				if (0.275f <= fAnimPlayRate && false == m_bOncePlaySound)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
					m_bOncePlaySound = true;
					GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(57.f, 1.f, 0.2f, false);
				}
			}
			else if (0.05f <= fAnimPlayRate)
			{
				m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSING_CAMMOTIONBLUR, true);

				m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				m_fAttachCamPos.y += 8.f;
				m_fAttachCamPos.z -= 3.f;   
				m_fAttachCamPos.x -= 3.f; 
				GetSingle(CUtilityMgr)->Get_MainCamera()->Lock_CamLook(true, XMVectorSet(0.5f, -0.5f, 0.5f, 0.f));
				GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->LookDir(_float3(0.5f, -0.5f, 0.5f).XMVector());
				GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-1.82299995f, 48.2100296f, 319.647278f));

				if (0.094f <= fAnimPlayRate && 0.174f > fAnimPlayRate  && false == m_bOncePlaySound)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
					m_bOncePlaySound = true;
					GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(57.f, 1.f, 0.2f, false);
				}
				else if (0.174f <= fAnimPlayRate && true == m_bOncePlaySound)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
					m_bOncePlaySound = false;
					GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(57.f, 1.f, 0.2f, false);
				}
			}
		}
	}
		break;
	case 10:
	{
		if (0.98f <= fAnimPlayRate)
		{
			CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

			m_pModel->Change_AnimIndex(0);
			m_bBlockUpdate = false;
			pUtil->Get_MainCamera()->Set_FocusTarget(m_pPlayerObj);
			m_bIsMainWeaponOff = false;
			static_cast<CPlayer*>(m_pPlayerObj)->Set_State_StopActionEnd();
			static_cast<CPlayer*>(m_pPlayerObj)->Set_Targeting(this);
			m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSING_CAMMOTIONBLUR, false);



		}
		else if (true == m_bOnceSwitch && 0.341f <= fAnimPlayRate && 0.512f > fAnimPlayRate)
		{
			Set_Dissolve_InOut(false);
			m_bOnceSwitch = false;
			g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_SkullMace_Fire_Swing_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
		}
		else if (false == m_bOnceSwitch && 0.512f <= fAnimPlayRate)
		{
			Set_Dissolve_InOut(true);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(0.127f, 35.81f, 281.077f));

			m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			m_fAttachCamPos.y += 5.f;
			m_fAttachCamPos.z -= 20.f;
			GetSingle(CUtilityMgr)->Get_MainCamera()->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 0.f));
			GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->LookDir(_float3(0.f, 0.f, 1.f).XMVector());
			GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(0.127000064f, 40.8100204f, 261.077118f));
			m_bOnceSwitch = true;

		}

		if (true == m_bOncePlaySound && 0.6f <= fAnimPlayRate)
		{
			m_bOncePlaySound = false;
			g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Jump_Heavy.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);



			CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

			INSTPARTICLEDESC tParticleTexDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_23");
			tParticleTexDesc.vFixedPosition = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
			tParticleTexDesc.vPowerDirection = _float3(0, 0, 1);
			tParticleTexDesc.TargetColor = _float4(_float3(1.5), tParticleTexDesc.TargetColor.w);
			//tParticleTexDesc.vEmissive_SBB = _float3(0);
			tParticleTexDesc.TempBuffer_0.x = 0.4f;
			tParticleTexDesc.TempBuffer_1.w = 0.4f;
			tParticleTexDesc.ParticleSize = _float3(15.f);
			pUtil->Create_TextureInstance(m_eNowSceneNum, tParticleTexDesc);

		}
	}
		break;
	}
}

HRESULT CChiedtian::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_Chieftian), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(1));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.iCurrentIndex = 0;

	FAILED_CHECK(__super::Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Navigation), TAG_COM(Com_Navaigation), (CComponent**)&m_pNavigationCom, &NaviDesc));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(9.f, 9.f, 9.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_02", _float3(1), _float3(0), _float3(0.f, -0.04408f, -3.10981f));
	m_vecAttachedDesc.push_back(tAttachedDesc);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 1.4f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "head", _float3(1), _float3(0), _float3(0.f, 0.012524f, -4.42935f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 2.5f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "pelvis", _float3(1), _float3(0), _float3(0.f, 0.027885f, -2.54343f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();



	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, -0.1f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "head", _float3(1), _float3(0), _float3(0.f, 0.012524f, -4.42935f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, -0.1f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_01", _float3(1), _float3(0), _float3(0.f, -0.033538f, -2.83053f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "calf_r", _float3(1), _float3(0), _float3(-0.443741f, 0.047085f, -1.39788f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(2);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "calf_l", _float3(1), _float3(0), _float3(0.44374f, 0.047085f, -1.39788f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(2);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "foot_r", _float3(1), _float3(0), _float3(-0.612995f, 0.226194f, -0.264389f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(2);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "foot_l", _float3(1), _float3(0), _float3(0.612994f, 0.226194f, -0.264393f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(2);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "upperarm_r", _float3(1), _float3(0), _float3(-0.570974f, 0.137702f, -3.78514f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "upperarm_l", _float3(1), _float3(0), _float3(0.570976f, 0.137702f, -3.78514f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_r", _float3(1), _float3(0), _float3(-1.16276f, 0.060726f, -3.44879f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_l", _float3(1), _float3(0), _float3(1.16276f, 0.060726f, -3.44879f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_twist_01_r", _float3(1), _float3(0), _float3(-1.75266f, -0.14289f, -3.13727f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_twist_01_l", _float3(1), _float3(0), _float3(1.75266f, -0.14289f, -3.13727f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSub), (CComponent**)&m_pFireCollider));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(15.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, -4.f, 0.f, 1);
	FAILED_CHECK(m_pFireCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "root", _float3(1), _float3(0), _float3(0, -5.f, 0.f));
	m_FireAttachedDesc = tAttachedDesc;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSubSub), (CComponent**)&m_pJumpCollider));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pJumpCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "root", _float3(1), _float3(0), _float3(0, 0, 0));
	m_JumpAttachedDesc = tAttachedDesc;


	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	DissolveDesc.pShader = m_pShaderCom;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));

	return S_OK;
}

HRESULT CChiedtian::Ready_ParticleDesc()
{
	Safe_Release(m_pFireParticleTransformCom);
	m_pFireParticleTransformCom = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pFireParticleTransformCom, E_FAIL);

	{
		m_vecTexInstDesc.clear();
		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);
		//0
		{
			//INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_23");
			INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_12");
			m_vecTexInstDesc.push_back(tDesc);
		}
		//1
		{

			INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_9");
			//tDesc.eInstanceCount = Prototype_VIBuffer_Point_Instance_16;
			//tDesc.ParticleStartRandomPosMin = _float3(-1.f, 0, -1.f);
			//tDesc.ParticleStartRandomPosMax = _float3(1.f, 0.5f, 1.f);
			m_vecTexInstDesc.push_back(tDesc);


		}
		//2
		{

			INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_13");
			tDesc.FollowingTarget = m_pFireParticleTransformCom;
			tDesc.iFollowingDir = FollowingDir_Look;
			tDesc.TotalParticleTime = 10.f;
			tDesc.TargetColor.w = 10.f;
			tDesc.vEmissive_SBB = _float3(tDesc.vEmissive_SBB.x * 0.5f , 0,0);
			m_vecTexInstDesc.push_back(tDesc);


		}
		//3
		{

			INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_15");
			tDesc.FollowingTarget = nullptr;
			//tDesc.ePassID = InstancePass_AllDistortion_Bright; 
			tDesc.EachParticleLifeTime = 0.35f;
			m_vecTexInstDesc.push_back(tDesc);


		}
		//4
		{

			INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_16");
			tDesc.FollowingTarget = nullptr;
			tDesc.vEmissive_SBB = _float3(0);
			m_vecTexInstDesc.push_back(tDesc);


		}
		//5
		{

			INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_18");
			tDesc.FollowingTarget = m_pFireParticleTransformCom;
			tDesc.iFollowingDir = FollowingDir_Look;
			tDesc.TotalParticleTime = 10.f;
			//tDesc.TargetColor.w = 10.f;
			//tDesc.vEmissive_SBB = _float3(tDesc.vEmissive_SBB.x * 0.5f, 0, 0);
			m_vecTexInstDesc.push_back(tDesc);


		}
	}

	{
		m_vecMeshParticleDesc.clear();
		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);
		//5
		{
			INSTMESHDESC tDesc = pUtil->Get_MeshParticleDesc(L"JY_Mesh_1");
			tDesc.FollowingTarget = nullptr;
			tDesc.TotalParticleTime = 14.f;

			m_vecMeshParticleDesc.push_back(tDesc);
		}
	}

	{

		m_vecNonInstMeshDesc.clear();

		//0
		{
			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.vPosition = _float3(-0.073f, 35.900f, 281.488f);
			tNIMEDesc.vLookDir = _float3(1, 0, 0);


			tNIMEDesc.eMeshType = Prototype_Mesh_JYChiedFireLine1;

			tNIMEDesc.fMaxTime_Duration = 19.f;
			tNIMEDesc.fAppearTime = 1.f;

			tNIMEDesc.noisingdir = _float2(0.1f, 0);

			tNIMEDesc.NoiseTextureIndex = 200;
			tNIMEDesc.MaskTextureIndex = 63;
			tNIMEDesc.iDiffuseTextureIndex = 321;
			tNIMEDesc.m_iPassIndex = 23;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.98046875f, 0.19140625f, 0.19140625f, 1.f);
			tNIMEDesc.vColor = _float3(0.98046875f, 0.19140625f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.vSize = _float3(0.129f);

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}


		//1
		{
			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.vPosition = _float3(-0.073f, 35.900f, 281.488f);
			tNIMEDesc.vLookDir = _float3(1, 0, 0);


			tNIMEDesc.eMeshType = Prototype_Mesh_JYChiedFireLine2;

			tNIMEDesc.fMaxTime_Duration = 20.f;
			tNIMEDesc.fAppearTime = 0.75f;

			tNIMEDesc.noisingdir = _float2(1.f, 0);

			tNIMEDesc.NoiseTextureIndex = 200;
			tNIMEDesc.MaskTextureIndex = 63;
			tNIMEDesc.iDiffuseTextureIndex = 276;
			tNIMEDesc.m_iPassIndex = 17;

			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.98046875f, 0.19140625f, 0.19140625f, 1.f);
			tNIMEDesc.vColor = _float3(0.98046875f, 0.19140625f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			//tNIMEDesc.vSize = _float3(0.125f);
			//tNIMEDesc.vSize = _float3(0.162f, 0.162f, -0.162f);

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);

		}

		//2
		{
			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.vPosition = _float3(-0.073f, 30.900f, 281.488f);
			tNIMEDesc.vLookDir = _float3(1, 0, 0);


			tNIMEDesc.eMeshType = Prototype_Mesh_JYChiedFireLine3;

			tNIMEDesc.fMaxTime_Duration = 19.f;
			tNIMEDesc.fAppearTime = 1.5f;

			tNIMEDesc.noisingdir = _float2(1.f, 1.f).Get_Nomalize() * 0.07f;
			tNIMEDesc.fDistortionNoisingPushPower = 1.f;

			tNIMEDesc.NoiseTextureIndex = 200;
			tNIMEDesc.MaskTextureIndex = 63;
			tNIMEDesc.iDiffuseTextureIndex = 321;
			tNIMEDesc.m_iPassIndex = 23;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.98046875f, 0.19140625f, 0.19140625f, 1.f);
			tNIMEDesc.vColor = _float3(0.98046875f, 0.19140625f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.vSize = _float3(0.129f);

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}
		//3
		{
			NONINSTNESHEFTDESC tNIMEDesc;

			//tNIMEDesc.vPosition = _float3(-0.073f, 35.900f, 281.488f);
			//tNIMEDesc.vLookDir = _float3(1, 0, 0);


			//tNIMEDesc.eMeshType = Prototype_Mesh_JYOrnate1;

			//tNIMEDesc.fMaxTime_Duration = 0.5f;
			//tNIMEDesc.fAppearTime = 0.25f;

			//tNIMEDesc.noisingdir = _float2(1.f, -2.f).Get_Nomalize() * 0.5f;
			//tNIMEDesc.fDistortionNoisingPushPower = 3.f;
			//tNIMEDesc.NoiseTextureIndex = 200;
			//tNIMEDesc.MaskTextureIndex = 63;
			//tNIMEDesc.iDiffuseTextureIndex = 321;
			//tNIMEDesc.m_iPassIndex = 23;
			//tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			//tNIMEDesc.vLimLight = _float4(0.98046875f, 0.19140625f, 0.19140625f, 1.f);
			//tNIMEDesc.vColor = _float3(0.98046875f, 0.19140625f, 0.19140625f);

			//tNIMEDesc.RotAxis = FollowingDir_Look;
			//tNIMEDesc.RotationSpeedPerSec = 0.f;
			//tNIMEDesc.vSize = _float3(0.16125f, 0.16125f, 0.12135f);

			//tNIMEDesc.RotAxis = FollowingDir_Look;
			//tNIMEDesc.OnceStartRot = 45;

			//tNIMEDesc.MoveDir = FollowingDir_Look;


			tNIMEDesc.eMeshType = Prototype_Mesh_JYSwordEnergy;

			tNIMEDesc.fMaxTime_Duration = 0.5f;
			tNIMEDesc.fAppearTime = 0.25f;

			tNIMEDesc.noisingdir = _float2(1.f, 0).Get_Nomalize() * 0.5f;
			tNIMEDesc.fDistortionNoisingPushPower = 3.f;
			tNIMEDesc.NoiseTextureIndex = 200;
			tNIMEDesc.MaskTextureIndex = 63;
			tNIMEDesc.iDiffuseTextureIndex = 321;
			tNIMEDesc.m_iPassIndex = 23;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.98046875f, 0.19140625f, 0.19140625f, 1.f);
			tNIMEDesc.vColor = _float3(0.98046875f, 0.19140625f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.vSize = _float3(0.258f, 0.258f, 0.258f);

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);

		}
		//4
		{
			NONINSTNESHEFTDESC tNIMEDesc;

			//tNIMEDesc.vPosition = _float3(-0.073f, 35.900f, 281.488f);
			//tNIMEDesc.vLookDir = _float3(1, 0, 0);


			tNIMEDesc.eMeshType = Prototype_Mesh_JYSwordEnergy;

			tNIMEDesc.fMaxTime_Duration = 0.5f;
			tNIMEDesc.fAppearTime = 0.25f;

			tNIMEDesc.noisingdir = _float2(-1.f, 0).Get_Nomalize() * 0.5f;
			tNIMEDesc.fDistortionNoisingPushPower = 3.f;
			tNIMEDesc.NoiseTextureIndex = 200;
			tNIMEDesc.MaskTextureIndex = 63;
			tNIMEDesc.iDiffuseTextureIndex = 321;
			tNIMEDesc.m_iPassIndex = 23;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.98046875f, 0.19140625f, 0.19140625f, 1.f);
			tNIMEDesc.vColor = _float3(0.98046875f, 0.19140625f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.vSize = _float3(0.258f, 0.258f, 0.258f);


			m_vecNonInstMeshDesc.push_back(tNIMEDesc);

		}
		//5
		{
			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.vPosition = _float3(-0.073f, 35.900f, 281.488f);
			tNIMEDesc.vLookDir = _float3(1, 0, 0);


			tNIMEDesc.eMeshType = Prototype_Mesh_JYBall;


			tNIMEDesc.fMaxTime_Duration = 0.5f;
			tNIMEDesc.fAppearTime = 0.01f;

			tNIMEDesc.noisingdir = _float2(1.f, 0.f).Get_Nomalize() * 0.03f;
			tNIMEDesc.fDistortionNoisingPushPower = 3.f;
			tNIMEDesc.NoiseTextureIndex = 200;
			tNIMEDesc.MaskTextureIndex = 63;
			tNIMEDesc.iDiffuseTextureIndex = 321;
			tNIMEDesc.m_iPassIndex = 23;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.98046875f, 0.19140625f, 0.19140625f, 1.f);
			tNIMEDesc.vColor = _float3(0.98046875f, 0.19140625f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Up;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.vSize = _float3(0.1f, 0.1f, 0.1f);

			tNIMEDesc.SizeSpeed = 15.f;
			tNIMEDesc.vSizingRUL = _float3(1, 1, 1);

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);

			//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
			//	TAG_OP(Prototype_NonInstanceMeshEffect), &tNIMEDesc);
		}
		//6
		{

			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.vPosition = _float3(-0.073f, 33.900f, 281.488f);
			tNIMEDesc.vLookDir = _float3(0, 0, 1);


			tNIMEDesc.eMeshType = Prototype_Mesh_Tornado2;
			tNIMEDesc.vLookDir = _float3(1, 0, 0);
			tNIMEDesc.fAppearTime = 0.25f;
			tNIMEDesc.fMaxTime_Duration = tNIMEDesc.fAppearTime*2.f;

			tNIMEDesc.noisingdir = _float2(1, 0);

			tNIMEDesc.fDistortionNoisingPushPower = 20.f;
			//m_tSubFlameDesc.NoiseTextureIndex = 381;
			tNIMEDesc.NoiseTextureIndex = 6;
			tNIMEDesc.MaskTextureIndex = 81;
			tNIMEDesc.iDiffuseTextureIndex = 299;
			tNIMEDesc.m_iPassIndex = 19;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			//tNIMEDesc.vLimLight = _float4(0.35f, 0.85f, 0.35f, 0.1f);
			tNIMEDesc.vColor = _float4(1, 1, 1, 1);

			tNIMEDesc.RotAxis = FollowingDir_Up;
			tNIMEDesc.RotationSpeedPerSec = 1080.f;

			tNIMEDesc.SizeSpeed = 1.5f;
			tNIMEDesc.vSizingRUL = _float3(1, 1, 1);
			tNIMEDesc.vSize = _float3(0.5f, 0.6667f, 0.5f);
			//m_tSubFlameDesc.vLimLight = _float4(_float3(vOldRimLightColor), 1);

			tNIMEDesc.MoveDir = FollowingDir_Look;
			tNIMEDesc.MoveSpeed = 0.f;
			tNIMEDesc.fAlphaTestValue = 0.8f;

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);

		}

		//7
		{

			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.vPosition = _float3(-0.073f, 35.900f, 281.488f);
			tNIMEDesc.vLookDir = _float3(0, 1, 0);


			tNIMEDesc.eMeshType = Prototype_Mesh_JYBall;

			tNIMEDesc.fMaxTime_Duration = 0.5f;
			tNIMEDesc.fAppearTime = 0.25f;

			tNIMEDesc.noisingdir = _float2(0, -1);

			tNIMEDesc.NoiseTextureIndex = 200;
			tNIMEDesc.MaskTextureIndex = 69;
			tNIMEDesc.iDiffuseTextureIndex = 299;
			tNIMEDesc.m_iPassIndex = 19;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(1.f, 0.f, 0.0f, 1.f);
			tNIMEDesc.vColor = _float3(0.5f, 0.19140625f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.StartRot = GetSingle(CUtilityMgr)->RandomFloat(0, 360);
			tNIMEDesc.RotationSpeedPerSec = 360.f;

			tNIMEDesc.SizeSpeed = 7.8f;
			tNIMEDesc.vSizingRUL = _float3(1, 1, 0);
			tNIMEDesc.vSize = _float3(0.1f, 0.1f, 0.00001f);


			m_vecNonInstMeshDesc.push_back(tNIMEDesc);

		}
		//8
		{

			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.vPosition = _float3(-0.073f, 35.900f, 281.488f);
			tNIMEDesc.vLookDir = _float3(0, 1, 0);


			tNIMEDesc.eMeshType = Prototype_Mesh_JYBall;

			tNIMEDesc.fMaxTime_Duration = 0.5f;
			tNIMEDesc.fAppearTime = 0.25f;

			tNIMEDesc.noisingdir = _float2(0, -1);

			tNIMEDesc.NoiseTextureIndex = 200;
			tNIMEDesc.MaskTextureIndex = 69;
			tNIMEDesc.iDiffuseTextureIndex = 299;
			tNIMEDesc.m_iPassIndex = 16;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(1.f, 0.f, 0.0f, 1.f);
			tNIMEDesc.vColor = _float3(0.5f, 0.19140625f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.StartRot = GetSingle(CUtilityMgr)->RandomFloat(0, 360);
			tNIMEDesc.RotationSpeedPerSec = 360.f;

			tNIMEDesc.SizeSpeed = 4.5f;
			tNIMEDesc.vSizingRUL = _float3(1, 1, 1);
			tNIMEDesc.vSize = _float3(0.1f, 0.1f, 0.1f);

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}
		//9
		{

			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.vPosition = _float3(-0.073f, 35.900f, 281.488f);
			tNIMEDesc.vLookDir = _float3(1, 0, 0);

			tNIMEDesc.eMeshType = Prototype_Mesh_JY_HalfRing;
			tNIMEDesc.fAppearTime = 0.35f;
			tNIMEDesc.fMaxTime_Duration = tNIMEDesc.fAppearTime*2.f;

			tNIMEDesc.noisingdir = _float2(2, 0);

			tNIMEDesc.fDistortionNoisingPushPower = 20.f;
			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.MaskTextureIndex = 46;
			tNIMEDesc.iDiffuseTextureIndex = 299;
			tNIMEDesc.m_iPassIndex = 19;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.05f, 0.15f, 0.05f, 0);
			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.vColor = _float4(1, 1, 1, 1);

			tNIMEDesc.RotAxis = FollowingDir_Up;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.vSize = _float3(0.2f, 0.2f, 0.2f);
			//m_tVolcanoDesc.vLimLight = _float4(_float3(vOldRimLightColor), 1);

			tNIMEDesc.MoveDir = FollowingDir_Look;
			tNIMEDesc.MoveSpeed = 0.f;

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}
	}

	return S_OK;
}

HRESULT CChiedtian::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate >= 0.98 )
		m_iAdjMovedIndex = 0;

	if (iNowAnimIndex == m_iOldAnimIndex  && m_OldPlayRate >  PlayRate)
		m_iAdjMovedIndex = 0;


	if (PlayRate <= 0.98)
	{
		switch (iNowAnimIndex)
		{
		case 0:
		{
			static _double ParticleTimer = 0;
			ParticleTimer += fDeltatime;
			if (ParticleTimer > 0.15f)
			{
				ParticleTimer = 0;

				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);
				m_vecNonInstMeshDesc[9].vPosition = _float3(-0.073f, 33.900f, 281.488f).XMVector() + XMVectorSetY(pUtil->RandomFloat3(-20, 20).XMVector(), 0);
				m_vecNonInstMeshDesc[9].vLookDir = XMVector3Normalize(XMVectorSetY(pUtil->RandomFloat3(-1, 1).XMVector(), 0));
				m_vecNonInstMeshDesc[9].vSize = _float3(pUtil->RandomFloat(0.2f, 0.6f));


				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[9]);

			}

			CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
			if (PlayRate > 0 && m_iAdjMovedIndex == 0)
			{
				m_bIsVolcanoAttack = true;
				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.14893617 && m_iAdjMovedIndex == 1)
			{
				m_pSecondPageWeapons[0]->Set_WeaponPosition();
				m_pSecondPageWeapons[0]->Set_PlayerPos(PlayerTransform->Get_MatrixState(CTransform::STATE_POS));
				m_pSecondPageWeapons[0]->Set_InitialPosDessolve(false);
				m_pSecondPageWeapons[0]->Set_VolcanoAttack(true);
				m_pSecondPageWeapons[0]->Set_IsAttackState(true);
				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.2978723404 && m_iAdjMovedIndex == 2)
			{
				m_pSecondPageWeapons[1]->Set_WeaponPosition();
				m_pSecondPageWeapons[1]->Set_PlayerPos(PlayerTransform->Get_MatrixState(CTransform::STATE_POS));
				m_pSecondPageWeapons[1]->Set_InitialPosDessolve(false);
				m_pSecondPageWeapons[1]->Set_VolcanoAttack(true);
				m_pSecondPageWeapons[1]->Set_IsAttackState(true);
				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.595744680 && m_iAdjMovedIndex == 3)
			{
				m_pSecondPageWeapons[2]->Set_WeaponPosition();
				m_pSecondPageWeapons[2]->Set_PlayerPos(PlayerTransform->Get_MatrixState(CTransform::STATE_POS));
				m_pSecondPageWeapons[2]->Set_InitialPosDessolve(false);
				m_pSecondPageWeapons[2]->Set_VolcanoAttack(true);
				m_pSecondPageWeapons[2]->Set_IsAttackState(true);
				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.85106382 && m_iAdjMovedIndex == 4)
			{
				m_pSecondPageWeapons[3]->Set_WeaponPosition();
				m_pSecondPageWeapons[3]->Set_PlayerPos(PlayerTransform->Get_MatrixState(CTransform::STATE_POS));
				m_pSecondPageWeapons[3]->Set_InitialPosDessolve(false);
				m_pSecondPageWeapons[3]->Set_VolcanoAttack(true);
				m_pSecondPageWeapons[3]->Set_IsAttackState(true);
				++m_iAdjMovedIndex;
			}
		}
		break;

		case 1://애니메이션 인덱스마다 잡아주면 됨
		{
			m_bIsAtaackAimEnd = true;
			if (PlayRate > 0.f)
			{
				m_bIsAttack = false;
				m_bISkill = false;
				m_bIsLookAt = true;
				//m_bIsBackJump = false;

			}
		}
		break;
		case 2:
		{
			m_bIsAtaackAimEnd = false;
			m_pTransformCom->Move_Forward(fDeltatime);
			
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.1612903225)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Footstep_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 1 && PlayRate > 0.5483870967)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Footstep_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}
		}
			break;

		case 3:
		{

		}
			break;

		case 4:
		{

		}

		break;

		case 5:
		{
			m_bIsAtaackAimEnd = false;

			if (!m_ActivateSecondPage)
			{
				if (m_iAdjMovedIndex == 0 && PlayRate > 0.1960784313)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Himeko_Attack_2.mp3", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
					m_iAdjMovedIndex++;
				}
				else if (m_iAdjMovedIndex == 1 && PlayRate < 0.254901960784313f)
				{
					{

						CUtilityMgr*	pUtil = GetSingle(CUtilityMgr);
						INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_17");

						tDesc.FollowingTarget = nullptr;

						m_vecNonInstMeshDesc[6].SizeSpeed = 1.5f;
						m_vecNonInstMeshDesc[7].SizeSpeed = 7.8f;
						m_vecNonInstMeshDesc[8].SizeSpeed = 4.5f;
					
						tDesc.vFixedPosition = m_vecNonInstMeshDesc[6].vPosition = m_vecNonInstMeshDesc[7].vPosition =
							m_vecNonInstMeshDesc[8].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
							XMVector3Normalize(	m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 10.f +
							m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * -3.5f) * 11.f;

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[6]);
						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[7]);
						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[8]);

						CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
						CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));
						 
						CParticleCollider::SETTINGCOLLIDER ColliderDesc;
						ColliderDesc.ColliderType = COLLIDER_SPHERE;
						ColliderDesc.ColliderDesc.vScale = _float3(8.5f, 8.5f, 8.5f);
						ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
						ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
						ColliderDesc.pTargetTransform = Transform;

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
							TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);

						
						pUtil->Create_TextureInstance(m_eNowSceneNum, tDesc);
					}


					m_iAdjMovedIndex++;
				}
				if (m_iAdjMovedIndex == 2 && PlayRate < 0.2926829268)
				{

					m_bIsLookAt = false;
					m_iAdjMovedIndex++;
				}
				else if (m_iAdjMovedIndex == 3 && PlayRate > 0.3921568623921565)
				{
					{

						CUtilityMgr*	pUtil = GetSingle(CUtilityMgr);
						INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_17");

						tDesc.FollowingTarget = nullptr;

						m_vecNonInstMeshDesc[6].SizeSpeed = 1.5f;
						m_vecNonInstMeshDesc[7].SizeSpeed = 7.8f;
						m_vecNonInstMeshDesc[8].SizeSpeed = 4.5f;

						tDesc.vFixedPosition = m_vecNonInstMeshDesc[6].vPosition = m_vecNonInstMeshDesc[7].vPosition =
							m_vecNonInstMeshDesc[8].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
							XMVector3Normalize(m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 10.f +
								m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * -3.5f) * 15.f;

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[6]);
						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[7]);
						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[8]);


						CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
						CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

						CParticleCollider::SETTINGCOLLIDER ColliderDesc;
						ColliderDesc.ColliderType = COLLIDER_SPHERE;
						ColliderDesc.ColliderDesc.vScale = _float3(8.5f, 8.5f, 8.5f);
						ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
						ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
						ColliderDesc.pTargetTransform = Transform;

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
							TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);


						pUtil->Create_TextureInstance(m_eNowSceneNum, tDesc);
					}

					m_iAdjMovedIndex++;

				}
				else if (m_iAdjMovedIndex == 4 && PlayRate > 0.529411764)
				{
					{

						CUtilityMgr*	pUtil = GetSingle(CUtilityMgr);
						INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_17");

						tDesc.FollowingTarget = nullptr;

						m_vecNonInstMeshDesc[6].SizeSpeed = 1.5f;
						m_vecNonInstMeshDesc[7].SizeSpeed = 7.8f;
						m_vecNonInstMeshDesc[8].SizeSpeed = 4.5f;

						tDesc.vFixedPosition = m_vecNonInstMeshDesc[6].vPosition = m_vecNonInstMeshDesc[7].vPosition =
							m_vecNonInstMeshDesc[8].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
							XMVector3Normalize(m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 10.f +
								m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * -3.5f) * 19.f;

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[6]);
						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[7]);
						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[8]);



						CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
						CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

						CParticleCollider::SETTINGCOLLIDER ColliderDesc;
						ColliderDesc.ColliderType = COLLIDER_SPHERE;
						ColliderDesc.ColliderDesc.vScale = _float3(8.5f, 8.5f, 8.5f);
						ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
						ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
						ColliderDesc.pTargetTransform = Transform;

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
							TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);


						pUtil->Create_TextureInstance(m_eNowSceneNum, tDesc);
					}

					m_pMainWeapons[0]->Set_IsAttackState(false);
					m_iAdjMovedIndex++;
				}


				if (PlayRate > 0.215686274 && PlayRate < 0.529411764)
				{
					m_pMainWeapons[0]->Set_IsAttackState(true);
				}
			}
			else
			{
				if (m_iAdjMovedIndex == 0 && PlayRate > 0.19607843137)
				{
					CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
					_float3 m_vPlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

					for (auto& SecondPageWeapon : m_pSecondPageWeapons)
					{
						_float RandomPosX = GetSingle(CUtilityMgr)->RandomFloat(-2.3f, 2.3f);
						_float RandomPosZ = GetSingle(CUtilityMgr)->RandomFloat(-1.3f, 1.3f);

						m_vPlayerPos.x += RandomPosX;
						m_vPlayerPos.z += RandomPosZ;

						SecondPageWeapon->Set_PlayerPos(m_vPlayerPos);
						SecondPageWeapon->Set_IsAttackState(true);
						SecondPageWeapon->Set_AttackfinishOff(false);
						SecondPageWeapon->Set_WeaponPosition();
					}
					m_iAdjMovedIndex++;
				}
				if (m_iAdjMovedIndex == 1 && PlayRate < 0.2926829268)
				{
					m_bIsLookAt = false;
					m_iAdjMovedIndex++;
				}
			}


			//if (PlayRate < 0.1372549 && PlayRate > 0.3725490)
			//{
			//	//무기 회전 시킬려고 한거 같은데???
			//	//CChiedtuan_Weapon* Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Weapon"), 0);
			//}
		}

		break;

		case 6:
		{
			m_bIsAtaackAimEnd = false;
			


			//m_pMainWeapons[0]->SwordTrailOnOff(true);
			//m_pMainWeapons[1]->SwordTrailOnOff(true);
			//m_pMainWeapons[0]->SwordTrailOnOff(false);
			//m_pMainWeapons[1]->SwordTrailOnOff(false);

			if (m_iAdjMovedIndex == 0 && PlayRate > 0.12)
			{
				m_vecNonInstMeshDesc[3].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
					m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * -1.5f  +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * 3.5f +
					XMVectorSet(0,2.5f,0,0);
				m_vecNonInstMeshDesc[3].vLookDir = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

				m_vecNonInstMeshDesc[3].OnceStartRot = 320.f;
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[3]);


				m_pMainWeapons[1]->SwordTrailOnOff(true);
				++m_iAdjMovedIndex;
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate > 0.16)
			{


				g_pGameInstance->Play3D_Sound(L"JJB_Himeko_Attack_2.mp3", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 2 && PlayRate >	0.1866666666)
			{

				CUtilityMgr*	pUtil = GetSingle(CUtilityMgr);
				INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_17");

				tDesc.FollowingTarget = nullptr;


				m_vecNonInstMeshDesc[6].SizeSpeed = 1.8f;
				m_vecNonInstMeshDesc[7].SizeSpeed = 9.36f;
				m_vecNonInstMeshDesc[8].SizeSpeed = 5.4f;

				tDesc.vFixedPosition = m_vecNonInstMeshDesc[6].vPosition = m_vecNonInstMeshDesc[7].vPosition =
					m_vecNonInstMeshDesc[8].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 8.f +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * 6.5f;

				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[6]);
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[7]);
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[8]);


				CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
				CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

				CParticleCollider::SETTINGCOLLIDER ColliderDesc;
				ColliderDesc.ColliderType = COLLIDER_SPHERE;
				ColliderDesc.ColliderDesc.vScale = _float3(8.5f, 8.5f, 8.5f);
				ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
				ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
				ColliderDesc.pTargetTransform = Transform;

				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
					TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);

				pUtil->Create_TextureInstance(m_eNowSceneNum, tDesc);


				m_pMainWeapons[1]->SwordTrailOnOff(false);
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 3 && PlayRate > 0.28)
			{
				m_vecNonInstMeshDesc[4].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * -2.f + XMVectorSet(0, 6, 0, 0) +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 2.f;

				m_vecNonInstMeshDesc[4].vLookDir = XMVector3Normalize(-m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) + 
										m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK))
					;
				m_vecNonInstMeshDesc[4].OnceStartRot = -10.f;
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);

				m_pMainWeapons[0]->SwordTrailOnOff(true);
				m_pMainWeapons[1]->SwordTrailOnOff(true);
				++m_iAdjMovedIndex;
			}			
			if (m_iAdjMovedIndex == 4 && PlayRate > 0.32)
			{

				
				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 5 && PlayRate > 0.4)
			{
				m_pMainWeapons[0]->SwordTrailOnOff(false);
				m_pMainWeapons[1]->SwordTrailOnOff(false);
				++m_iAdjMovedIndex;
			}
			
			if (m_iAdjMovedIndex == 6 && PlayRate > 0.4533333)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Footstep_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 7 && PlayRate > 0.50666666666666)
			{
				m_pMainWeapons[0]->SwordTrailOnOff(true);
				++m_iAdjMovedIndex;
			}
			

			if (m_iAdjMovedIndex == 8 && PlayRate > 0.0526315)
			{




				m_vecNonInstMeshDesc[3].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
					m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * -1.5f +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * -2.f;
				m_vecNonInstMeshDesc[3].vLookDir = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

				m_vecNonInstMeshDesc[3].OnceStartRot = 304.5f;

				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[3]);

				for (_int i = 0; i < m_pMainWeapons.size(); ++i)
				{
					m_pMainWeapons[i]->Set_IsAttackState(true);
				}
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 9 && PlayRate > 0.5466)
			{
				CUtilityMgr*	pUtil = GetSingle(CUtilityMgr);
				INSTPARTICLEDESC tDesc = pUtil->Get_TextureParticleDesc(L"JY_TextureEft_17");

				tDesc.FollowingTarget = nullptr;


				m_vecNonInstMeshDesc[6].SizeSpeed = 1.8f;
				m_vecNonInstMeshDesc[7].SizeSpeed = 9.36f;
				m_vecNonInstMeshDesc[8].SizeSpeed = 5.4f;

				tDesc.vFixedPosition = m_vecNonInstMeshDesc[6].vPosition = m_vecNonInstMeshDesc[7].vPosition =
					m_vecNonInstMeshDesc[8].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 10.f +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * -1.0f;

				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[6]);
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[7]);
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[8]);

				CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
				CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

				CParticleCollider::SETTINGCOLLIDER ColliderDesc;
				ColliderDesc.ColliderType = COLLIDER_SPHERE;
				ColliderDesc.ColliderDesc.vScale = _float3(8.5f, 8.5f, 8.5f);
				ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
				ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
				ColliderDesc.pTargetTransform = Transform;

				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
					TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);

				pUtil->Create_TextureInstance(m_eNowSceneNum, tDesc);


				g_pGameInstance->Play3D_Sound(L"JJB_Himeko_Attack_2.mp3", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 10 && PlayRate > 0.573333333333333)
			{
				m_pMainWeapons[0]->SwordTrailOnOff(false);
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 11 && PlayRate > 0.7733333333333333333)
			{
				m_vecNonInstMeshDesc[4].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT) * 2.f + XMVectorSet(0, 4, 0, 0);

				m_vecNonInstMeshDesc[4].vLookDir = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) +
					m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK))
					;
				m_vecNonInstMeshDesc[4].OnceStartRot = 185.f;
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);


				m_pMainWeapons[0]->SwordTrailOnOff(true);
				++m_iAdjMovedIndex;
			}
			
			if (m_iAdjMovedIndex == 12 && PlayRate > 0.92)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Footstep_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.92 && m_iAdjMovedIndex == 12)
			{
				for (_int i = 0; i < m_pMainWeapons.size(); ++i)
				{
					m_pMainWeapons[i]->Set_IsAttackState(false);
				}
				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.266666666 && PlayRate <= 0.34666666)
			{
				m_pTransformCom->Move_Forward(fDeltatime, m_pNavigationCom);
			}
			else if (PlayRate > 0.68 && PlayRate < 0.826666666)
			{
				m_pTransformCom->Move_Forward(fDeltatime, m_pNavigationCom);
			}

			if (PlayRate > 0 && PlayRate < 0.92)
			{
				_float Value = max(min(g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 2.f, (_float)PlayRate , 0.92f), 1.f), 0) * 0.4f;
				m_pMainWeapons[1]->Set_LimLight_N_Emissive(_float4(0.60546875f, 0.06640625f, 0.1171875f, 1.f).XMVector() * Value, 
					_float4(1.f, 0.5f, 1.f, 0.f).XMVector() *Value);
				m_pMainWeapons[0]->Set_LimLight_N_Emissive(_float4(0.60546875f, 0.06640625f, 0.1171875f, 1.f).XMVector() * Value,
					_float4(1.f, 0.5f, 1.f, 0.f).XMVector() *Value);
			}
		}
			break;

		case 7://SpinAttackStartAnim
		{
			m_bIsLookAt = false;

			if (PlayRate > 0 && PlayRate < 0.92)
			{
				_float Value = max(min(g_pGameInstance->Easing(TYPE_Linear, 0, 2.f, (_float)PlayRate, 0.92f), 1.f), 0) * 0.4f;
				m_pMainWeapons[1]->Set_LimLight_N_Emissive(_float4(0.60546875f, 0.06640625f, 0.1171875f, 1.f).XMVector() * Value,
					_float4(1.f, 0.5f, 1.f, 0.f).XMVector() *Value);
				m_pMainWeapons[0]->Set_LimLight_N_Emissive(_float4(0.60546875f, 0.06640625f, 0.1171875f, 1.f).XMVector() * Value,
					_float4(1.f, 0.5f, 1.f, 0.f).XMVector() *Value);

				m_pSubWeapons[1]->Set_LimLight_N_Emissive(_float4(0.60546875f, 0.06640625f, 0.1171875f, 1.f).XMVector() * Value,
					_float4(1.f, 0.5f, 1.f, 0.f).XMVector() *Value);
				m_pSubWeapons[0]->Set_LimLight_N_Emissive(_float4(0.60546875f, 0.06640625f, 0.1171875f, 1.f).XMVector() * Value,
					_float4(1.f, 0.5f, 1.f, 0.f).XMVector() *Value);
			}

			if (!m_ActivateSecondPage)
			{
				m_bIsAtaackAimEnd = false;

				if (m_iAdjMovedIndex == 0 && PlayRate > 0.0526315)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_SwordSound.mp3", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

					CChiedtuan_Weapon::WEAPOPNDESC WeaponDesc;

					//CChiedtuan_Weapon* Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Weapon"), 2);
					CChiedtuan_Weapon* Weapon = m_pSubWeapons[0];
					WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_02_r_02", XMVectorSet(9.f, 9.f, 9.f, 0.f), XMVectorSet(90.f, 65.f, 90.f, 0.f), XMVectorSet(-2.580f * 9.f, -1.870f * 9.f, -3.470f * 9.f, 1.f));
					WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BR;
					Weapon->Set_WeaponDesc(WeaponDesc);
					Weapon->Set_SpinScal();

					//Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Weapon"), 3);
					Weapon = m_pSubWeapons[1];
					WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_02_l_02", XMVectorSet(10.f, 10.f, 10.f, 0.f), XMVectorSet(50.f, 90.f, 0.f, 0.f), XMVectorSet(2.230f * 10.f, -1.990f* 10.f, -3.500f* 10.f, 1.f));
					WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BL;
					Weapon->Set_WeaponDesc(WeaponDesc);
					Weapon->Set_SpinScal();


					++m_iAdjMovedIndex;
				}
				if (m_iAdjMovedIndex == 1 && PlayRate > 0.9)
				{
					for (auto& Weapon : m_pMainWeapons) Weapon->SwordTrailOnOff(true);
					for (auto& Weapon : m_pSubWeapons) Weapon->SwordTrailOnOff(true);
					++m_iAdjMovedIndex;
				}

			}
			else
			{

				if (m_iAdjMovedIndex == 0 && PlayRate > 0.0526315)
				{

					m_vecNonInstMeshDesc[5].fMaxTime_Duration = 0.5f;
					m_vecNonInstMeshDesc[5].vSize = _float3(0.1f, 0.1f, 0.1f);
					m_vecNonInstMeshDesc[5].SizeSpeed = 15.f;
					m_vecNonInstMeshDesc[5].vSizingRUL = _float3(1, 1, 1);
					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[5]);

					m_vecNonInstMeshDesc[1].vSize = _float3(0.125f);
					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[1]);
					m_vecNonInstMeshDesc[1].vSize = _float3(0.162f, 0.162f, -0.162f);
					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[1]);

					CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Boss));
					CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

					CParticle_ColliderInOut::SETTINGCOLLIDERINOUT ColliderDesc;
					ColliderDesc.ColliderType = COLLIDER_SPHERE;
					ColliderDesc.ColliderDesc.vScale = _float3(60.f, 60.f, 60.f);
					ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
					ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
					ColliderDesc.pTargetTransform = Transform;

					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleColliderInOut),
						TAG_OP(Prototype_Object_ParticleColliderInOut), &ColliderDesc);

					++m_iAdjMovedIndex;
				}
				else if (m_iAdjMovedIndex == 1 && PlayRate > 0.1263157894736842)
				{
					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[0]);

					++m_iAdjMovedIndex;
				}
				else if (m_iAdjMovedIndex == 2 && PlayRate > 0.18947368421052631)
				{
					//m_vecNonInstMeshDesc[5].fMaxTime_Duration = 19.f;
					//m_vecNonInstMeshDesc[5].vSize = _float3(40.f);
					//m_vecNonInstMeshDesc[5].SizeSpeed = 0.f;
					//m_vecNonInstMeshDesc[5].vSizingRUL = _float3(0, 0, 0);
					//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
					//	TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[5]);
					++m_iAdjMovedIndex;

				}
				else if (m_iAdjMovedIndex == 3 && PlayRate > 0.25263157894736842)
				{

					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[2]);
					++m_iAdjMovedIndex;
				}
	
			}
		}

		break;

		case 8:
		{
			m_bIsAtaackAimEnd = false;

			if (m_ActivateSecondPage)
			{
				if (m_iAdjMovedIndex == 0)
				{
					for (auto& SecondPageWeapon : m_pSecondPageWeapons)
					{
						SecondPageWeapon->Set_Dissolve(false);
						//SecondPageWeapon->BeginningPos();
						
					}
					++m_iAdjMovedIndex;
				}
			}
		}
		break;

		case 9://SpinAttackAnim
		{
			m_bIsAtaackAimEnd = false;

			if (!m_ActivateSecondPage)
			{
				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				PlayerPos.y = MonsterPos.y;

				_float3 vGoalDir = (PlayerPos.XMVector() - MonsterPos.XMVector());
				//m_pTransformCom->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * m_fSpinSpeed + 0.01f);

				if (m_fSpinSpeed > 0.1f)
					m_pTransformCom->MovetoDir(XMLoadFloat3(&vGoalDir), fDeltatime * 0.3, m_pNavigationCom);

				//m_fAnimmultiple = (m_fSpinSpeed *0.1f);

				if (m_iAdjMovedIndex == 0)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Chief_Tornedo_Wind.mp3", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
					m_iAdjMovedIndex++;
				}
			}
			else
			{
				//if (PlayRate > 0 && PlayRate < 0.3)
				//{
				//	_float tt = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 0, m_fSpinSpeed*0.25f, PlayRate, 0.3f);
				//	_float t2 = g_pGameInstance->Easing(TYPE_SinInOut, -0.5f, 0.5f, PlayRate, 0.3f);

				//	m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * tt + 0.01f);
				//	m_pTransformCom->Move_Forward(fDeltatime * t2);
				//}
				//else if (PlayRate > 0.433333333 && PlayRate < 0.53333333)
				//{
				//	_float tt = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 0, m_fSpinSpeed *0.25f ,
				//		PlayRate - 0.433333333f, 0.1f);

				//	_float t2 = g_pGameInstance->Easing(TYPE_SinInOut, -0.5f, 0.5f,
				//		PlayRate - 0.433333333f, 0.1f);

				//	m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * tt + 0.01f);
				//	m_pTransformCom->Move_Forward(fDeltatime * t2);
				//}
				////m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * (m_fSpinSpeed*0.6f) + 0.01f);
				//////m_fAnimmultiple = (m_fSpinSpeed *0.1f);
			}
		}

		break;

		case 10://Jump Animation
		{
			m_bIsAtaackAimEnd = false;

			if (!m_ActivateSecondPage)
			{


				if (m_iAdjMovedIndex == 0 && PlayRate < 0.2926829268)
				{
					m_bIsLookAt = false;
					m_iAdjMovedIndex++;
				}

				if (m_iAdjMovedIndex == 1 && PlayRate > 0.34146341)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Attack_Grunt_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

					m_vecNonInstMeshDesc[7].vPosition =m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);


					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[7]);

					m_iAdjMovedIndex++;
				}
				if (PlayRate > 0.317073170 && PlayRate < 0.63414634146)
				{
					m_bIsLookAt = true;

					_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

					CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
					_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

					PlayerPos.y = MonsterPos.y;

					_float3 vGoalDir = (PlayerPos.XMVector() - MonsterPos.XMVector());

					_float fLength = g_pGameInstance->Easing(TYPE_CircularIn, 0.f, vGoalDir.Get_Lenth(), (_float)PlayRate - 0.317073170f, 0.31707317146f);

					m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
				}
				if (m_iAdjMovedIndex == 2 && PlayRate > 0.63414634146)
				{

					m_vecTexInstDesc[3].vFixedPosition = m_vecTexInstDesc[4].vFixedPosition =
						m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
					m_vecTexInstDesc[4].vFixedPosition.y = 35.9f;
					GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTexInstDesc[3]);
					GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTexInstDesc[4]);

					

					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Jump_Heavy.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
					m_bIsLookAt = false;
					m_iAdjMovedIndex++;
				}

				if (m_iAdjMovedIndex == 3 && PlayRate > 0.682926)
				{
					m_bIsJump = true;
					m_iAdjMovedIndex++;
				}
				if (m_iAdjMovedIndex == 4 && PlayRate > 0.71)
				{
					m_bIsJump = false;
					m_iAdjMovedIndex++;
				}
			}
			////////////////////////////
			else
			{
				if (m_iAdjMovedIndex == 0 && PlayRate < 0.2926829268)
				{
					m_bIsLookAt = false;
					m_iAdjMovedIndex++;
				}
				if (m_iAdjMovedIndex == 1 && PlayRate > 0.34146341)
				{

					m_vecNonInstMeshDesc[5].fMaxTime_Duration = 0.5f;
					m_vecNonInstMeshDesc[5].vSize = _float3(0.1f, 0.1f, 0.1f);
					m_vecNonInstMeshDesc[5].SizeSpeed = 15.f;
					m_vecNonInstMeshDesc[5].vSizingRUL = _float3(1, 1, 1);
					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[5]);
					

					//m_vecNonInstMeshDesc[1].vSize = _float3(0.125f);
					//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
					//	TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[1]);
					//m_vecNonInstMeshDesc[1].vSize = _float3(0.162f, 0.162f, -0.162f);
					//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
					//	TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[1]);

					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Attack_Grunt_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

					m_iAdjMovedIndex++;
				}
				if (PlayRate > 0.317073170 && PlayRate < 0.63414634146)
				{
					m_bIsLookAt = false;

					_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

					_float3 vGoalDir = (_float3(-0.073f, 35.900f, 281.488f).XMVector() - MonsterPos.XMVector());

					_float fLength = g_pGameInstance->Easing(TYPE_CircularIn, 0.f, vGoalDir.Get_Lenth(), (_float)PlayRate - 0.317073170f, 0.31707317146f);

					m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
				}
				if (m_iAdjMovedIndex == 2 && PlayRate > 0.63414634146)
				{
					g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Jump_Heavy.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
					if (!m_bIs2PageOnceJump)
					{
						//JH
						GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
						GetSingle(CUtilityMgr)->Get_MainCamera()->Lock_CamLook(true, XMVectorSet(0.f, -0.35f, 1.f, 0.f));
						m_pPlayerObj->Set_AttachCamPosOffset(_float3(0.f, 12.f, -15.f));
						//

						m_vecNonInstMeshDesc[5].fMaxTime_Duration = 999999999999999.f;
						m_vecNonInstMeshDesc[5].vSize = _float3(40.f);
						m_vecNonInstMeshDesc[5].SizeSpeed = 0.f;
						m_vecNonInstMeshDesc[5].vSizingRUL = _float3(0, 0, 0);
						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[5]);

						m_bIs2PageOnceJump = true;
					}

					//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
					//	TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[0]);
					//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
					//	TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[2]);


					m_bIsLookAt = false;
					m_iAdjMovedIndex++;
				}

				if (m_iAdjMovedIndex == 3 && PlayRate > 0.682926)
				{
					m_bIsJump = true;
					m_iAdjMovedIndex++;
				}
				if (m_iAdjMovedIndex == 4 && PlayRate > 0.71)
				{
					m_bIsJump = false;
					m_iAdjMovedIndex++;
				}
			}

		}
		break;

		case 12: //FireAttackStartAnim
		{
			m_bIsAtaackAimEnd = false;

			m_bIsLookAt = false;
			m_fAnimmultiple = 1.3f;

			//무기없애기
			if (PlayRate > 0.0411392 && m_iAdjMovedIndex == 0)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_SkullMace_Fire_Swing_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_bIsMainWeaponOff = true;
				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.167721518 && m_iAdjMovedIndex == 1)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Flamethrower_Ignite.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex; 
			}

			if (PlayRate > 0.205696202531 && m_iAdjMovedIndex == 2)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Footstep_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.43354430379 && m_iAdjMovedIndex == 3)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.5601265822 && m_iAdjMovedIndex == 4)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start_Long.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

		}
		break;

		case 13: //FireAttack
		{
			m_pFireParticleTransformCom->Set_MatrixState(CTransform::STATE_POS,
				m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) *1.25f +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) *-0.1f + XMVectorSet(0, 6.f, 0, 0));
			m_pFireParticleTransformCom->LookAt(
				m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * -3.5f);

			m_bIsAtaackAimEnd = false;

			m_bIsLookAt = true;
			m_pTransformCom->Move_Forward(fDeltatime , m_pNavigationCom);
		
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.14285714)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Flamethrower_Loop.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				m_vecTexInstDesc[0].vFixedPosition = m_vecTexInstDesc[1].vFixedPosition
					= m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(0, 0.25f, 0, 0) +
					m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * -1.5f +
					m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.2f;
				m_vecTexInstDesc[1].vFixedPosition.y = 35.9f;

				GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTexInstDesc[0]);
				GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTexInstDesc[1]);


				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 1 && PlayRate > 0.14285714)
			{


				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Footstep_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 2 && PlayRate > 0.71428571428)
			{
				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

				m_vecTexInstDesc[0].vFixedPosition = m_vecTexInstDesc[1].vFixedPosition 
					= m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(0, 0.25f, 0, 0) +
					m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 1.5f+
					m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.2f;
				m_vecTexInstDesc[1].vFixedPosition.y = 35.9f;
				GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTexInstDesc[0]);
				GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTexInstDesc[1]);


				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Footstep_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

		}
		break;

		case 14: //FireAttackEnd
		{
			m_bIsAtaackAimEnd = false;

			m_bIsLookAt = false;
			m_fAnimmultiple = 1.3f;

			if (m_iAdjMovedIndex == 0 && PlayRate > 0.117088607)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 1 && PlayRate > 0.193037974)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start_Long.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 2 && PlayRate > 0.5316455)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 3 && PlayRate > 0.588607594)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_BoneBreak_Start.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 4 && PlayRate > 0.8101265)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Flamethrower_Ignite.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 5 && PlayRate > 0.841772151)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Footstep_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			//무기생성
			if (PlayRate > 0.962025316 && m_iAdjMovedIndex == 6)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_SkullMace_Fire_Swing_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_bIsMainWeaponOff = false;

				for (auto& Weapon : m_pMainWeapons)
					Weapon->Set_Dissolve(true);

				for (auto& SubWeapon : m_pSubWeapons)
					SubWeapon->Set_Dissolve(true);
				
				++m_iAdjMovedIndex;
			}

		}
		break;
		case 15:
		{

		}
		break;

		//SecondPage_Jump
		case 20:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate < 0.2926829268)
			{
				m_bIsLookAt = false;
				m_iAdjMovedIndex++;
			}
			if (m_iAdjMovedIndex == 1 && PlayRate > 0.34146341)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Attack_Grunt_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				m_iAdjMovedIndex++;
			}
			if (PlayRate > 0.317073170 && PlayRate < 0.63414634146)
			{
				m_bIsLookAt = true;

				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

				_float3 vGoalDir = (_float3(0).XMVector() - MonsterPos.XMVector());

				_float fLength = g_pGameInstance->Easing(TYPE_CircularIn, 0.f, vGoalDir.Get_Lenth(), (_float)PlayRate - 0.317073170f, 0.31707317146f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
			}
			if (m_iAdjMovedIndex == 2 && PlayRate > 0.63414634146)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Jump_Heavy.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_bIsLookAt = false;
				m_iAdjMovedIndex++;
			}

			if (m_iAdjMovedIndex == 3 && PlayRate > 0.682926)
			{
				m_bIsJump = true;
				m_iAdjMovedIndex++;
			}
			if (m_iAdjMovedIndex == 4 && PlayRate > 0.71)
			{
				m_bIsJump = false;
				m_iAdjMovedIndex++;
			}
		}
		break;

		}
	}
	else
	{
		if (iNowAnimIndex == 0)
		{
			//if (m_fRange > 12.f && !m_bIsAttack && !m_bIsWalk)
			//{
			//	m_bIsWalk = true;
			//	m_bIsAttack = true;
			//	m_bIsLookAt = false;

			//	_Vector LookDir = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK))
			//		+ XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * GetSingle(CUtilityMgr)->RandomFloat(-0.7f, 0.7f));

			//	m_pTransformCom->LookDir(LookDir);
			//	m_pModel->Change_AnimIndex(2);
			//}
		}
		if (iNowAnimIndex == 1)
		{
			if (m_ActivateSecondPage == false)
			{
				if (m_fRange > 12.f && !m_bIsAttack && !m_bIsWalk)
				{
					m_bIsWalk = true;
					m_bIsAttack = true;
					m_bIsLookAt = false;

					_Vector LookDir = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK))
						+ XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * GetSingle(CUtilityMgr)->RandomFloat(-0.7f, 0.7f));

					m_pTransformCom->LookDir(LookDir);
					m_pModel->Change_AnimIndex(2);
				}
			}
			else
			{
				m_bIsAttack = true;
				m_bIsLookAt = true;
			}
		}
		if (iNowAnimIndex == 3)
		{
			Set_IsDead();

			list<CGameObject*>* pList = g_pGameInstance->Get_ObjectList_from_Layer(m_eNowSceneNum, Tag_Layer(Layer_ParticleNoDead));
			if (pList != nullptr)
			{
				for (auto& pNoDeadParticleObj : *pList)
					((CNonInstanceMeshEffect*)pNoDeadParticleObj)->Set_GonnabeDie();
			}

			// Jino
			GetSingle(CUtilityMgr)->Get_MainCamera()->Set_FocusTarget(m_pPlayerObj);
			static_cast<CPlayer*>(m_pPlayerObj)->Set_State_StopActionEnd();
			//
		}
		if (iNowAnimIndex == 4)
		{
			m_ActivateSecondPage = true;

			for (auto& SecondPageWeapon : m_pSecondPageWeapons)
			{
				SecondPageWeapon->Set_Dissolve(true,2.f);
			}


		}
		if (iNowAnimIndex == 5)
		{
			//m_bIsLookAt = true;
			m_bIsBasicAttack = false;
			m_fAttackCoolTime = 3.f;
			if(m_fSkillCoolTime <= 1.f)
				m_fSkillCoolTime = 1.f;

			if (m_ActivateSecondPage)
			{
				for (auto& SecondPageWeapon : m_pSecondPageWeapons)
				{
					SecondPageWeapon->Set_IsAttackState(false);
				}
			}

			m_pModel->Change_AnimIndex(1);
		}
		if (iNowAnimIndex == 6)
		{
			for (_int i = 0; i < m_pMainWeapons.size(); ++i)
			{
				m_pMainWeapons[i]->Set_IsAttackState(false);
			}

			for (auto& Weapon : m_pMainWeapons) Weapon->SwordTrailOnOff(false);
			for (auto& Weapon : m_pSubWeapons) Weapon->SwordTrailOnOff(false);

			//m_bIsLookAt = true;
			m_fAttackCoolTime = 1.f;
			m_fSkillCoolTime = 6.f;
			m_fJumpTime = 0.5f;

		}
		if (iNowAnimIndex == 8)
		{
			if (!m_ActivateSecondPage)
			{
				m_bIsSpinAttack = true;

				for (_int i = 0; i < m_pMainWeapons.size(); ++i)
					m_pMainWeapons[i]->Set_IsAttackState(true);

				for (_int i = 0; i < m_pSubWeapons.size(); ++i)
					m_pSubWeapons[i]->Set_IsAttackState(true);
			}
			else
			{
				if (!m_bIsSpinAttack)
				{

					m_vecMeshParticleDesc[0].vFixedPosition = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
					GetSingle(CUtilityMgr)->Create_MeshInstance(m_eNowSceneNum, m_vecMeshParticleDesc[0]);

					m_bIsSpinAttack = true;
				}


				for (auto& SecondPageWeapon : m_pSecondPageWeapons)
				{
					SecondPageWeapon->Set_IsAttackState(true);
					SecondPageWeapon->Set_SpinAttack(true);
					SecondPageWeapon->Set_InitializSpinAttackDistance();
					//SecondPageWeapon->Set_InitialPosDessolve(false);
				}
				m_pModel->Change_AnimIndex(9);
			}

			m_fSpinTime = 14.f;
		}
		if (iNowAnimIndex == 9)
		{
			if (!m_ActivateSecondPage)
			{
				if (!m_bIsSpinAttack)
					m_bIsSpinAttack = true;
			}
			else
			{
				if (!m_bIsSpinAttack)
				{
					m_bIsSpinAttack = true;
				}
			}
		}
		if (iNowAnimIndex == 10)
		{
			if (m_ActivateSecondPage && !m_bISVolcanoAttackStart)
			{
				m_bIsLookAt = true;
				m_pModel->Change_AnimIndex(1);

			}
			m_fJumpTime = 3.f;

		}
		if (iNowAnimIndex == 12)
		{
			//m_bIsLookAt = true;
			if (!m_bIsFireAttack)
			{

				m_pFireParticleTransformCom->Set_MatrixState(CTransform::STATE_POS,
					m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) *1.f + XMVectorSet(0, 6.5f, 0, 0));
				m_pFireParticleTransformCom->LookAt(
					m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * -3.5f);

				GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTexInstDesc[2]);
				//GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTexInstDesc[5]);
				m_bIsFireAttack = true;
			}
			m_fFireTime = 10.f;
			m_fAnimmultiple = 1.f;
		}
		if (iNowAnimIndex == 14)
		{
			//m_bIsLookAt = true;
			m_fAttackCoolTime = 0.5f;
			m_fSkillCoolTime = 6.f;
			m_fAnimmultiple = 1.f;
		}
		if (iNowAnimIndex == 15)
		{
			m_bIsBasicAttack = false;
			m_fAttackCoolTime = 3.f;
			m_fSkillCoolTime = 1.f;

			m_pModel->Change_AnimIndex(1);
		}
		if (iNowAnimIndex == 20)
		{
			m_bIsLookAt = true;
			m_fJumpTime = 3.f;
			m_pModel->Change_AnimIndex(1);
		}
	}


	m_iOldAnimIndex = iNowAnimIndex;
	m_OldPlayRate = PlayRate;
	return S_OK;
}

CChiedtian * CChiedtian::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CChiedtian*	pInstance = NEW CChiedtian(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CChiedtian");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CChiedtian::Clone(void * pArg)
{
	CChiedtian*	pInstance = NEW CChiedtian(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CChiedtian");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChiedtian::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pFireCollider);
	Safe_Release(m_pJumpCollider);
	Safe_Release(m_pFireParticleTransformCom);
	Safe_Release(m_pDissolve);

	for (auto& Weapon : m_pMainWeapons)
		Safe_Release(Weapon);

	for (auto& SubWeapon : m_pSubWeapons)
		Safe_Release(SubWeapon);

	for (auto& SecondPageWeapon : m_pSecondPageWeapons)
		Safe_Release(SecondPageWeapon);

	m_pMainWeapons.clear();
	m_pSubWeapons.clear();
	m_pSecondPageWeapons.clear();

	Safe_Release(m_pHPUI);
}
