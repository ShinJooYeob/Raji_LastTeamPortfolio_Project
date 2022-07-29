#include "stdafx.h"
#include "..\public\Rangda.h"
#include "Rangda_MagicCircle.h"

CRangda::CRangda(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CBoss(pDevice, pDeviceContext)
{
}

CRangda::CRangda(const CRangda & rhs)
	: CBoss(rhs)
{
}

HRESULT CRangda::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CRangda::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_vStartPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));

	m_pTransformCom->Scaled_All(_float3(5.f, 5.f, 5.f));

	m_pModel->Change_AnimIndex(0);

	m_fAttackCoolTime = 3.f;
	m_fSkillCoolTime = 8.f;
	m_bIsHit = false;

	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum,
		TEXT("Layer_Player"));

	for (_int i = 0; i < 8; ++i)
	{
		m_vFingerPoss[i] = Get_FingerPos(i);
	}

	m_fHP = 32.f;
	m_fMaxHP = 32.f;

	_int iRandom = rand() % 11;

	wstring teampString;
	teampString = L"JJB_Narration" + to_wstring(iRandom) + L".wav";

	g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

	return S_OK;
}

_int CRangda::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	m_pDissolve->Update_Dissolving(fDeltaTime);

	m_fAttackCoolTime -= (_float)fDeltaTime;
	m_fSkillCoolTime -= (_float)fDeltaTime;


	if (m_bIsLookAt)
	{
		CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
		_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

		PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;
		_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		Dir = XMVector3Normalize(PlayerPos.XMVector() - XMVectorSetY(Dir, PlayerPos.y));
		m_pTransformCom->Turn_Dir(Dir, 0.90f);

		//m_fRange = XMVectorGetZ(XMVector3Length(XMLoadFloat3(&Pos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));
	}

	/*if (g_pGameInstance->Get_DIKeyState(DIK_M)& DIS_Down)
		m_bIsHit = true;*/

	if (g_pGameInstance->Get_DIKeyState(DIK_M)& DIS_Down)
		m_bIsHit = true;

	if (g_pGameInstance->Get_DIKeyState(DIK_N)& DIS_Down)
	{
		m_fTestHPIndex += 0.2f;
	}
	//if (m_fTestHPIndex >= 1.4f)
	//	m_bIsHalf = true;

	//m_bIsHalf = true;

	if (m_iMaterialCount == 7)
		m_bIsHalf = true;


	// #DEBUG
	if (KEYDOWN(DIK_B))
	{
	
		//		m_fAttackCoolTime = 0;
		//		m_fSkillCoolTime = 1;

		m_fAttackCoolTime = 1;
		m_fSkillCoolTime = 0;
		m_bIsAttack = false;
		m_bIsHit = false;

		m_bIsHalf = false;
	}
	if (KEYDOWN(DIK_C))
	{

//		m_fAttackCoolTime = 0;
//		m_fSkillCoolTime = 1;

		m_fAttackCoolTime = 1;
		m_fSkillCoolTime = 0;

		m_bIsAttack = false;
		m_bIsHit = false;

		m_bIsHalf = true;
	}


	//맞았을때
	if (m_bIsHit)
	{
		m_bIsHit = false;
		m_bIsAttack = true;
		if(m_fHP > 0)
			m_pModel->Change_AnimIndex_UntilNReturn(2, 3, 0);
		else if (m_fHP <= 0)
		{
			m_pModel->Change_AnimIndex_ReturnTo(2, 1);
		}

		if (m_iMaterialCount - 3 < 8)
		{

			_float3 vPos = Get_FingerPos(m_iMaterialCount - 3);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Finger"), TAG_OP(Prototype_Object_Boss_Rangda_Finger), &vPos);
		}

		++m_iMaterialCount;

		m_vecFinger.push_back((_uint)m_iMaterialCount);
	}
	//일반 공격
	else if (m_fAttackCoolTime <= 0 && !m_bIsAttack && !m_bIsHit)
	{
		/*_int iRandom = rand() % 11;

		wstring teampString;
		teampString = L"JJB_Narration" + to_wstring(iRandom) + L".wav";

		g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
		*/

		m_bIsAttack = true;

		if (m_bIsHalf)
		{
			m_pModel->Change_AnimIndex_ReturnTo(7, 0);
		}
		else
		{
			m_pModel->Change_AnimIndex_ReturnTo(6, 0);
		}
	}
	//스킬 공격
	else if (m_fSkillCoolTime <= 0 && !m_bIsAttack && !m_bIsHit)
	{
		_int iRandom = rand() % 11;

		wstring teampString;
		teampString = L"JJB_Narration" + to_wstring(iRandom) + L".wav";

		g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

		m_bIsAttack = true;

		if (m_bIsHalf)
			m_pModel->Change_AnimIndex_ReturnTo(5, 0);
		else
			m_pModel->Change_AnimIndex_ReturnTo(4, 0);

	}


	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple* m_fTestHPIndex), m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	//Left
	m_pHand_L_Collider->Update_ConflictPassedTime(fDeltaTime);
	m_pHand_L_Collider->Update_Transform(0, m_LeftAttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed());

	//Right
	m_pHand_R_Collider->Update_ConflictPassedTime(fDeltaTime);
	m_pHand_R_Collider->Update_Transform(0, m_RightAttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed());

	//Scream
	m_pScreamCollider->Update_ConflictPassedTime(fDeltaTime);
	m_pScreamCollider->Update_Transform(0, m_ScreamAttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed());

	if (m_bIsNailAttack && !m_bIsHalf)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pHand_L_Collider));
	}
	if (m_bIsNailAttack && m_bIsHalf)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pHand_R_Collider));
	}

	if (m_bIsNailHit && !m_bIsHalf)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pHand_L_Collider));
	}
	else if (m_bIsNailHit && m_bIsHalf)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pHand_R_Collider));
	}

	if(m_bIsScreamAttack)
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pScreamCollider));

	if(m_bIsDissolveStart)
		m_pDissolve->Set_DissolveOn(false, 9.4f);
	return _int();
}

_int CRangda::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pHand_L_Collider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pHand_R_Collider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pScreamCollider));

	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	//g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);

	return _int();
}

_int CRangda::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolve->Render_SkipMtrl(3, &m_vecFinger));

	//_uint NumMaterial = m_pModel->Get_NumMaterial();

	//for (_uint i = 0; i < NumMaterial; i++)
	//{
	//	//if(i == 10)continue;

	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

	//	if(i > 3 && i <= _uint(m_iMaterialCount) )
	//		continue;

	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	//}

	return _int();
}

_int CRangda::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CRangda::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CRangda::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	m_fHP -= fDamageAmount;

	if (m_fHP == 28)
		m_bIsHit = true;
	else if(m_fHP == 24)
		m_bIsHit = true;
	else if (m_fHP == 20)
		m_bIsHit = true;
	else if (m_fHP == 16)
		m_bIsHit = true;
	else if (m_fHP == 12)
		m_bIsHit = true;
	else if (m_fHP == 8)
		m_bIsHit = true;
	else if (m_fHP == 4)
		m_bIsHit = true;
	else if (m_fHP == 0)
		m_bIsHit = true;


	return _float();
}

_fVector CRangda::Get_BonePos(const char * pBoneName)
{
	_Matrix BoneMatrix = m_pModel->Get_BoneMatrix(pBoneName);
	_Matrix TransformMatrix = BoneMatrix * m_pTransformCom->Get_WorldMatrix();
	_Vector vPos, vRot, vScale;
	XMMatrixDecompose(&vScale, &vRot, &vPos, TransformMatrix);

	return vPos;
}

_fMatrix CRangda::Get_BoneMatrix(const char * pBoneName)
{
	_Matrix BoneMatrix = m_pModel->Get_BoneMatrix(pBoneName);
	_Matrix TransformMatrix = BoneMatrix * m_pTransformCom->Get_WorldMatrix();

	return TransformMatrix;
}

_float3 CRangda::Get_FingerPos(_int Num)
{
	_float3 BonePos;

	switch (Num)
	{
	case 0:
		//_float3 HandPos = (MonsterPos.XMVector() + m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 8.6f);
		BonePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(-0.780f, 1.485f, -10.206f, 0.f);
		return BonePos;
		break;
	case 1:
		BonePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(-0.390f, 1.485f, -10.871f, 0.f);
		return BonePos;
		break;
	case 2:
		BonePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(+0.435f, 1.485f, -10.871f, 0.f);
		return BonePos;
		break;
	case 3:
		BonePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(+1.455f, 1.485f, -10.451f, 0.f);
		return BonePos;
		break;
	case 4:
		BonePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(+0.615f, 1.485f, -10.966f, 0.f);
		return BonePos;
		break;
	case 5:
		BonePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(+0.315f, 1.485f, -11.721f, 0.f);
		return BonePos;
		break;
	case 6:
		BonePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(-0.570f, 1.485f, -11.736f, 0.f);
		return BonePos;
		break;
	case 7:
		BonePos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(-1.200f, 1.485f, -10.761f, 0.f);
		return BonePos;
		break;
	}
	return _float3();
}




HRESULT CRangda::Ready_ParticleDesc()
{
	m_pTextureParticleTransform_R = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform_R, E_FAIL);
	m_pTextureParticleTransform_L = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform_L, E_FAIL);
	m_pTextureParticleTransform_Screen = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform_Screen, E_FAIL);
	m_pTextureParticleTransform_Player = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform_Player, E_FAIL);
	

	return S_OK;

}

HRESULT CRangda::Update_Particle(_double timer)
{
	_Matrix mat_Trans = m_pTransformCom->Get_WorldMatrix();

	mat_Trans.r[0] = XMVector3Normalize(mat_Trans.r[0]);
	mat_Trans.r[1] = XMVector3Normalize(mat_Trans.r[1]);
	mat_Trans.r[2] = XMVector3Normalize(mat_Trans.r[2]);

	mat_Trans.r[3] = m_pHand_R_Collider->Get_ColliderPosition(0).XMVector();
	m_pTextureParticleTransform_R->Set_Matrix(mat_Trans);
	mat_Trans.r[3] = m_pHand_L_Collider->Get_ColliderPosition(0).XMVector();
	m_pTextureParticleTransform_L->Set_Matrix(mat_Trans);
	mat_Trans.r[3] = m_pScreamCollider->Get_ColliderPosition(0).XMVector();
	m_pTextureParticleTransform_Screen->Set_Matrix(mat_Trans);


	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	m_pTextureParticleTransform_Player->Set_Matrix(PlayerTransform->Get_WorldMatrix());
	



	return S_OK;


}

HRESULT CRangda::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_Rangda), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));

	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	DissolveDesc.pShader = m_pShaderCom;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pHand_L_Collider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(15.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pHand_L_Collider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_l", _float3(1), _float3(0), _float3(674.453f, 60.4958f, -66.9747f));
	m_LeftAttachedDesc = tAttachedDesc;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSub), (CComponent**)&m_pHand_R_Collider));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(15.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pHand_R_Collider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_r", _float3(1), _float3(0), _float3(-674.453f, 60.4957f, -66.9735f));
	m_RightAttachedDesc = tAttachedDesc;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSubSub), (CComponent**)&m_pScreamCollider));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(100.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pScreamCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "head", _float3(1), _float3(0), _float3(0.000036f, 47.287f, -731.925f));
	m_ScreamAttachedDesc = tAttachedDesc;
	
	return S_OK;
}

HRESULT CRangda::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	{
		m_iAdjMovedIndex = 0;
		m_EffectAdjust = 0;
	}


	if (PlayRate <= 0.98)
	{
		switch (iNowAnimIndex)
		{
		case 0:
			if (PlayRate > 0.f)
			{
				m_bIsAttack = false;

			}

			break;
		case 1://애니메이션 인덱스마다 잡아주면 됨
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.0) // 이렇게 되면 이전 애니메이션에서 보간되는 시간 끝나자 마자 바로 들어옴
			{
				m_bIsDissolveStart = true;

				if (PlayRate > 0 && m_iAdjMovedIndex == 0 && m_fHP <= 0)
				{
					g_pGameInstance->Play3D_Sound(TEXT("JJB_Rangda_Dead.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

					++m_iAdjMovedIndex;
				}

				if (PlayRate > 0.666666666666 && m_iAdjMovedIndex == 1 && m_fHP <= 0)
				{
					g_pGameInstance->Play3D_Sound(TEXT("JJB_Rangda_Scream_01.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

					++m_iAdjMovedIndex;
				}

				m_iAdjMovedIndex++;
			}
			break;
		case 2:
		{
			if (PlayRate <= 0 && m_iAdjMovedIndex == 0 && m_fHP > 0)
			{
				_int iRandom = rand() % 6 + 1;

				wstring teampString;
				teampString = L"JJB_Rangda_Get_Hit_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0 && m_iAdjMovedIndex == 1 && m_fHP > 0)
			{
				_int iRandom = rand() % 6;

				wstring teampString;
				teampString = L"JJB_Hit_narration" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;
			}


			if (PlayRate <= 0.97f)
			{
				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

				_float3 vGoalDir = (m_vStartPos.XMVector() - MonsterPos.XMVector());
				_float fLength = g_pGameInstance->Easing(TYPE_SinInOut, 0, vGoalDir.Get_Lenth(),  (_float)PlayRate, 0.97f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
			}
		}
		break;

		case 3:
		{
			m_bIsLookAt = false;

			if (PlayRate > 0.465116279 && m_iAdjMovedIndex == 0)
			{
				g_pGameInstance->Play3D_Sound(TEXT("JJB_Rangda_Scream_02.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_bIsScreamAttack = true;
				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.8527131 && m_iAdjMovedIndex == 1)
			{
				m_bIsScreamAttack = false;
				++m_iAdjMovedIndex;
			}


		}
		break;

		case 4:
		{
			CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
			_float3 Pos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

			if (m_iAdjMovedIndex == 0 && PlayRate > 0.2987012)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));
				m_iAdjMovedIndex++;
			}
			if (m_iAdjMovedIndex == 1 && PlayRate > 0.4545454)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));
				m_iAdjMovedIndex++;
			}
			if (m_iAdjMovedIndex == 2 && PlayRate > 0.629870129)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				//		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));
				m_iAdjMovedIndex++;
			}
			if (m_iAdjMovedIndex == 3 && PlayRate > 0.814935064)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				//	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));
				m_iAdjMovedIndex++;
			}



			if (m_EffectAdjust == 0 && PlayRate > 0.24)
			{
				m_EffectAdjust++;
			}

			if (m_EffectAdjust == 1 && PlayRate > 0.2987012)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_4, m_pTextureParticleTransform_Player);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_3, m_pTextureParticleTransform_Player);
				m_EffectAdjust++;
			}

			if (m_EffectAdjust == 2 && PlayRate > 0.4)
			{

				m_EffectAdjust++;
			}

			if (m_EffectAdjust == 3 && PlayRate > 0.4545454)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_4, m_pTextureParticleTransform_Player);

				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_3, m_pTextureParticleTransform_Player);
				m_EffectAdjust++;
			}

			if (m_EffectAdjust == 4 && PlayRate > 0.57)
			{

				m_EffectAdjust++;
			}



			if (m_EffectAdjust == 5 && PlayRate > 0.629870129)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_4, m_pTextureParticleTransform_Player);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_3, m_pTextureParticleTransform_Player);
				m_EffectAdjust++;

			}

			if (m_EffectAdjust == 6 && PlayRate > 0.76)
			{

				m_EffectAdjust++;
			}

			if (m_EffectAdjust == 7 && PlayRate > 0.814935064)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_4, m_pTextureParticleTransform_Player);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_3, m_pTextureParticleTransform_Player);
				m_EffectAdjust++;

			}




		}

		break;

		case 5:
		{
			CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
			_float3 Pos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

			if (m_iAdjMovedIndex == 0 && PlayRate > 0.223926380)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));

				CRangda_MagicCircle* Obj = (CRangda_MagicCircle*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_TestEffect));
				Obj->ChangeScaledAndTexture();
				m_iAdjMovedIndex++;
			}
			if (m_iAdjMovedIndex == 1 && PlayRate > 0.3067484)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));

				CRangda_MagicCircle* Obj = (CRangda_MagicCircle*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_TestEffect));
				Obj->ChangeScaledAndTexture();
				m_iAdjMovedIndex++;
			}
			if (m_iAdjMovedIndex == 2 && PlayRate > 0.401840490)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));

				CRangda_MagicCircle* Obj = (CRangda_MagicCircle*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_TestEffect));
				Obj->ChangeScaledAndTexture();
				m_iAdjMovedIndex++;
			}
			if (m_iAdjMovedIndex == 3 && PlayRate > 0.503067484)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));

				CRangda_MagicCircle* Obj = (CRangda_MagicCircle*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_TestEffect));
				Obj->ChangeScaledAndTexture();
				m_iAdjMovedIndex++;
			}
			if (m_iAdjMovedIndex == 4 && PlayRate > 0.598159509)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));

				CRangda_MagicCircle* Obj = (CRangda_MagicCircle*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_TestEffect));
				Obj->ChangeScaledAndTexture();
				m_iAdjMovedIndex++;
			}

			if (m_iAdjMovedIndex == 5 && PlayRate > 0.68404907975)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));

				CRangda_MagicCircle* Obj = (CRangda_MagicCircle*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_TestEffect));
				Obj->ChangeScaledAndTexture();
				m_iAdjMovedIndex++;
			}

			if (m_iAdjMovedIndex == 6 && PlayRate > 0.7760736196)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));

				CRangda_MagicCircle* Obj = (CRangda_MagicCircle*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_TestEffect));
				Obj->ChangeScaledAndTexture();
				m_iAdjMovedIndex++;
			}

			if (m_iAdjMovedIndex == 7 && PlayRate > 0.8773006134969)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Wave_Rangda_handswipe_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_TestEffect), TAG_OP(Prototype_Object_Effect_MagicCircle), &Pos));

				CRangda_MagicCircle* Obj = (CRangda_MagicCircle*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_TestEffect));
				Obj->ChangeScaledAndTexture();
				m_iAdjMovedIndex++;
			}
		}



		break;

		case 6:
			if (PlayRate < 0.1674876847290640)
			{
				if (m_iAdjMovedIndex == 0)
				{
					_int iRandom = rand() % 3 + 1;

					wstring teampString;
					teampString = L"JJB_Wave_Rangda_HandMovement_0" + to_wstring(iRandom) + L".wav";

					g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

					++m_iAdjMovedIndex;
				}
				m_bIsLookAt = false;
				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				_float3 HandPos = (MonsterPos.XMVector() + m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 8.6f);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				PlayerPos.y = HandPos.y = MonsterPos.y;

				_float3 vGoalDir = (PlayerPos.XMVector() - HandPos.XMVector());
				_float fLength = g_pGameInstance->Easing(TYPE_SinInOut, 0, vGoalDir.Get_Lenth(), (_float)PlayRate, 0.1674876847290640f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));

			}

			if (m_iAdjMovedIndex == 1 && PlayRate < 0.310344827586206)
			{
				m_fAnimmultiple = 1.8f;

				m_bIsLookAt = true;
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 2 && PlayRate < 0.330049261083743)
			{

				m_fAnimmultiple = 1.f;
				m_bIsLookAt = false;
				++m_iAdjMovedIndex;

			}

			if (m_iAdjMovedIndex == 3 && PlayRate > 0.216748768)
			{
				m_bIsNailAttack = true;

				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 4 && PlayRate > 0.26600985)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Rangda_Slash_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;

			}

			if (m_iAdjMovedIndex == 5 && PlayRate > 0.33004926)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Rangda_Ground_Impact_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 6 && PlayRate > 0.359605911)
			{
				m_bIsNailAttack = false;

				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 7 && PlayRate > 0.37931034)
			{
				m_bIsNailHit = true;
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 8 && PlayRate > 0.842364532)
			{
				m_bIsNailHit = false;
				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.8522167487)
			{
				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

				_float3 vGoalDir = (m_vStartPos.XMVector() - MonsterPos.XMVector());
				_float fLength = g_pGameInstance->Easing(TYPE_SinInOut, 0, vGoalDir.Get_Lenth(), (_float)PlayRate - 0.8522167487f, 0.11779f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
			}

			// EFFECT
			if (m_EffectAdjust == 0 && PlayRate > 0.1f)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_1, m_pTextureParticleTransform_L);
				m_EffectAdjust++;
			}

			if (m_EffectAdjust == 1 && PlayRate > 0.33004926)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_0, m_pTransformCom);

				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_2, m_pTransformCom);

				m_EffectAdjust++;

			}
		



			break;

		case 7:
			if (PlayRate < 0.1674876847290640)
			{
				if (m_iAdjMovedIndex == 0)
				{
					_int iRandom = rand() % 3 + 1;

					wstring teampString;
					teampString = L"JJB_Wave_Rangda_HandMovement_0" + to_wstring(iRandom) + L".wav";

					g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

					++m_iAdjMovedIndex;
				}

				m_bIsLookAt = false;
				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				_float3 HandPos = (MonsterPos.XMVector() + m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 8.6f);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				PlayerPos.y = HandPos.y = MonsterPos.y;

				_float3 vGoalDir = (PlayerPos.XMVector() - HandPos.XMVector());
				_float fLength = g_pGameInstance->Easing(TYPE_SinInOut, 0, vGoalDir.Get_Lenth(), (_float)PlayRate, 0.1674876847290640f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));

			}
			if (m_iAdjMovedIndex == 1 && PlayRate < 0.310344827586206)
			{
				m_fAnimmultiple = 1.8f;

				m_bIsLookAt = true;
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 2 && PlayRate < 0.330049261083743)
			{
				m_fAnimmultiple = 1.f;
				m_bIsLookAt = false;
				++m_iAdjMovedIndex;

			}

			if (m_iAdjMovedIndex == 3 && PlayRate > 0.216748768)
			{
				m_bIsNailAttack = true;

				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 4 && PlayRate > 0.26600985)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Rangda_Slash_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;

			}

			if (m_iAdjMovedIndex == 5 && PlayRate > 0.33004926)
			{
				_int iRandom = rand() % 3 + 1;

				wstring teampString;
				teampString = L"JJB_Rangda_Ground_Impact_0" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 6 && PlayRate > 0.359605911)
			{
				m_bIsNailAttack = false;

				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 7 && PlayRate > 0.37931034)
			{
				m_bIsNailHit = true;
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 8 && PlayRate > 0.842364532)
			{
				m_bIsNailHit = false;
				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.8522167487)
			{
				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				    
				_float3 vGoalDir = (m_vStartPos.XMVector() - MonsterPos.XMVector());
				_float fLength = g_pGameInstance->Easing(TYPE_SinInOut, 0, vGoalDir.Get_Lenth(), (_float)PlayRate - 0.8522167487f, 0.11779f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
			}

			// EFFECT
			if (m_EffectAdjust == 0 && PlayRate > 0.1f)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_1, m_pTextureParticleTransform_R);
				m_EffectAdjust++;
			}

			if (m_EffectAdjust == 1 && PlayRate > 0.33004926)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_0, m_pTransformCom);

				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Rangda_2, m_pTransformCom);

				m_EffectAdjust++;

			}


			break;
		}
	}
	else
	{
		if (iNowAnimIndex == 1)
		{
			Set_IsDead();
		}
		if (iNowAnimIndex == 6 || iNowAnimIndex == 7)
		{
			m_bIsLookAt = true;
			_int iRandom = rand() % 3+1 ;
			m_fAttackCoolTime = (_float)iRandom - m_fTestHPIndex;
			m_fAttackCoolTime = 2.2f - m_fTestHPIndex;
		}

		if (iNowAnimIndex == 5 || iNowAnimIndex == 4)
		{
			m_fSkillCoolTime = 8.f - m_fTestHPIndex;
			m_fAttackCoolTime = 3.f - m_fTestHPIndex;
		}

		if (iNowAnimIndex == 3)
		{
			m_bIsLookAt = true;
			m_fSkillCoolTime = 8.f - m_fTestHPIndex;
			m_fAttackCoolTime = 2.f - m_fTestHPIndex;
		}
	}


	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CRangda * CRangda::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRangda*	pInstance = NEW CRangda(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CRangda");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRangda::Clone(void * pArg)
{
	CRangda*	pInstance = NEW CRangda(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CRangda");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRangda::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolve);

	Safe_Release(m_pHand_L_Collider);
	Safe_Release(m_pHand_R_Collider);
	Safe_Release(m_pScreamCollider);
	
	Safe_Release(m_pTextureParticleTransform_R);
	Safe_Release(m_pTextureParticleTransform_L);
	Safe_Release(m_pTextureParticleTransform_Screen);
	Safe_Release(m_pTextureParticleTransform_Player);
}
