#include "stdafx.h"
#include "..\public\CopyMahabalasura.h"
#include "Mahabalasura.h"
#include "Mahabalasura_SpearWave.h"

#define InstanceCount 64
#define LimLightColor _float3(0.375f,0.25f,1.f)

CCopyMahabalasura::CCopyMahabalasura(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CBoss(pDevice, pDeviceContext)
{
}


CCopyMahabalasura::CCopyMahabalasura(const CCopyMahabalasura & rhs)
	: CBoss(rhs)
{
}

HRESULT CCopyMahabalasura::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CCopyMahabalasura::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));

	m_pBossObj = (CMahabalasura*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Boss));

	FAILED_CHECK(SetUp_Components());

	m_fAttackCoolTime = 4.f;

	m_bIsHit = false;

	//m_pTransformCom->Scaled_All(_float3(1.5f));

	m_iRandomIndex = rand() % InstanceCount;

	CTransform* pPlayerPosition = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = pPlayerPosition->Get_MatrixState(CTransform::STATE_POS);
	m_startPos = PlayerPos;





	{
		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.vPosition = _float3(101.721f, 33.260f, 323.105f);
		tNIMEDesc.vLookDir = _float3(0, -1, 0);

		tNIMEDesc.eMeshType = Prototype_Mesh_Half_Sheild;
		tNIMEDesc.fMaxTime_Duration = 999999999999999.f;


		tNIMEDesc.fAppearTime = 0.0001f;

		tNIMEDesc.noisingdir = _float2(0, -1).XMVector()*0.015f;

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 59;
		tNIMEDesc.iDiffuseTextureIndex = 156;
		tNIMEDesc.m_iPassIndex = 23;
		tNIMEDesc.vEmissive = _float4(0.1f, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(1, 1, 0.2f, 0);
		tNIMEDesc.vColor = _float3(1.f, 0, 0);

		tNIMEDesc.RotAxis = FollowingDir_Look;
		tNIMEDesc.RotationSpeedPerSec = 0.f;
		tNIMEDesc.OnceStartRot = 90;

		tNIMEDesc.SizeSpeed = 10.f;
		tNIMEDesc.vSizingRUL = _float3(1, 1, 1);
		tNIMEDesc.vSizieLimit = _float3(1.f, 1.f, 1.f);
		tNIMEDesc.vSize = _float3(0.01f, 0.01f, 0.01f);

		tNIMEDesc.fAlphaTestValue = 0.0f;



		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
			TAG_OP(Prototype_NonInstanceMeshEffect), &tNIMEDesc);

		m_EffectFloor = (CNonInstanceMeshEffect*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle)));
		NULL_CHECK_RETURN(m_EffectFloor, E_FAIL);



	}
	//05
	{

		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.vPosition = _float3(101.721f, 34.260f, 323.105f);
		tNIMEDesc.vLookDir = _float3(0, -1, 0);

		tNIMEDesc.eMeshType = Prototype_Mesh_Half_Sheild;
		tNIMEDesc.fMaxTime_Duration = 9999999999999999999999.f;


		tNIMEDesc.fAppearTime = 0.0001f;

		tNIMEDesc.noisingdir = _float2(0, -1).XMVector()*0.015f;

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 59;
		tNIMEDesc.iDiffuseTextureIndex = 156;
		tNIMEDesc.m_iPassIndex = 23;
		tNIMEDesc.vEmissive = _float4(0.1f, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(1, 1, 0.2f, 0);
		tNIMEDesc.vColor = _float3(1.f, 0, 0);

		tNIMEDesc.RotAxis = FollowingDir_Look;
		tNIMEDesc.RotationSpeedPerSec = 0.f;
		tNIMEDesc.OnceStartRot = 90;

		tNIMEDesc.SizeSpeed = 10.f;
		tNIMEDesc.vSizingRUL = _float3(1, 1, 0);
		tNIMEDesc.vSizieLimit = _float3(1.f, 1.f, 0.000001f);
		tNIMEDesc.vSize = _float3(0.01f, 0.01f, 0.000001f);

		tNIMEDesc.fAlphaTestValue = 0.0f;

		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
			TAG_OP(Prototype_NonInstanceMeshEffect), &tNIMEDesc);

		m_EffectWall = (CNonInstanceMeshEffect*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle)));
		NULL_CHECK_RETURN(m_EffectWall, E_FAIL);

	}




	return S_OK;
}

_int CCopyMahabalasura::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	m_fAttackCoolTime -= (_float)fDeltaTime;

	if (m_fAttackCoolTime <= 0 && !m_bIsAttack)
	{
		m_fAttackCoolTime = 20.f;

		if (m_bCopyAttackNarration == false)
		{
			_int iRandom = rand() % 3 + 1;

			wstring teampString;
			teampString = L"JJB_MrM_Shadow_attack_" + to_wstring(iRandom) + L".wav";

			g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
		}
		m_bCopyAttackNarration = !m_bCopyAttackNarration;

		m_pModel->Change_AnimIndex_ReturnTo(1, 0);

	}

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

	CTransform* BossTransform = (CTransform*)m_pBossObj->Get_Component(TAG_COM(Com_Transform));
	_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);
	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);
	for (_int i = 0; i < m_vecInstancedTransform.size(); ++i)
	{
		m_vecInstancedTransform[i].fPassedTime += (_float)fDeltaTime*pUtil->RandomFloat(0.8f,1.2f);
		m_vecInstancedTransform[i].pTransform->LookAt(XMLoadFloat3(&m_startPos));

		if (i % 2 == 0)
		{
			m_vecInstancedTransform[i].pTransform->Move_Left(fDeltaTime);
		}
		else if (i % 2 == 1)
		{
			m_vecInstancedTransform[i].pTransform->Move_Right(fDeltaTime);
		}


		if (m_vecInstancedTransform[i].bLimLightIsUp)
		{
			m_vecInstancedTransform[i].vLimLight.w += (_float)fDeltaTime;
			if (m_vecInstancedTransform[i].vLimLight.w > 1)
			{
				m_vecInstancedTransform[i].vLimLight.w = 1.f;
				
				m_vecInstancedTransform[i].bLimLightIsUp = false;
			}
			m_vecInstancedTransform[i].vLimLight =
				_float4(_float3(LimLightColor.XMVector() * m_vecInstancedTransform[i].vLimLight.w), 
					m_vecInstancedTransform[i].vLimLight.w);

		}
		else
		{
			m_vecInstancedTransform[i].vLimLight.w -= (_float)fDeltaTime;
			if (m_vecInstancedTransform[i].vLimLight.w < 0)
			{
				m_vecInstancedTransform[i].vLimLight.w = 0.f;
				m_vecInstancedTransform[i].bLimLightIsUp = true;
			}

			m_vecInstancedTransform[i].vLimLight =
				_float4(_float3(LimLightColor.XMVector() * m_vecInstancedTransform[i].vLimLight.w),
					m_vecInstancedTransform[i].vLimLight.w);
		}

	}

	//나중에 충돌로 해당 인덱스일때 다 사라지게 하고 보스를 그위치에서 나오게끔한다.
	if (g_pGameInstance->Get_DIKeyState(DIK_5 & DIS_Down))
	{
		CTransform* BossTransform = (CTransform*)m_pBossObj->Get_Component(TAG_COM(Com_Transform));
		_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);

		_float3 CopyPos = m_vecInstancedTransform[m_iRandomIndex].pTransform->Get_MatrixState(CTransform::STATE_POS);

		BossTransform->Set_MatrixState(CTransform::STATE_POS, CopyPos);
		m_pBossObj->Set_CopyOff(false);
		m_pBossObj->Set_Hit();


		m_EffectWall->Set_GonnabeDie();
		m_EffectFloor->Set_GonnabeDie();

		Set_IsDead();
		return 0;
	}
	//m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple), m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	m_pAttackCollider->Update_ConflictPassedTime(fDeltaTime);
	m_pCollider->Update_ConflictPassedTime(1);

	_Matrix mat = PlayerTransform->Get_WorldMatrix();

	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);

	m_pAttackCollider->Update_Transform(0, mat);
	m_pCollider->Update_Transform(0, mat);


	if (m_bIsAttack)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pAttackCollider));
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pCollider));

	return _int();
}

_int CCopyMahabalasura::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	if (m_bIsDead)return 0;

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);


	m_vecRenderInstanceLimLight.clear();
	m_vecRenderInstanceEmissive.clear();
	m_vecRenderInstanceTransform.clear();
	m_vecPassedInstanceTimer.clear();

	sort(m_vecColliderIndexs.begin(), m_vecColliderIndexs.end());

	_int ColliderIndex = 0;

	for (_int i = 0; i < m_vecInstancedTransform.size(); ++i)
	{

		if (m_vecColliderIndexs.size() > ColliderIndex && m_vecColliderIndexs[ColliderIndex] == i)
		{
			++ColliderIndex;
			m_vecInstancedTransform[i].bIsDead = true;
			continue;
		}
		else
		{
			m_vecRenderInstanceTransform.push_back(m_vecInstancedTransform[i].pTransform);
			m_vecRenderInstanceLimLight.push_back((m_vecInstancedTransform[i].vLimLight));
			m_vecPassedInstanceTimer.push_back(_float4(m_vecInstancedTransform[i].fPassedTime, FLT_MAX, 0, 0));
			m_vecRenderInstanceEmissive.push_back(_float4(m_vecInstancedTransform[i].vLimLight.w, m_vecInstancedTransform[i].vLimLight.w * 0.5f, m_vecInstancedTransform[i].vLimLight.w,0.f));
		}
		//_bool bState = false;
		//for (_int j = 0; j < m_vecColliderIndexs.size(); ++j)
		//{
		//	if (i == m_vecColliderIndexs[j])
		//	{
		//		bState = true;
		//		break;
		//	}
		//}
		//if(!bState)
		//	m_vecRenderInstanceTransform.push_back(m_vecInstancedTransform[i]);
	}

	ColliderIndex = 0;

	for (_int i = 0; i < m_vecInstancedTransform.size(); ++i)
	{

		if (m_vecColliderIndexs.size() > ColliderIndex && m_vecColliderIndexs[ColliderIndex] == i)
		{
			++ColliderIndex;
			continue;
		}

		m_vecInstancedTransform[i].pTransform->LookAtExceptY(XMLoadFloat3(&PlayerPos));


		_Matrix mat = m_vecInstancedTransform[i].pTransform->Get_WorldMatrix();
		mat.r[0] = XMVector3Normalize(mat.r[0]);
		mat.r[1] = XMVector3Normalize(mat.r[1]);
		mat.r[2] = XMVector3Normalize(mat.r[2]);
		mat.r[3] = mat.r[3] + m_vecInstancedTransform[i].pTransform->Get_MatrixState(CTransform::STATE_LOOK);
		m_pAttackCollider->Update_Transform(i + 1, mat);
		m_pCollider->Update_Transform(i + 1, m_vecInstancedTransform[i].pTransform->Get_WorldMatrix());

	}



	//FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SUBDISTORTION, this));
	//m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	//g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pAttackCollider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	return _int();
}

_int CCopyMahabalasura::Render()
{
	if (__super::Render() < 0)		return -1;


	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	//_float4 color = _float4(1.f, 0.734375f, 0.75234375f, 1);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &m_vecNonInstMeshDesc[3].vColor, sizeof(_float4)));


	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAppearTimer", &m_vecNonInstMeshDesc[3].fAppearTime, sizeof(_float)));

	_float2 NoiseDir = _float2(1, 0).Get_Nomalize() * 0.7f;
	_float p = 0.5;
	FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &NoiseDir, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fDistortionNoisingPushPower", &p, sizeof(_float)));
	//FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &m_vecNonInstMeshDesc[3].noisingdir, sizeof(_float2)));
	//FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fDistortionNoisingPushPower", &m_vecNonInstMeshDesc[3].fDistortionNoisingPushPower, sizeof(_float)));

	//FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAlphaTestValue", &m_vecNonInstMeshDesc[3].fAlphaTestValue, sizeof(_float)));

	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture",198));
	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", 108));

	//FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", 168));
	//FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", m_vecNonInstMeshDesc[3].NoiseTextureIndex));
	//FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", m_vecNonInstMeshDesc[3].MaskTextureIndex));
	FAILED_CHECK(m_pShaderCom->Set_Texture("g_BackBufferTexture", g_pGameInstance->Get_SRV(L"Target_ReferenceDefferred")));

	
	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 3, &m_vecRenderInstanceTransform,0,&m_vecRenderInstanceLimLight,&m_vecRenderInstanceEmissive,&m_vecPassedInstanceTimer));


	//FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	//_uint NumMaterial = m_pModel->Get_NumMaterial();

	//for (_uint i = 0; i < NumMaterial; i++)
	//{
	//	//if(i == 10)continue;

	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	//}

	return _int();
}

_int CCopyMahabalasura::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CCopyMahabalasura::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	switch (eConflictedObjCollisionType)
	{
	case Engine::CollisionType_Player:
		pConflictedObj->Take_Damage(this, 1.f, XMVectorSet(0.f, 0.f, 0.f, 0.f), false, 0.f);
		pConflictedCollider->Set_Conflicted(1.f);
		break;
	case Engine::CollisionType_PlayerWeapon:
		if (m_iRandomIndex == iMyColliderIndex - 1)
		{
			CTransform* BossTransform = (CTransform*)m_pBossObj->Get_Component(TAG_COM(Com_Transform));
			_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);

			_float3 CopyPos = m_vecInstancedTransform[m_iRandomIndex].pTransform->Get_MatrixState(CTransform::STATE_POS);

			BossTransform->Set_MatrixState(CTransform::STATE_POS, CopyPos);
			m_pBossObj->Set_CopyOff(false);
			m_pBossObj->Set_Hit();


			m_EffectWall->Set_GonnabeDie();
			m_EffectFloor->Set_GonnabeDie();

			Set_IsDead();
		}
		else
		{
			for (_int i = 0; i < m_vecColliderIndexs.size(); ++i)
			{
				if (m_vecColliderIndexs[i] == _uint(iMyColliderIndex - 1))
					return;
			}
			m_pCollider->Delete_ChildeBuffer(0, iMyColliderIndex);
			m_vecColliderIndexs.push_back(_uint(iMyColliderIndex - 1));
		}
		break;
	}

	//if (pMyCollider == m_pAttackCollider)
	//{

	//}
	//else if (pMyCollider == m_pCollider)
	//{
	//
	//}
}

_float CCopyMahabalasura::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

HRESULT CCopyMahabalasura::Ready_ParticleDesc()
{
	//0
	{
		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = -5.f;


		tNIMEDesc.eMeshType = Prototype_Mesh_Spear_SecondAttack;
		tNIMEDesc.fMaxTime_Duration = 0.3f;

		tNIMEDesc.fAppearTime = 0.15f;

		tNIMEDesc.noisingdir = _float2(-1, 0);

		tNIMEDesc.NoiseTextureIndex = 249;
		tNIMEDesc.MaskTextureIndex = 64;
		tNIMEDesc.iDiffuseTextureIndex = 365;
		tNIMEDesc.m_iPassIndex = 17;
		tNIMEDesc.vEmissive = _float4(1.f, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(0.10015625f, 0.005625f, 0.206875f, 0.2f);
		tNIMEDesc.vColor = _float3(1.0f, 1, 1);

		tNIMEDesc.RotAxis = FollowingDir_Look;
		tNIMEDesc.RotationSpeedPerSec = -480.f;
		tNIMEDesc.vSize = _float3(0.02f, 0.02f, 0.075f);
		tNIMEDesc.m_bNotDead = true;
		m_vecNonInstMeshDesc.push_back(tNIMEDesc);
	}
	//1
	{
		NONINSTNESHEFTDESC tNIMEDesc;
		tNIMEDesc.eMeshType = Prototype_Mesh_Spear_NormalEffect;
		tNIMEDesc.fMaxTime_Duration = 0.35f;
		tNIMEDesc.fAppearTime = 0.175f;

		tNIMEDesc.noisingdir = _float2(0, -1);

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 10;
		tNIMEDesc.iDiffuseTextureIndex = 365;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1.f, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(0.10015625f, 0.005625f, 0.206875f, 0.2f);
		tNIMEDesc.vColor = _float3(1.0f, 1, 1);




		tNIMEDesc.RotAxis = FollowingDir_Right;
		tNIMEDesc.RotationSpeedPerSec = -180;
		tNIMEDesc.StartRot = -0.5f;
		tNIMEDesc.vSize = _float3(1.f, -2, 2.f);
		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = 0.f;
		tNIMEDesc.m_bNotDead = true;

		m_vecNonInstMeshDesc.push_back(tNIMEDesc);

	}
	//2
	{

		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.eMeshType = Prototype_Mesh_Spear_NormalEffect;
		tNIMEDesc.fAppearTime = 0.175f;
		tNIMEDesc.fMaxTime_Duration = 0.35f;
		tNIMEDesc.noisingdir = _float2(0, -1);

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 10;
		tNIMEDesc.iDiffuseTextureIndex = 365;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1.f, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(0.10015625f, 0.005625f, 0.206875f, 0.2f);
		tNIMEDesc.vColor = _float3(1.0f, 1, 1);


		tNIMEDesc.RotAxis = FollowingDir_Right;
		tNIMEDesc.RotationSpeedPerSec = 180;
		tNIMEDesc.StartRot = -0.5f;
		tNIMEDesc.vSize = _float3(1.f, 2, 2.f);
		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = 0.f;
		tNIMEDesc.m_bNotDead = true;

		m_vecNonInstMeshDesc.push_back(tNIMEDesc);
	}
	//3
	{
		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.vPosition = _float3(101.721f, 33.260f, 323.105f);
		tNIMEDesc.vLookDir = _float3(0, -1, 0);

		tNIMEDesc.eMeshType = Prototype_Mesh_Half_Sheild;
		tNIMEDesc.fMaxTime_Duration = 999999999999999.f;


		tNIMEDesc.fAppearTime = 0.0001f;

		tNIMEDesc.noisingdir = _float2(0, -1).XMVector()*0.015f;

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 59;
		tNIMEDesc.iDiffuseTextureIndex = 156;
		tNIMEDesc.m_iPassIndex = 23;
		tNIMEDesc.vEmissive = _float4(0.1f, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(1, 1, 0.2f, 0);
		tNIMEDesc.vColor = _float3(1.f, 0, 0);

		tNIMEDesc.RotAxis = FollowingDir_Look;
		tNIMEDesc.RotationSpeedPerSec = 0.f;
		tNIMEDesc.OnceStartRot = 90;

		tNIMEDesc.SizeSpeed = 10.f;
		tNIMEDesc.vSizingRUL = _float3(1, 1, 1);
		tNIMEDesc.vSizieLimit = _float3(1.f, 1.f, 1.f);
		tNIMEDesc.vSize = _float3(0.01f, 0.01f, 0.01f);

		tNIMEDesc.fAlphaTestValue = 0.0f;

		m_vecNonInstMeshDesc.push_back(tNIMEDesc);
	}

	return S_OK;
}

HRESULT CCopyMahabalasura::Update_Particle(_double timer)
{
	return S_OK;
}

HRESULT CCopyMahabalasura::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_MahabalasurCopy), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pAttackCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(100.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pAttackCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSub), (CComponent**)&m_pCollider));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(100.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	Safe_Release(m_pGuideTransform);
	m_pGuideTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pGuideTransform, E_FAIL);

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	CGameInstance* pInstance = g_pGameInstance;
	CNonInstanceMeshEffect* pNonEffect = nullptr;

	list<CGameObject*>* pList = pInstance->Get_ObjectList_from_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead));
	if (pList != nullptr)
	{
		for (auto& pObj : *pList)
			pObj->Set_IsDead();
	}

	CMahabalasura_SpearWave::SPEARWAVEDESC tSpearWaveDesc;
	tSpearWaveDesc.fStartPos = m_pGuideTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
	tSpearWaveDesc.fLookDir = XMVector3Normalize(XMVectorSetY(m_pGuideTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK), 0));
	tSpearWaveDesc.fStartPos.y += 1.8f;
	tSpearWaveDesc.iDir = 3;

	for (_uint i = 0; i < InstanceCount; i++)
	{
		{
			ITVED tDesc;

			tDesc.bIsDead = false;
			tDesc.bLimLightIsUp = (rand() % 2)?true:false;
			tDesc.vLimLight = _float4(LimLightColor, 0);
			tDesc.vLimLight.w = pUtil->RandomFloat(0, 1);
			tDesc.vEmissive = _float4(1, 0.5f, 1, 0);
			tDesc.fPassedTime = 0;

			tDesc.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
			NULL_CHECK_RETURN(tDesc.pTransform, E_FAIL);

			tDesc.pTransform->Set_MoveSpeed(1.5f);

			CTransform* BossTransform = (CTransform*)m_pBossObj->Get_Component(TAG_COM(Com_Transform));
			_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);

			tDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(GetSingle(CUtilityMgr)->RandomFloat(-10.5f, 10.5f) + BossPos.x, BossPos.y, GetSingle(CUtilityMgr)->RandomFloat(-10.5f, 8.5f) + BossPos.z));
			//pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(rand()& 6+1 * iTemp, BossPos.y, rand() & 6 + 1 * iTemp));
			
			
			FAILED_CHECK(pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[0]));
			pNonEffect = (CNonInstanceMeshEffect*)(pInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead)));
			NULL_CHECK_RETURN(pNonEffect, E_FAIL);
			tDesc.vecEffect.push_back(pNonEffect);
			

			FAILED_CHECK(pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[0]));
			pNonEffect = (CNonInstanceMeshEffect*)(pInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead)));
			NULL_CHECK_RETURN(pNonEffect, E_FAIL);
			tDesc.vecEffect.push_back(pNonEffect);


			FAILED_CHECK(pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[1]));
			pNonEffect = (CNonInstanceMeshEffect*)(pInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead)));
			NULL_CHECK_RETURN(pNonEffect, E_FAIL);
			tDesc.vecEffect.push_back(pNonEffect);


			FAILED_CHECK(pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[2]));
			pNonEffect = (CNonInstanceMeshEffect*)(pInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead)));
			NULL_CHECK_RETURN(pNonEffect, E_FAIL);
			tDesc.vecEffect.push_back(pNonEffect);
			

			FAILED_CHECK(pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead),	TAG_OP(Prototype_Object_Boss_MahabalasuraSpearWave), &tSpearWaveDesc));
			tDesc.pSpearWaveEffect = (CMahabalasura_SpearWave*)(pInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_ParticleNoDead)));
			NULL_CHECK_RETURN(tDesc.pSpearWaveEffect, E_FAIL);

			m_vecInstancedTransform.push_back(tDesc);
		}
		//Attack충돌체
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.3f, 1.6f, 1);
		FAILED_CHECK(m_pAttackCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pAttackCollider->Set_ParantBuffer();

		//피격 충돌체
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(3.5f, 3.5f, 3.5f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1);
		FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		m_pCollider->Set_ParantBuffer();
	}

	CModelInstance::MODELINSTDESC tModelIntDsec;
	tModelIntDsec.m_pTargetModel = m_pModel;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_ModelInstance_64), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tModelIntDsec));


	//CTransform::TRANSFORMDESC tDesc = {};

	//tDesc.fMovePerSec = 5;
	//tDesc.fRotationPerSec = XMConvertToRadians(60);
	//tDesc.fScalingPerSec = 1;
	//tDesc.vPivot = _float3(0, 0, 0);

	//FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CCopyMahabalasura::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	{

		m_iAdjMovedIndex = 0;
		m_iEffectAdjustIndex = 0;
	}


	if (PlayRate <= 0.98)
	{

		switch (iNowAnimIndex)
		{
		case 0:
			break;
		case 1:
		{
			CGameInstance* pInstance = g_pGameInstance;

			if (m_iAdjMovedIndex == 0 && PlayRate > 0.203644859)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Trishul_Swing_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_bIsAttack = true;
				++m_iAdjMovedIndex;
			}
			if (m_iEffectAdjustIndex == 0 && PlayRate > 0.2f)
			{

				_Vector PlayerPos = g_pGameInstance->Get_TargetPostion_Vector(PLV_PLAYER);

				for (_uint i = 0; i < m_vecInstancedTransform.size(); ++i)
				{
					if(m_vecInstancedTransform[i].bIsDead) continue;

					m_pGuideTransform->Set_MatrixState(CTransform::STATE_POS, m_vecInstancedTransform[i].pTransform->Get_MatrixState(CTransform::STATE_POS));
					m_pGuideTransform->LookAt(PlayerPos);



					m_vecNonInstMeshDesc[0].vPosition = m_pGuideTransform->Get_MatrixState(CTransform::STATE_POS)
						+ m_pGuideTransform->Get_MatrixState(CTransform::STATE_UP) * 1.0f
						+ m_pGuideTransform->Get_MatrixState(CTransform::STATE_LOOK) * 10.2f;
					-m_pGuideTransform->Get_MatrixState(CTransform::STATE_RIGHT) * 0.5f;

					m_vecNonInstMeshDesc[0].vLookDir = XMVector3Normalize((m_pGuideTransform->Get_MatrixState(CTransform::STATE_POS)
						+m_pGuideTransform->Get_MatrixState(CTransform::STATE_LOOK)
						+m_pGuideTransform->Get_MatrixState(CTransform::STATE_UP) * 2.5f
						+m_pGuideTransform->Get_MatrixState(CTransform::STATE_RIGHT) * 1.25f)
						- m_vecNonInstMeshDesc[0].vPosition.XMVector());

					m_vecInstancedTransform[i].vecEffect[0]->ReInitialize(m_vecNonInstMeshDesc[0].vPosition, m_vecNonInstMeshDesc[0].vLookDir);



					m_vecNonInstMeshDesc[0].vLookDir =
						XMVector3Normalize((m_pGuideTransform->Get_MatrixState(CTransform::STATE_POS)
							+ m_pGuideTransform->Get_MatrixState(CTransform::STATE_LOOK)
							+ m_pGuideTransform->Get_MatrixState(CTransform::STATE_UP) * 2.5f
							- m_pGuideTransform->Get_MatrixState(CTransform::STATE_RIGHT) * 1.25f)
							- m_vecNonInstMeshDesc[0].vPosition.XMVector());

					m_vecInstancedTransform[i].vecEffect[1]->ReInitialize(m_vecNonInstMeshDesc[0].vPosition, m_vecNonInstMeshDesc[0].vLookDir);



				}

				m_iEffectAdjustIndex++;
			}


			if (m_iAdjMovedIndex == 1 && PlayRate > 0.2803738)
			{
				m_bIsAttack = false;;
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 2 && PlayRate > 0.392523)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Trishul_Swing_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_bIsAttack = true;

				_Vector PlayerPos = g_pGameInstance->Get_TargetPostion_Vector(PLV_PLAYER);

				for (_uint i = 0; i < m_vecInstancedTransform.size(); ++i)
				{
					if (m_vecInstancedTransform[i].bIsDead) continue;

					m_pGuideTransform->Set_MatrixState(CTransform::STATE_POS, m_vecInstancedTransform[i].pTransform->Get_MatrixState(CTransform::STATE_POS));
					m_pGuideTransform->LookAt(PlayerPos);

					m_vecNonInstMeshDesc[1].vPosition = m_pGuideTransform->Get_MatrixState(CTransform::STATE_POS) +
						m_pGuideTransform->Get_MatrixState(CTransform::STATE_UP) * 3.5f
						+ m_pGuideTransform->Get_MatrixState(CTransform::STATE_LOOK) * 4.5f;
					m_vecNonInstMeshDesc[1].vLookDir = -m_pGuideTransform->Get_MatrixState(CTransform::STATE_LOOK);


					m_vecInstancedTransform[i].vecEffect[2]->ReInitialize(m_vecNonInstMeshDesc[1].vPosition, m_vecNonInstMeshDesc[1].vLookDir);



					m_vecNonInstMeshDesc[2].vPosition = m_pGuideTransform->Get_MatrixState(CTransform::STATE_POS) +
						m_pGuideTransform->Get_MatrixState(CTransform::STATE_UP) * 1.f
						+ m_pGuideTransform->Get_MatrixState(CTransform::STATE_LOOK) * 4.5f;
					m_vecNonInstMeshDesc[2].vLookDir = -m_pGuideTransform->Get_MatrixState(CTransform::STATE_LOOK);

					m_vecInstancedTransform[i].vecEffect[3]->ReInitialize(m_vecNonInstMeshDesc[2].vPosition, m_vecNonInstMeshDesc[2].vLookDir);

				}

				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 3 && PlayRate > 0.47663551)
			{
				m_bIsAttack = false;
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 4 && PlayRate > 0.56074766)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Trishul_Swing_03.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_bIsAttack = true;

				_Vector PlayerPos = g_pGameInstance->Get_TargetPostion_Vector(PLV_PLAYER);

				for (_uint i = 0; i < m_vecInstancedTransform.size(); ++i)
				{
					if (m_vecInstancedTransform[i].bIsDead) continue;

					m_pGuideTransform->Set_MatrixState(CTransform::STATE_POS, m_vecInstancedTransform[i].pTransform->Get_MatrixState(CTransform::STATE_POS));
					m_pGuideTransform->LookAt(PlayerPos);


					 
					_float3 vPosition = m_pGuideTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
					_float3 vLookDir = XMVector3Normalize(XMVectorSetY(m_pGuideTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK), 0));
					vPosition.y += 1.8f;
					FAILED_CHECK(m_vecInstancedTransform[i].pSpearWaveEffect->ReInitialize(vPosition, vLookDir));
				}

				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 5 && PlayRate > 0.6074766)
			{
				m_bIsAttack = false;
				++m_iAdjMovedIndex;
			}
			break;

		}
		case 5:

			break;


		}
	}
	else
	{
		if (iNowAnimIndex == 0)
		{
		}
		if (iNowAnimIndex == 1)
		{
			m_bIsAttack = false;
			m_fAttackCoolTime = 4.f;
		}
	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CCopyMahabalasura * CCopyMahabalasura::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CCopyMahabalasura*	pInstance = NEW CCopyMahabalasura(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CCopyMahabalasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCopyMahabalasura::Clone(void * pArg)
{
	CCopyMahabalasura*	pInstance = NEW CCopyMahabalasura(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CCopyMahabalasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCopyMahabalasura::Free()
{
	__super::Free();



	//Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pModelInstance);
	Safe_Release(m_pAttackCollider);
	Safe_Release(m_pCollider);
	Safe_Release(m_pGuideTransform);
	

	for (auto& pTransform : m_vecInstancedTransform)
		Safe_Release(pTransform.pTransform);
	m_vecInstancedTransform.clear();
}
