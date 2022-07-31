#include "stdafx.h"
#include "..\public\Snake.h"
#include "Player.h"
#include "Snake_Poison_Raser.h"

CSnake::CSnake(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CBoss(pDevice, pDeviceContext)
{
}

CSnake::CSnake(const CSnake & rhs)
	: CBoss(rhs)
{
}

HRESULT CSnake::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CSnake::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr) 
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));
		m_StartPos = *((_float3*)pArg);
	}
	//else
	//{
	//	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(8.f, -110.f, 53.f));
	//	
	//}

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));

	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(8.f, -110.f, 53.f));
	//m_StartPos = _float3(8.f, -110.f, 53.f);

	m_pTransformCom->Scaled_All(_float3(4.f, 4.f, 4.f));

	m_pModel->Change_AnimIndex_ReturnTo(5, 1);

	m_bIsAttack = true;

	m_fAttackCoolTime = 5.f;
	m_fSkillCoolTime = 8.f;  
	m_bIsHit = false;

	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));
	m_vPlayerStartPos = ((CTransform*)(m_pPlayerObj->Get_Component(TAG_COM(Com_Transform))))->Get_MatrixState(CTransform::STATE_POS);
	Set_IsOcllusion(true);

	_float3 Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	_float PosX = XMVectorGetX(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)) + 20.f;
	_float PosZ = XMVectorGetZ(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)) - 20.f;

	Pos.x = PosX;
	Pos.z = PosZ;
	m_AttackAnimPos = Pos;

	m_pModel->Update_AnimationClip(g_fDeltaTime);

	CSnake_Poison_Raser::SNAKERASERDESC RaserDesc;
	RaserDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "sk_jaw_01", _float3(1), _float3(0), _float3(-60.8041f, -0.000036f, -114.866f));
	RaserDesc.SnakeObj = this;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pRaserObj), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_Snake_Poison_Raser), &RaserDesc);

	m_fAngleSpeed = 1.f;
	return S_OK;
}

_int CSnake::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	//if (!TestBool)
	//{
	//	TestBool = true;
	//	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(8.f, -110.f, 53.f));
	//}

	m_pMotionTrail->Update_MotionTrail(fDeltaTime);

	// Jino
	if (true == static_cast<CPlayer*>(m_pPlayerObj)->Is_Hiding())
	{
		m_bHiding = true;
	}
	else
	{
		m_bHiding = false;
	}

	if (m_fSpecialSillTime > 0.f)
		m_fSpecialSillTime -= (_float)fDeltaTime;

	if (m_fSpecialSillTime <= 0 && !m_bIsAttack && m_bIsSpecialSkill)
	{
		m_bIsLookAt = false;
		m_bIsAttack = true;
		m_pModel->Change_AnimIndex(8);
	}


	if (!m_bIsAttack)
		m_fAttackCoolTime -= (_float)fDeltaTime;
	
	if (m_pModel->Get_NowAnimIndex() == 1)
		m_fRotTime += (_float)fDeltaTime;

	if (m_bIsAtackMoveStart)
		m_fNarrationTime -= (_float)fDeltaTime;

	if (m_fNarrationTime <= 0 && !m_bIsAttack)
	{
		m_fNarrationTime = 10.f;
		_int iRandom = rand() % 3 + 1;

		wstring teampString;
		teampString = L"JJB_Naga_" + to_wstring(iRandom) + L".wav";

		g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
	}

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

	PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;

	//_float3	MapDir = XMVector3Normalize(XMVectorSet(1.f, 0.f, 0.f, 1.f) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	m_vMapAngle = XMVector3Dot(XMVectorSet(1.f, 0.f, 0.f, 1.f), XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)));
	_float3 TargetDir = XMVector3Normalize(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	m_vAngle = XMVector3Dot(XMLoadFloat3(&TargetDir), XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)));

	// #DEBUG
	if (g_pGameInstance->Get_DIKeyState(DIK_M)& DIS_Down)
		m_bTestHodeing = !m_bTestHodeing;

	if (g_pGameInstance->Get_DIKeyState(DIK_N)& DIS_Down)
		m_pModel->Change_AnimIndex_ReturnTo(5, 1);

	if (m_bIsAttack && m_bIsLookAt)
	{
		CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
		_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

		PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;
		_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		Dir = XMVector3Normalize(PlayerPos.XMVector() - XMVectorSetY(Dir, PlayerPos.y));
		m_pTransformCom->Turn_Dir(Dir, 0.90f);
	}

	if (KEYDOWN(DIK_B))
	{
		m_vAngle = _float3(0.99f,0.1f,0.1f).XMVector();
		m_fAttackCoolTime = 0;
		m_bHiding = false;
		m_bTestHodeing = false;
	}

	if (XMVectorGetX(m_vAngle) > 0.99f && !m_bIsAttack && m_fAttackCoolTime <= 0.f)
	{ 
		//if (m_bTestHodeing)
		//	m_bHiding = true;


		if (m_bHiding)
		{
			m_bIsSpecialSkill = false;
			//m_bIsAttack = true;
			// #TIME
			m_pModel->Change_AnimIndex_ReturnTo(2, 1);
		}
		else
		{
			m_bIsAttack = true;

			_int iRandom =  (_int)GetSingle(CUtilityMgr)->RandomFloat(1.0f, 2.9f);
			iRandom = 1;

			// #TIME
			if(iRandom == 1)
				m_pModel->Change_AnimIndex(3);
			else
				m_pModel->Change_AnimIndex(7);
		}
	}

	//m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple)));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	m_pCollider->Update_ConflictPassedTime(fDeltaTime);
	m_pCollider->Update_Transform(0, m_AttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed());

	if (m_bIsBite)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));
	}

	m_pSpecialSkillCollider->Update_ConflictPassedTime(fDeltaTime);
	m_pSpecialSkillCollider->Update_Transform(0, m_AttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed());

	if (m_bIsSpecialSkillAttack)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pCollider));
	}

	//if(m_bIsSpecialSkillAttack)
		m_pRaserObj->Update(fDeltaTime);

	return _int();
}

_int CSnake::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_MOTION, m_pMotionTrail));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pSpecialSkillCollider));

	//if(m_bIsSpecialSkillAttack)
		m_pRaserObj->LateUpdate(fDeltaTime);

	return _int();
}

_int CSnake::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	}

	return _int();
}

_int CSnake::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CSnake::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		pConflictedObj->Take_Damage(this, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), false, 0.f);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}


HRESULT CSnake::Ready_ParticleDesc()
{
	// HandPos

	m_pTextureParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform, E_FAIL);
	m_pTextureParticleTransform->Set_MoveSpeed(1.f);

	m_pTextureParticleTransform1 = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform1, E_FAIL);
	m_pTextureParticleTransform1->Set_MoveSpeed(1.f);

	// TailPos
	m_pTextureParticleTransform2 = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform2, E_FAIL);



	//// 0
	//INSTPARTICLEDESC instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_Default);
	//instanceDesc.TotalParticleTime = 99999.f;
	//instanceDesc.FollowingTarget = m_pTextureParticleTransform_BowUp;
	////	GETPARTICLE->Create_Texture_Effect_Desc(instanceDesc, m_eNowSceneNum);
	//m_vecTextureParticleDesc.push_back(instanceDesc);


	//// 1
	//instanceDesc.FollowingTarget = m_pTextureParticleTransform_BowBack;
	////	GETPARTICLE->Create_Texture_Effect_Desc(instanceDesc, m_eNowSceneNum);
	//m_vecTextureParticleDesc.push_back(instanceDesc);

	//// 9999���� �״´�. 
	//m_pTextureParticleTransform_BowUp->Set_IsOwnerDead(true);
	//m_pTextureParticleTransform_BowBack->Set_IsOwnerDead(true);

	{
		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.vLookDir = _float3(1, 0, 0);

		tNIMEDesc.eMeshType = Prototype_Mesh_JY_Dough_2;



		tNIMEDesc.fMaxTime_Duration = 0.5f;
		tNIMEDesc.fAppearTime = 0.35f;
		tNIMEDesc.noisingdir = _float2(0, -1);


		tNIMEDesc.NoiseTextureIndex = 350;
		tNIMEDesc.MaskTextureIndex = 81;
		tNIMEDesc.iDiffuseTextureIndex = 232;
		tNIMEDesc.m_iPassIndex = 17;
		tNIMEDesc.vEmissive = _float4(1, 1.f, 1.f, 1);

		tNIMEDesc.vLimLight = _float4(1.f, 0.f, 1.f, 1.f);
		tNIMEDesc.vColor = _float3(1.f, 0.f, 1.f);





		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = 1080.f;
		tNIMEDesc.vSize = _float3(1.5f, 1.5f, 1.5f);
		tNIMEDesc.SizeSpeed = 1.f;
		tNIMEDesc.vSizingRUL = _float3(1.f, 0, 1.f);

		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = 0;

		m_vecJYMeshNonInst.push_back(tNIMEDesc);
	}
	{

		NONINSTNESHEFTDESC tNIMEDesc;
		tNIMEDesc.vPosition = _float3(0.f, -96.5f, 96.5f);
		tNIMEDesc.vLookDir = _float3(1, 0, 1).Get_Nomalize();

		tNIMEDesc.eMeshType = Prototype_Mesh_Cylinder;

		tNIMEDesc.fMaxTime_Duration = 4.6666666666f;
		tNIMEDesc.fAppearTime = 0.5f;

		tNIMEDesc.noisingdir = _float2(0, 1);

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 109;
		tNIMEDesc.iDiffuseTextureIndex = 370;
		//tNIMEDesc.iDiffuseTextureIndex = 366;
		tNIMEDesc.m_iPassIndex = 20;
		tNIMEDesc.vEmissive = _float4(0, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(1, 1, 0.2f, 0);
		tNIMEDesc.vColor = _float3(1.f, 0, 0);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = 0.f;

		tNIMEDesc.fAlphaTestValue = 0.0f;


		tNIMEDesc.vSize = _float3(16.5f, 16.5f, -16.5f).XMVector() * 3.8f;
		m_vecJYMeshNonInst.push_back(tNIMEDesc);

	}


	return S_OK;
}

HRESULT CSnake::Update_Particle(_double timer)
{
	_Matrix mat_1 = m_AttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed();
	_Matrix mat_2 = m_pTransformCom->Get_WorldMatrix();


	m_pTextureParticleTransform->Set_Matrix(mat_1);

	
	mat_2.r[0] = XMVector3Normalize(mat_2.r[0]);
	mat_2.r[1] = XMVector3Normalize(mat_2.r[1]);
	mat_2.r[2] = XMVector3Normalize(mat_2.r[2]);
	mat_2.r[3] = m_pCollider->Get_ColliderPosition(0).XMVector();

	m_pTextureParticleTransform2->Set_Matrix(mat_2);

	

	return S_OK;
}


HRESULT CSnake::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_Snake), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(1));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(20.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "sk_tongue_01", _float3(1), _float3(0), _float3(-172.598f, -0.000048f, 76.4859f));
	m_AttachedDesc = tAttachedDesc;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSub), (CComponent**)&m_pSpecialSkillCollider));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(500.f, 20.f, 30.f);
	ColliderDesc.vRotation = _float4(13.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pSpecialSkillCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));

	tAttachedDesc.Initialize_AttachedDesc(this, "sk_jaw_01", _float3(1), _float3(0), _float3(-60.8041f, -0.000036f, -114.866f));
	m_SpecialSkillAttachedDesc = tAttachedDesc;


	CMotionTrail::MOTIONTRAILDESC tMotionDesc;
	tMotionDesc.iNumTrailCount = 25;
	tMotionDesc.iPassIndex = 15;
	tMotionDesc.pModel = m_pModel;
	tMotionDesc.pShader = m_pShaderCom;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_MotionTrail), TAG_COM(Com_MotionTrail), (CComponent**)&m_pMotionTrail, &tMotionDesc));
	


	return S_OK;
}

HRESULT CSnake::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	{
		m_EffectAdjust = 0;
		m_iAdjMovedIndex = 0;
	}


	if (PlayRate <= 0.98)
	{
		switch (iNowAnimIndex)
		{
		case 0:
		{
		}
		break;

		case 1:
		{

			float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.46f, 0.44f, 0.21f, Value), _float4(Value, Value*0.7f, Value, 0.9f));

			m_bIsAttack = false;
			if (m_iRotationRandom == 0)
			{
				//CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				//_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				//PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;
				//_float3 TargetDir = XMVector3Normalize(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
				//m_vAngle = XMVector3Dot(XMLoadFloat3(&TargetDir), XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)));
				
				_float fAngle = XMVectorGetX(m_vMapAngle);

				m_fAngleSpeed = m_fAngleSpeed *fAngle;

				if (fAngle > 0.4f)
					m_fAngleSpeed = 0.4f;

				if (m_fAngleSpeed < 0.2)
					m_fAngleSpeed = 0.2f;

				if (XMVectorGetX(m_vMapAngle) <= 0 && !m_bIsAngleOut)
				{
					m_fAngleSpeed = 0.1f;
					m_bIsAngleOut = true;
				}

				if(!m_bIsAngleOut)
					m_pTransformCom->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * m_fAngleSpeed);
				else
					m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * m_fAngleSpeed);
			}
			else if (m_iRotationRandom == 1)
			{
				_float fAngle = XMVectorGetX(m_vMapAngle);

				m_fAngleSpeed = m_fAngleSpeed * fAngle;

				if (fAngle > 0.4f)
					m_fAngleSpeed = 0.4f;

				if (m_fAngleSpeed < 0.2)
					m_fAngleSpeed = 0.2f;

				if (XMVectorGetX(m_vMapAngle) <= 0 && !m_bIsAngleOut)
				{
					m_fAngleSpeed = 0.1f;
					m_bIsAngleOut = true;
				}

				if (!m_bIsAngleOut)
					m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * m_fAngleSpeed);
				else
					m_pTransformCom->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * m_fAngleSpeed);
			}
			//else if (m_iRotationRandom == 2)
			//{
			//	m_fRotTime += (_float)fDeltatime;

			//	if (m_fRotTime < 3)
			//		m_pTransformCom->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * 0.5f);
			//	else
			//		m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * 0.5f);
			//}
			if (PlayRate > 0.2425 && m_iAdjMovedIndex == 0)
			{
				g_pGameInstance->Play3D_Sound(TEXT("JJB_Wave_Snake_Hiss.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
				m_iAdjMovedIndex++;
			}

			if (PlayRate > 0.6525 && m_iAdjMovedIndex == 1)
			{
				g_pGameInstance->Play3D_Sound(TEXT("JJB_Wave_Snake_Hiss.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
				m_iAdjMovedIndex++;
			}


		}
		break;

		case 2:
		{
			if (PlayRate > 0.2425 && m_iAdjMovedIndex == 0)
			{
				g_pGameInstance->Play3D_Sound(TEXT("JJB_Wave_Snake_Hiss.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
				m_iAdjMovedIndex++;
			}

			if (PlayRate > 0.6525 && m_iAdjMovedIndex == 1)
			{
				g_pGameInstance->Play3D_Sound(TEXT("JJB_Wave_Snake_Hiss.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
				m_iAdjMovedIndex++;
			}

			if (PlayRate > 0 && PlayRate < 0.5f)
			{
				if (m_iAdjMovedIndex == 0 && PlayRate > 0.499999f)
				{
					m_bIsLookAt = false;
					++m_iAdjMovedIndex;
				}
				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				_float3 FindPos = (MonsterPos.XMVector() + m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 20.f);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				PlayerPos.y = FindPos.y =  MonsterPos.y;

				_float3 vGoalDir = (PlayerPos.XMVector() - FindPos.XMVector());
				_float fLength = g_pGameInstance->Easing(TYPE_SinInOut, 0, vGoalDir.Get_Lenth(), (_float)PlayRate, 0.5f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
			}
			else if (PlayRate > 0.55f && PlayRate < 0.98f)
			{
				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

				//CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				//_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				//PlayerPos.y = MonsterPos.y;

				_float3 vGoalDir = (m_StartPos.XMVector() - MonsterPos.XMVector());
				_float fLength = g_pGameInstance->Easing(TYPE_SinInOut, 0, vGoalDir.Get_Lenth(), (_float)PlayRate - 0.55f, 0.43f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
			}
		}
		break;

		case 3:
		{
			float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.87f, 0.39f, 0.96f, Value), _float4(Value, Value*0.5f, Value, 0.9f));

			static _double Timer = 0;
			static _float3 FindPos = _float3(0);
			static _uint	iEffectCount = 0;

			if (PlayRate > 0 && PlayRate <= 0.27678571)
			{
				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				FindPos = (MonsterPos.XMVector() + m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 20.f);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				PlayerPos.y = FindPos.y = MonsterPos.y;

				_float3 vGoalDir = (PlayerPos.XMVector() - FindPos.XMVector());
				_float fLength = g_pGameInstance->Easing(TYPE_SinInOut, 0, vGoalDir.Get_Lenth(), (_float)PlayRate, 0.27678571f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
				//_float3 EsingPos = g_pGameInstance->Easing_Vector(TYPE_SinInOut, m_StartPos.XMVector(), m_AttackAnimPos.XMVector(), (_float)PlayRate, 0.27678571f);

				//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, EsingPos);
			}

			if (PlayRate < 0.3303571428571428571f)
			{
				iEffectCount = 0;

				if (PlayRate > 0.258928571428571428f)
				{
					Timer -= fDeltatime;
					if (Timer < 0)
					{
						m_pTextureParticleTransform->Set_Matrix(m_pTransformCom->Get_WorldMatrix());
						m_pTextureParticleTransform1->Set_Matrix(m_pTransformCom->Get_WorldMatrix());

						m_pTextureParticleTransform->Move_Right(30);
						m_pTextureParticleTransform1->Move_Right(-30);

						m_pTextureParticleTransform->LookAt(FindPos.XMVector());
						m_pTextureParticleTransform1->LookAt(FindPos.XMVector());



						m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(0.734375f, 0.13671875f, 0.98046875f, 6.472f), 3.236f);

						m_pMotionTrail->Add_MotionBuffer(m_pTextureParticleTransform->Get_WorldFloat4x4(), _float4(0.734375f, 0.13671875f, 0.98046875f, 6.472f), 3.236f);
						m_pMotionTrail->Add_MotionBuffer(m_pTextureParticleTransform1->Get_WorldFloat4x4(), _float4(0.734375f, 0.13671875f, 0.98046875f, 6.472f), 3.236f);
						Timer = 0.05;
					}
				}
			}
			else if (iEffectCount == 0)
			{
				m_vecJYMeshNonInst[0].vPosition = XMVectorSetY(FindPos.XMVector(),2.13f);
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecJYMeshNonInst[0]);



				iEffectCount++;
			}
			else
			{
				Timer = 0;

			}

			if (PlayRate > 0 && m_iAdjMovedIndex == 0)
			{
				m_bIsBite = true;


				m_iAdjMovedIndex++;
			}

			if (PlayRate > 0 && m_iAdjMovedIndex == 1)
			{
				g_pGameInstance->Play3D_Sound(TEXT("JJB_Snake_Hiss_Charge.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
				m_iAdjMovedIndex++;
			}

			if (PlayRate > 0.4375 && m_iAdjMovedIndex == 2)
			{
				g_pGameInstance->Play3D_Sound(TEXT("JJB_Snake_Bite.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);

				m_iAdjMovedIndex++;
			}

			if (PlayRate > 0.45 && m_iAdjMovedIndex == 3)
			{
				g_pGameInstance->Play3D_Sound(TEXT("JJB_Snake_Hit_Impact.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);

				m_iAdjMovedIndex++;
			}


			if (m_EffectAdjust == 0 && PlayRate >0.f)
			{
				m_vecJYMeshNonInst[1].iDiffuseTextureIndex = 370;
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecJYMeshNonInst[1]);


				m_EffectAdjust++;
			}
			else if (m_EffectAdjust == 1 && PlayRate >0.05f)
			{

				m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSING_GODRAY, false);
				m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSING_LENSEFLARE, false);

				LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
				pLightDesc->vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
				pLightDesc->vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
				pLightDesc->vSpecular = _float4(0.f, 0.f, 0.f, 1.f);
			}

			//0.
			//if (m_EffectAdjust == 1 && PlayRate >= 0.3303571428571428571f)
			//	//if (m_EffectAdjust == 1 && PlayRate >= 0.455f)
			//{
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_SNAKE_1, m_pTextureParticleTransform2);
			//	m_EffectAdjust++;
			//}
			//if (m_EffectAdjust == 2 && PlayRate >= 0.455f)
			//	//if (m_EffectAdjust == 2 && PlayRate >= 0.555f)
			//{
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_SNAKE_2, m_pTextureParticleTransform2);

			//	auto instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Universal_Ball);
			//	instanceDesc.FollowingTarget = m_pTransformCom;
			//	instanceDesc.TotalParticleTime = 1.f;
			//	instanceDesc.EachParticleLifeTime = 1.0f;
			//	instanceDesc.Particle_Power = 13;
			//	instanceDesc.TargetColor = _float4(0.98f, 0.96f, 0.41f, 0.7f);
			//	instanceDesc.TargetColor2 = _float4(0.98f, 0.96f, 0.41f, 0.7f);
			//	instanceDesc.ParticleSize = _float3(0.3f);
			//	instanceDesc.ParticleSize2 = _float3(0.1f);
			//	instanceDesc.SizeChageFrequency = 6;
			//	instanceDesc.ColorChageFrequency = 1;
			//	instanceDesc.bEmissive = true;
			//	instanceDesc.vEmissive_SBB = _float3(1);

			//	GETPARTICLE->Create_Texture_Effect_Desc(instanceDesc, m_eNowSceneNum);




			//	m_EffectAdjust++;
			//}

			if (PlayRate >= 0.4375 && PlayRate <= 0.625)
			{

				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

				//CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				//_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				//PlayerPos.y = MonsterPos.y;

				_float3 vGoalDir = (m_StartPos.XMVector() - MonsterPos.XMVector());
				_float fLength = g_pGameInstance->Easing(TYPE_SinInOut, 0, vGoalDir.Get_Lenth(), (_float)PlayRate - 0.4375f, 0.1875f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
				/*_float3 SnakePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

				_float3 EsingPos = g_pGameInstance->Easing_Vector(TYPE_SinInOut, m_AttackAnimPos.XMVector(), m_StartPos.XMVector(), (_float)PlayRate - 0.4375f, 0.1875f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, EsingPos);*/
			}

		}
		break;

		case 4:
		break;

		case 5:
		{
			float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.46f,0.44f,0.21f,Value), _float4(Value, Value*0.7f, Value, 0.9f));

			if (PlayRate > 0 && m_iAdjMovedIndex == 0)
			{
				_float3 Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				_float PosZ = XMVectorGetZ(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)) - 20.f;
				Pos.z = PosZ;
				m_StartAnimPos = Pos;

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_StartAnimPos);

				g_pGameInstance->Play3D_Sound(TEXT("JJB_Naga_1.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
				m_iAdjMovedIndex++;
				//m_fAnimmultiple = 0.2f;
			}

			if (PlayRate > 0.69014084 && PlayRate  < 0.8309859154)
			{
				_float3 SnakePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

				_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				Dir = XMVector3Normalize(SnakePos.XMVector() - XMVectorSetY(Dir, SnakePos.y));
				m_pTransformCom->Turn_Dir(Dir, 0.90f);
			}
		}
		break;

		case 6:
			break;
		case 7:

			//if (m_iAdjMovedIndex == 0 && PlayRate >= 0.5)
			//{
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_SNAKE_1, m_pTextureParticleTransform);
			//	m_iAdjMovedIndex++;
			//}

			break;
		break;
		case 8:
		{
			if (PlayRate > 0.10055865921787709f  && m_iAdjMovedIndex == 0)
			{
				m_pRaserObj->Start_BeamEffect();
				m_iAdjMovedIndex++;
			}

			if (PlayRate > 0 && PlayRate <= 0.1284916)
			{
				_float3 SnakePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				//_float fAngle = g_pGameInstance->Easing(TYPE_SinInOut, 0, 1.f, (_float)PlayRate, 0.0670391f);

				//_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				//Dir = XMVector3Normalize(SnakePos.XMVector() - XMVectorSetY(Dir, SnakePos.y));
				//m_pTransformCom->Turn_Dir(Dir, fAngle/*XMConvertToRadians(fAngle)*/);

				_float fAngle = g_pGameInstance->Easing(TYPE_SinInOut, 0, 1.f, (_float)PlayRate, 0.1284916f);

				_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				Dir = XMVector3Normalize(m_vPlayerStartPos.XMVector() - XMVectorSetY(Dir, m_vPlayerStartPos.y));
				m_pTransformCom->Turn_Dir(Dir, fAngle);

				_float vPosY = g_pGameInstance->Easing(TYPE_SinInOut, m_AttackAnimPos.y, m_AttackAnimPos.y + 20.f, (_float)PlayRate, 0.1284916f);
				_float vPosZ = g_pGameInstance->Easing(TYPE_SinInOut, m_AttackAnimPos.z, m_AttackAnimPos.z + 20.f, (_float)PlayRate, 0.1284916f);
				SnakePos.y = vPosY;
				SnakePos.z = vPosZ;

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, SnakePos);
			}

			if (PlayRate > 0.1284916 && PlayRate <= 0.86592178770)
			{
				m_bIsSpecialSkillAttack = true;
				_float fAngle = g_pGameInstance->Easing(TYPE_SinInOut, 180, 0.f, (_float)PlayRate - 0.1284916f, 0.7374301877f);

				/*_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				Dir = XMVector3Normalize(XMVectorSet(0, 0, 1.f, 1.f) - m_vPlayerStartPos.XMVector());*/

				m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0.f, 0.f), XMConvertToRadians(fAngle));
			}

			if (PlayRate > 0.86592178770 && PlayRate <= 0.95)
			{
				_float fAngle = g_pGameInstance->Easing(TYPE_SinInOut, 0, 90.f, (_float)PlayRate - 0.86592178770f, 0.0840782123f);
				m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0.f, 0.f), XMConvertToRadians(fAngle));

				_float3 SnakePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				_float vPosY = g_pGameInstance->Easing(TYPE_SinInOut, m_AttackAnimPos.y + 20.f, m_AttackAnimPos.y, (_float)PlayRate - 0.86592178770f, 0.0840782123f);
				_float vPosZ = g_pGameInstance->Easing(TYPE_SinInOut, m_AttackAnimPos.z + 20.f, m_AttackAnimPos.z, (_float)PlayRate - 0.86592178770f, 0.0840782123f);

				SnakePos.y = vPosY;
				SnakePos.z = vPosZ;

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, SnakePos);
			}


		}
			break;


		}
	}
	else
	{
		if (iNowAnimIndex == 1)
		{
			m_fAttackCoolTime = 2.f;
			m_fSpecialSillTime = 1.f;
			m_fRotTime = 0.f;
			m_iRotationRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.0f, 1.9f);
			//m_iRotationRandom = 2;
		}

		if (iNowAnimIndex == 2)
		{
			m_bIsAngleOut = false;
			m_bIsLookAt = true;
			m_bIsSpecialSkill = true;
			m_fSpecialSillTime = 2.f;
			m_fAttackCoolTime = 1.f;
			m_fRotTime = 0.f;
			m_iRotationRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.0f, 1.9f);
		}

		if (iNowAnimIndex == 3)
		{
			m_bIsBite = false;
			m_bIsAngleOut = false;
			m_fAttackCoolTime = 2.f;
			m_fSpecialSillTime = 1.f;
			m_fRotTime = 0.f;
			m_iRotationRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.0f, 1.9f);
			//m_iRotationRandom = 2;

			m_pModel->Change_AnimIndex(1);

			{

				m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSING_GODRAY, true);
				m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSING_LENSEFLARE, true);

				LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
				pLightDesc->vDiffuse = _float4(0.859375f, 0.859375f, 0.75390625f, 1.f);
				pLightDesc->vAmbient = _float4(0.859375f, 0.859375f, 0.75390625f, 1.f);
				pLightDesc->vSpecular = _float4(0.3203125f, 0.32421875f, 0.28125f, 1.f);

			}
		}

		if (iNowAnimIndex == 5)
		{
			m_bIsAtackMoveStart = true;
			m_fAttackCoolTime = 2.f;
			m_fSpecialSillTime = 1.f;
			m_fRotTime = 0.f;
			m_iRotationRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.0f, 1.9f);
			m_bIsAttack = false;
			m_fAnimmultiple = 1.f;
		}

		if (iNowAnimIndex == 7)
		{
			m_bIsAngleOut = false;
			m_fAttackCoolTime = 1.f;
			m_fRotTime = 0.f;
			//m_iRotationRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.0f, 1.9f);
			//m_pModel->Change_AnimIndex(1);
		}

		if (iNowAnimIndex == 8)
		{
			m_bIsAngleOut = false;
			m_bIsSpecialSkillAttack = false;
			m_fAttackCoolTime = 2.f;
			m_fSpecialSillTime = 12.f;
			m_fRotTime = 0.f;
			m_iRotationRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.0f, 1.9f);
			m_pModel->Change_AnimIndex(1, 1.f);
		}
	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CSnake * CSnake::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CSnake*	pInstance = NEW CSnake(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CSnake");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSnake::Clone(void * pArg)
{
	CSnake*	pInstance = NEW CSnake(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CSnake");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSnake::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pSpecialSkillCollider);
	Safe_Release(m_pMotionTrail);
	

	Safe_Release(m_pTextureParticleTransform);
	Safe_Release(m_pTextureParticleTransform1);
	Safe_Release(m_pTextureParticleTransform2);

	Safe_Release(m_pRaserObj);
}
