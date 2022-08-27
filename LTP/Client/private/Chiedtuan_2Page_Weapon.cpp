#include "stdafx.h"
#include "..\public\Chiedtuan_2Page_Weapon.h"

CChiedtuan_2Page_Weapon::CChiedtuan_2Page_Weapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CChiedtuan_2Page_Weapon::CChiedtuan_2Page_Weapon(const CChiedtuan_2Page_Weapon & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CChiedtuan_2Page_Weapon::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CChiedtuan_2Page_Weapon::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	ZeroMemory(&m_WeaponDesc, sizeof(WEAPOPNDESC));
	if (nullptr != pArg)
	{
		memcpy(&m_WeaponDesc, pArg, sizeof(WEAPOPNDESC));
	}

	FAILED_CHECK(SetUp_Components());

	m_pTransformCom->Scaled_All(_float3(10.f));

	switch (m_WeaponDesc.KatanaPOSType)
	{
	case Client::CChiedtuan_2Page_Weapon::KATANA_TR:
		m_bYDirection = true;
		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_TL:
		m_bYDirection = false;
		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BR:
		m_bYDirection = false;
		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BL:
		m_bYDirection = true;
		break;
	}

	m_PlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player));

	m_fDistance = 20.f;
	m_fWeaponPosY = 2.5f;

	FAILED_CHECK(Ready_ParticleDesc());
	return S_OK;
}

_int CChiedtuan_2Page_Weapon::Update(_double fDeltaTime)
{
	m_DelaTime = (_float)fDeltaTime;
	if (__super::Update(fDeltaTime) < 0) return -1;


	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));

	if (m_pDissolveCom->Get_IsFadeIn() == false && m_pDissolveCom->Get_DissolvingRate() >= 1.0)
	{
		if (m_bIsInitialPosDessolve && !m_DeadDessolve)
		{
			m_bIsInitialPosDessolve = false;
			m_WeaponMoveTime = 0.f;
			m_pDissolveCom->Set_DissolveOn(true, 1.f);

		}
	}

	if (!m_bIsAttack && !m_bIsSpinAttack && !m_bIsVolcanoAttack)
		Set_WeaponPosition();
	else if (m_bIsAttack && !m_bIsSpinAttack && !m_bIsVolcanoAttack)
		Set_Attack(fDeltaTime);
	else if (m_bIsAttack && m_bIsSpinAttack && !m_bIsVolcanoAttack)
	{
		WeaponSpinAttack(fDeltaTime);
	}
	else if (m_bIsAttack && !m_bIsSpinAttack && m_bIsVolcanoAttack)
	{
		VolcanoAttack(fDeltaTime);
	}

	if (!m_bIsAttack && !m_bIsAttackfinish)
	{
		Set_Attack(fDeltaTime);
	}


	m_pCollider->Update_ConflictPassedTime(fDeltaTime);

	_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();

	_Matrix mat = m_pTransformCom->Get_WorldMatrix();
	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);
	_Vector vPos = mat.r[3];

	for (_uint i = 0; i < iNumCollider; i++)
		m_pCollider->Update_Transform(i, mat);

	if (m_bIsAttack)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));
	}


	FAILED_CHECK(Update_ParticleTransform());

	return _int();
}

_int CChiedtuan_2Page_Weapon::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel,nullptr, m_pDissolveCom));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_SWORD, m_pSwordTrail));

	//if(m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_TL)
		FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
		
	return _int();
}

_int CChiedtuan_2Page_Weapon::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));


	FAILED_CHECK(m_pDissolveCom->Render(10));

	return _int();
}

_int CChiedtuan_2Page_Weapon::LateRender()
{
	return _int();
}

void CChiedtuan_2Page_Weapon::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

void CChiedtuan_2Page_Weapon::Set_InitializSpinAttackDistance()
{
	switch (m_WeaponDesc.KatanaPOSType)
	{
	case Client::CChiedtuan_2Page_Weapon::KATANA_TR:
	{
		m_bIsDistance = false;
		m_fDistance = 10.f;
	}
	break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_TL:
	{
		m_bIsDistance = false;
		m_fDistance = 10.f;
	}
	break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BR:
	{
		m_bIsDistance = true;
		m_fDistance = 10.f;
	}
	break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BL:
	{
		m_bIsDistance = true;
		m_fDistance = 10.f;
	}
	break;
	}
}

void CChiedtuan_2Page_Weapon::Set_SecondPageWeaponStart()
{

}

void CChiedtuan_2Page_Weapon::Set_WeaponPosition()
{
	_float3 vMyNewPos;
	_float3 vRight;
	CTransform* BossTransform = (CTransform*)m_WeaponDesc.BossObj->Get_Component(TAG_COM(Com_Transform));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, BossTransform->Get_MatrixState(CTransform::STATE_POS));
	//m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));

	vRight = BossTransform->Get_MatrixState(CTransform::STATE_RIGHT);
	vRight = XMVector3Normalize(vRight.XMVector());

	switch (m_WeaponDesc.KatanaPOSType)
	{
	case Client::CChiedtuan_2Page_Weapon::KATANA_TR:
		ShakeWeaponY();
		vMyNewPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) +
			(XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -3.f) +
			(vRight.XMVector() * -3.f);
		vMyNewPos.y += m_fWeaponPosY;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(35.f));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT), XMConvertToRadians(-90.f));

		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_TL:
		ShakeWeaponY();
		vMyNewPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -3.f) + (vRight.XMVector() * -1.5f);
		vMyNewPos.y += m_fWeaponPosY;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(15.f));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT), XMConvertToRadians(-90.f));

		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BR:
		ShakeWeaponY();
		vMyNewPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -3.f) + (vRight.XMVector() * 1.5f);
		vMyNewPos.y += m_fWeaponPosY;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(-15.f));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT), XMConvertToRadians(-90.f));

		break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BL:

		ShakeWeaponY();
		vMyNewPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -3.f) + (vRight.XMVector() * 3.f);
		vMyNewPos.y += m_fWeaponPosY;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(-35.f));
		m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT), XMConvertToRadians(-90.f));

		break;
	}
}

void CChiedtuan_2Page_Weapon::Set_Attack(_double fDeltaTime)
{
	if (m_bIsInitialPosDessolve == false)
	{
		m_WeaponMoveTime += (_float)fDeltaTime *3.f;
		if (m_WeaponMoveTime >= 0.95f)
			m_WeaponMoveTime = 0.95f;

		_Vector Dir = XMVector3Normalize(m_vPlayerPos.XMVector() - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		_Vector vNewLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * ( 1.f - m_WeaponMoveTime) +
			Dir * m_WeaponMoveTime);

		m_pTransformCom->LookDir_ver2(vNewLook);

		m_pTransformCom->Move_Forward(fDeltaTime * 15.f);



		CTransform* PlayerTransfom = (CTransform*)m_PlayerObj->Get_Component(TAG_COM(Com_Transform));

		if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y < PlayerTransfom->Get_MatrixState_Float3(CTransform::STATE_POS).y)
		{
			m_vPlayerPos.y += 1.f;

			m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_vPlayerPos);

			g_pGameInstance->Play3D_Sound(L"JJB_2PageAttac.mp3", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

			m_bIsInitialPosDessolve = true;
			m_pDissolveCom->Set_DissolveOn(false, 1.f);
			m_WeaponMoveTime = 0.f;
			m_bIsAttackfinish = true;
		}

	}

}

void CChiedtuan_2Page_Weapon::Set_Dissolve(_bool FadeIn, _double Time)
{
	m_bIsInitialPosDessolve = true;
	m_pDissolveCom->Set_DissolveOn(FadeIn, Time);
}

void CChiedtuan_2Page_Weapon::WeaponSpinAttack(_double fDeltaTime)
{
	CTransform* BossTransform = (CTransform*)m_WeaponDesc.BossObj->Get_Component(TAG_COM(Com_Transform));

	m_fAngle += 13.f;//(_float)fDeltaTime;
	if (m_fAngle >= 360.f)
		m_fAngle = 0.f;

	if (m_bIsDistance == false)
	{
		m_fDistance -= (_float)fDeltaTime * 10.f;
		if (m_fDistance <= 6.5f)
			m_bIsDistance = true;
	}
	else
	{
		m_fDistance += (_float)fDeltaTime * 10.f;
		if (m_fDistance >= 35.f)
			m_bIsDistance = false;
	}


	switch (m_WeaponDesc.KatanaPOSType)
	{
	case Client::CChiedtuan_2Page_Weapon::KATANA_TR:
	{
		if (m_bIsBeginningPos)
		{
			m_bIsBeginningPos = false;
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, BossTransform->Get_MatrixState(CTransform::STATE_POS));
			_Vector vMyNewPos = BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * 10.f);
			vMyNewPos = XMVectorSetY(vMyNewPos, XMVectorGetY(vMyNewPos) + 1.f);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyNewPos);
			m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));

			INSTPARTICLEDESC tDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"JY_TextureEft_11");
			tDesc.FollowingTarget = m_pJYParticleTransform;
			tDesc.iFollowingDir = FollowingDir_Look;
			tDesc.TotalParticleTime = 10.f;
			tDesc.Particle_Power = 15.f;

			_Vector vDir = XMVector3Normalize(m_pCollider->Get_ColliderPosition(3).XMVector() - m_pCollider->Get_ColliderPosition(1).XMVector());
			m_pSwordTrail->Set_TrailTurnOn(true, m_pCollider->Get_ColliderPosition(1), m_pCollider->Get_ColliderPosition(1).XMVector() + vDir * 7.f);

			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, tDesc);
		}

		m_pTransformCom->LookDir(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) - BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS)));
		m_pTransformCom->Turn_Revolution_CW(BossTransform->Get_MatrixState(CTransform::STATE_POS), m_fDistance, fDeltaTime * 5.f);

		m_pTransformCom->Rotation_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_fAngle));
	}
	break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_TL:
	{
		if (m_bIsBeginningPos)
		{
			m_bIsBeginningPos = false;
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, BossTransform->Get_MatrixState(CTransform::STATE_POS));
			_Vector vMyNewPos = BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -10.f);
			vMyNewPos = XMVectorSetY(vMyNewPos, XMVectorGetY(vMyNewPos) + 1.f);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyNewPos);
			m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));


			_Vector vDir = XMVector3Normalize(m_pCollider->Get_ColliderPosition(3).XMVector() - m_pCollider->Get_ColliderPosition(1).XMVector());
			m_pSwordTrail->Set_TrailTurnOn(true, m_pCollider->Get_ColliderPosition(1), m_pCollider->Get_ColliderPosition(1).XMVector() + vDir * 7.f);

			INSTPARTICLEDESC tDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"JY_TextureEft_11");
			tDesc.FollowingTarget = m_pJYParticleTransform;
			tDesc.iFollowingDir = FollowingDir_Look;
			tDesc.TotalParticleTime = 10.f;
			tDesc.Particle_Power = 15.f;
			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, tDesc);
		}
		m_pTransformCom->LookDir(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) - BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS)));
		m_pTransformCom->Turn_Revolution_CW(BossTransform->Get_MatrixState(CTransform::STATE_POS), m_fDistance, fDeltaTime * 5.f);

		m_pTransformCom->Rotation_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_fAngle));
	}
	break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BR:
	{
		if (m_bIsBeginningPos)
		{
			m_bIsBeginningPos = false;
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, BossTransform->Get_MatrixState(CTransform::STATE_POS));
			_Vector vMyNewPos = BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_RIGHT)) * 10.f);
			vMyNewPos = XMVectorSetY(vMyNewPos, XMVectorGetY(vMyNewPos) + 1.f);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyNewPos);
			m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));


			_Vector vDir = XMVector3Normalize(m_pCollider->Get_ColliderPosition(3).XMVector() - m_pCollider->Get_ColliderPosition(1).XMVector());
			m_pSwordTrail->Set_TrailTurnOn(true, m_pCollider->Get_ColliderPosition(1), m_pCollider->Get_ColliderPosition(1).XMVector() + vDir * 7.f);

			INSTPARTICLEDESC tDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"JY_TextureEft_11");
			tDesc.FollowingTarget = m_pJYParticleTransform;
			tDesc.iFollowingDir = FollowingDir_Look;
			tDesc.TotalParticleTime = 10.f;
			tDesc.Particle_Power = 15.f;

			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, tDesc);

		}
		m_pTransformCom->LookDir(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) - BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS)));
		m_pTransformCom->Turn_Revolution_CW(BossTransform->Get_MatrixState(CTransform::STATE_POS), m_fDistance, fDeltaTime * 5.f);

		m_pTransformCom->Rotation_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_fAngle));
	}
	break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BL:
	{
		if (m_bIsBeginningPos)
		{
			m_bIsBeginningPos = false;
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, BossTransform->Get_MatrixState(CTransform::STATE_POS));
			_Vector vMyNewPos = BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_RIGHT)) * -10.f);
			vMyNewPos = XMVectorSetY(vMyNewPos, XMVectorGetY(vMyNewPos) + 1.f);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyNewPos);
			m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));


			_Vector vDir = XMVector3Normalize(m_pCollider->Get_ColliderPosition(3).XMVector() - m_pCollider->Get_ColliderPosition(1).XMVector());
			m_pSwordTrail->Set_TrailTurnOn(true, m_pCollider->Get_ColliderPosition(1), m_pCollider->Get_ColliderPosition(1).XMVector() + vDir * 7.f);

			INSTPARTICLEDESC tDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"JY_TextureEft_11");
			tDesc.FollowingTarget = m_pJYParticleTransform;
			tDesc.iFollowingDir = FollowingDir_Look;
			tDesc.TotalParticleTime = 10.f;
			tDesc.Particle_Power = 15.f;
			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, tDesc);
		}

		m_pTransformCom->LookDir(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) - BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS)));
		m_pTransformCom->Turn_Revolution_CW(BossTransform->Get_MatrixState(CTransform::STATE_POS), m_fDistance, fDeltaTime * 5.f);

		m_pTransformCom->Rotation_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_fAngle));
	}
	break;
	}
	//_float3 vPivotPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	//vPivotPos.y += 1.f;
	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPivotPos);
}

void CChiedtuan_2Page_Weapon::SpinAttackOff()
{
	m_fDistance = 4.f;
	m_bIsDistance = false;
	m_bIsSpinAttack = false;
	m_bIsBeginningPos = true;
	m_bIsAttack = false;
	//m_bIsInitialPosDessolve = false;
	m_fAngle = 0.f;
	Set_WeaponPosition();
	m_pDissolveCom->Set_DissolveOn(true, 1.f);

	m_pSwordTrail->Set_TrailTurnOn(false, _float3(0), _float3(0));
}

void CChiedtuan_2Page_Weapon::VolcanoAttackOff()
{
	m_bIsInitialPosDessolve = false;
	m_bIsAttack = false;
	m_bIsVolcanoAttack = false;
	m_WeaponMoveTime = 0.f;
	Set_WeaponPosition();
	m_pDissolveCom->Set_DissolveOn(true, 0.5f);
}

void CChiedtuan_2Page_Weapon::BeginningPos()
{
	CTransform* BossTransform = (CTransform*)m_WeaponDesc.BossObj->Get_Component(TAG_COM(Com_Transform));

	switch (m_WeaponDesc.KatanaPOSType)
	{
	case Client::CChiedtuan_2Page_Weapon::KATANA_TR:
	{
		m_bIsBeginningPos = false;
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, BossTransform->Get_MatrixState(CTransform::STATE_POS));
		_Vector vMyNewPos = BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * 10.f);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
	}
	break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_TL:
	{
		m_bIsBeginningPos = false;
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, BossTransform->Get_MatrixState(CTransform::STATE_POS));
		_Vector vMyNewPos = BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * -10.f);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
	}
	break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BR:
	{
		m_bIsBeginningPos = false;
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, BossTransform->Get_MatrixState(CTransform::STATE_POS));
		_Vector vMyNewPos = BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_RIGHT)) * 10.f);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
	}
	break;
	case Client::CChiedtuan_2Page_Weapon::KATANA_BL:
	{
		m_bIsBeginningPos = false;
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, BossTransform->Get_MatrixState(CTransform::STATE_POS));
		_Vector vMyNewPos = BossTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) + (XMVector3Normalize(BossTransform->Get_MatrixState(CTransform::TransformState::STATE_RIGHT)) * -10.f);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyNewPos);
		m_pTransformCom->LookAtExceptY(BossTransform->Get_MatrixState(CTransform::STATE_POS));
	}
	break;
	}
}

void CChiedtuan_2Page_Weapon::VolcanoAttack(_double fDeltaTime)
{
	if (m_bIsInitialPosDessolve == false)
	{
		m_WeaponMoveTime += (_float)fDeltaTime *3.f;
		if (m_WeaponMoveTime >= 0.95f)
			m_WeaponMoveTime = 0.95f;

		_Vector Dir = XMVector3Normalize(m_vPlayerPos.XMVector() - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		_Vector vNewLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * (1.f - m_WeaponMoveTime) +
			Dir * m_WeaponMoveTime);

		m_pTransformCom->LookDir_ver2(vNewLook);

		m_pTransformCom->Move_Forward(fDeltaTime * 15.f);

	}

	CTransform* PlayerTransfom = (CTransform*)m_PlayerObj->Get_Component(TAG_COM(Com_Transform));

	if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y < PlayerTransfom->Get_MatrixState_Float3(CTransform::STATE_POS).y)
	{
		g_pGameInstance->Play3D_Sound(L"JJB_2PageAttac.mp3", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

		m_vPlayerPos.y += 1.f;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_vPlayerPos);

		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Volcano), TAG_OP(Prototype_Object_Boss_Volcano), &m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		g_pGameInstance->Play3D_Sound(L"JJB_Volcano.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
		Set_WeaponPosition();

		m_bIsInitialPosDessolve = true;
		m_pDissolveCom->Set_DissolveOn(true, 1.f);
		m_WeaponMoveTime = 0.f;
	}
}

void CChiedtuan_2Page_Weapon::ShakeWeaponY()
{
	if (m_bYDirection)
	{
		m_fWeaponPosY += (_float)m_DelaTime;

		if (m_fWeaponPosY >= 3.f)
			m_bYDirection = false;
	}
	else
	{
		m_fWeaponPosY -= (_float)m_DelaTime;

		if (m_fWeaponPosY <=  2.f)
			m_bYDirection = true;
	}
}

HRESULT CChiedtuan_2Page_Weapon::SetUp_Components()
{

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	CSwordTrail::TRAILDESC tSwordDesc;
	tSwordDesc.iPassIndex = 0;
	tSwordDesc.vColor = _float4(1.f, 0.2f, 0.1f, 1.f);
	tSwordDesc.iTextureIndex = 1;
	tSwordDesc.NoiseSpeed = 0;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_SwordTrail), TAG_COM(Com_SwordTrail), (CComponent**)&m_pSwordTrail, &tSwordDesc));

	

	if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_TR)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_SecondPage_ChieftianWeapon), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_TL)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_SecondPage_ChieftianWeapon2), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_BR)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_SecondPage_ChieftianWeapon3), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_BL)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_SecondPage_ChieftianWeapon4), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	}

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	CDissolve::DISSOLVEDESC	tDissolveDesc;

	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_TR)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(11.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 3.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 1.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 3.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 6.0f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_TL)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(11.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 3.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 1.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 3.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 6.0f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_BR)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(11.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 3.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 1.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 3.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 6.0f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}
	else if (m_WeaponDesc.KatanaPOSType == CChiedtuan_2Page_Weapon::KATANA_BL)
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(11.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 3.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 1.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 3.5f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();

		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(4.f);
		ColliderDesc.vRotation = _float4(0, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0, 6.0f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}

	return S_OK;
}

HRESULT CChiedtuan_2Page_Weapon::Ready_ParticleDesc()
{
	Safe_Release(m_pJYParticleTransform);
	m_pJYParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pJYParticleTransform, E_FAIL);

	m_pJYParticleTransform->Set_IsOwnerDead(false);
	INSTPARTICLEDESC tDesc = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"JY_TextureEft_11");
	tDesc.FollowingTarget = m_pJYParticleTransform;
	tDesc.iFollowingDir = FollowingDir_Look;
	tDesc.TotalParticleTime = 999999999.f;
	tDesc.Particle_Power = 5.f;
	//tDesc.vEmissive_SBB = _float3(0);

	GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, tDesc);
	return S_OK;
}

HRESULT CChiedtuan_2Page_Weapon::Update_ParticleTransform()
{


	_float Rate = GetSingle(CUtilityMgr)->RandomFloat(0, 1);
	_Vector vDir = XMVector3Normalize(m_pCollider->Get_ColliderPosition(3).XMVector() - m_pCollider->Get_ColliderPosition(1).XMVector());
	_float3 vPosition = (m_pCollider->Get_ColliderPosition(1).XMVector() + vDir * 8.f) * Rate + m_pCollider->Get_ColliderPosition(1).XMVector() * (1.f - Rate);

	m_pJYParticleTransform->Set_MatrixState(CTransform::STATE_POS, vPosition);
	
	Rate = GetSingle(CUtilityMgr)->RandomFloat(-1, 1);

	m_pJYParticleTransform->LookDir(XMVector3Normalize(vDir  + XMVectorSet(0, Rate,0,0)));
	

	m_pSwordTrail->Update_SwordTrail(m_pCollider->Get_ColliderPosition(1), m_pCollider->Get_ColliderPosition(1).XMVector() + vDir * 7.f, g_fDeltaTime);

	return S_OK;
}

CChiedtuan_2Page_Weapon * CChiedtuan_2Page_Weapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CChiedtuan_2Page_Weapon*	pInstance = NEW CChiedtuan_2Page_Weapon(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CChiedtuan_2Page_Weapon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CChiedtuan_2Page_Weapon::Clone(void * pArg)
{
	CChiedtuan_2Page_Weapon*	pInstance = NEW CChiedtuan_2Page_Weapon(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CChiedtuan_2Page_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChiedtuan_2Page_Weapon::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolveCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pSwordTrail);

	if (m_pJYParticleTransform)
		m_pJYParticleTransform->Set_IsOwnerDead(true);
	Safe_Release(m_pJYParticleTransform);
}
