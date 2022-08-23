#include "stdafx.h"
#include "..\public\Snake_Poison_Raser.h"
#include "Camera_Main.h"

CSnake_Poison_Raser::CSnake_Poison_Raser(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CSnake_Poison_Raser::CSnake_Poison_Raser(const CSnake_Poison_Raser & rhs)
	:CGameObject(rhs)
{
}

HRESULT CSnake_Poison_Raser::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CSnake_Poison_Raser::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	ZeroMemory(&m_RaserDesc, sizeof(SNAKERASERDESC));
	if (nullptr != pArg)
	{
		memcpy(&m_RaserDesc, pArg, sizeof(SNAKERASERDESC));
	}

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(Ready_MeshDesc());

	m_fCurTime_Duration = 99999999999999999999.f;

	//JH
	m_fDelayTime = 0.f;
	return S_OK;
}

_int CSnake_Poison_Raser::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	if (m_fCurTime_Duration > m_fMaxTime_Duration) return 0;

	m_fCurTime_Duration += (_float)fDeltaTime;


	//JH
	CGameObject* pPlayer = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player));
	_Vector vPlayerPos = static_cast<CTransform*>(pPlayer->Get_Component(Tag_Component(Com_Transform)))->Get_MatrixState(CTransform::STATE_POS);
	_Vector vBeamPos = m_pExplosionTransformCom->Get_MatrixState(CTransform::STATE_POS);
	_float fDist = XMVectorGetX(XMVector3Length(vBeamPos - vPlayerPos));
	m_fDelayTime -= (_float)fDeltaTime;
	if (fDist <= 20.f)
	{
		if (0.f >= m_fDelayTime)
		{
			m_fDelayTime = 0.5f;
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(55.f, 4.f, 0.5f, false);
		}
	}
	//

	return _int();
}

_int CSnake_Poison_Raser::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	if (m_fCurTime_Duration > m_fMaxTime_Duration) return 0;

	Update_AttachMatrix();

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	//FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	//81.5366751435f;
	_float Angle = g_pGameInstance->Easing(TYPE_Linear,75.f, 81.5366751435f,m_fCurTime_Duration, m_fMaxTime_Duration);

	m_fAttachedMatrix =
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0), XMConvertToRadians(0))
		*XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0), XMConvertToRadians(Angle))
		* XMMatrixRotationAxis(XMVectorSet(0, 0.f, 1.f, 0), XMConvertToRadians(180))
		* m_fAttachedMatrix.XMatrix();

	m_pLazerInsideTransformCom->Set_Matrix(m_fAttachedMatrix.XMatrix());
	m_pLazerInsideTransformCom->Move_Forward(3.f);
	m_pLazerInsideTransformCom->Scaled_All(_float3(2.88f, 2.88f, 0.85f));
	m_tLazerInsideMeshDesc.OnceStartRot += (_float)fDeltaTime * m_tLazerInsideMeshDesc.RotationSpeedPerSec;
	m_pLazerInsideTransformCom->Turn_CW(m_pLazerInsideTransformCom->Get_MatrixState(CTransform::STATE_LOOK),
		XMConvertToRadians(m_tLazerInsideMeshDesc.OnceStartRot));


	m_pLazerOutsideTransformCom->Set_Matrix(m_fAttachedMatrix.XMatrix());
	m_pLazerOutsideTransformCom->Move_Forward(3.f);
	m_pLazerOutsideTransformCom->Scaled_All(_float3(12.f, 12.f, 10.6f));
	m_tLazerOutsideMeshDesc.OnceStartRot += (_float)fDeltaTime * m_tLazerOutsideMeshDesc.RotationSpeedPerSec;
	m_pLazerOutsideTransformCom->Turn_CW(m_pLazerOutsideTransformCom->Get_MatrixState(CTransform::STATE_LOOK),
		XMConvertToRadians(m_tLazerOutsideMeshDesc.OnceStartRot));

	m_pExplosionTransformCom->Set_Matrix(m_fAttachedMatrix.XMatrix());
	_float3 vLazerStartPosition = m_pExplosionTransformCom->Get_MatrixState(CTransform::STATE_POS);
	_float3 vLazerEndPosition = m_pExplosionTransformCom->Get_MatrixState(CTransform::STATE_POS) + 
		m_pExplosionTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK);

	m_pExplosionTransformCom->Set_MatrixState(CTransform::STATE_POS,  m_pExplosionTransformCom->Get_MatrixState(CTransform::STATE_POS) +
	m_pExplosionTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 
		(fabs(vLazerStartPosition.y - 2.13f) / fabs(vLazerStartPosition.y - vLazerEndPosition.y)));

	// 1: x = fabs(vLazerStartPosition.y - vLazerEndPosition.y) : fabs(vLazerStartPosition.y - 2.13f)

	m_pExplosionTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(m_pExplosionTransformCom->Get_MatrixState(CTransform::STATE_POS), 2.2f));

	m_pExplosionTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(0));

	if (m_fCurTime_Duration > 0.3f && !m_iAdjustCount)
	{
		m_iAdjustCount++;

		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_ArrDesc[0]);
		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_ArrDesc[1]);
		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_ArrDesc[2]);


	}

	return _int();
}

_int CSnake_Poison_Raser::Render()
{
	if (__super::Render() < 0)		return -1;

	if (m_fCurTime_Duration > m_fMaxTime_Duration) return 0;

	NULL_CHECK_RETURN(m_pLazerInsideModel, E_FAIL);
	NULL_CHECK_RETURN(m_pLazerOutsideModel, E_FAIL);

	
	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_fCurTime_Duration, sizeof(_float)));


	{
		FAILED_CHECK(m_pLazerInsideTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

		FAILED_CHECK(Setup_MeshDesc(m_tLazerInsideMeshDesc));

		_uint NumMaterial = m_pLazerInsideModel->Get_NumMaterial();
		for (_uint i = 0; i < NumMaterial; i++)
		{
			FAILED_CHECK(m_pLazerInsideModel->Bind_OnShader(m_pShaderCom, i, 6, MODLETEXTYPE(6)));
			FAILED_CHECK(m_pLazerInsideModel->Render(m_pShaderCom, 19, i));
		}

	}
	{
		FAILED_CHECK(m_pLazerOutsideTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
		FAILED_CHECK(Setup_MeshDesc(m_tLazerOutsideMeshDesc));

		_uint NumMaterial = m_pLazerOutsideModel->Get_NumMaterial();
		for (_uint i = 0; i < NumMaterial; i++)
		{
			FAILED_CHECK(m_pLazerOutsideModel->Bind_OnShader(m_pShaderCom, i, 6, MODLETEXTYPE(6)));
			FAILED_CHECK(m_pLazerOutsideModel->Render(m_pShaderCom, 19, i));
		}

	}

	return _int();
}

_int CSnake_Poison_Raser::LateRender()
{
	return _int();
}

void CSnake_Poison_Raser::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

void CSnake_Poison_Raser::Start_BeamEffect()
{
	m_fCurTime_Duration = 0;
	m_iAdjustCount = 0;
}

void CSnake_Poison_Raser::Update_AttachMatrix()
{
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_RaserDesc.m_eAttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed();
}

HRESULT CSnake_Poison_Raser::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(90);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

		

	Safe_Release(m_pLazerInsideModel);
	m_pLazerInsideModel = (CModel*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Mesh_JYLaser));
	NULL_CHECK_RETURN(m_pLazerInsideModel, E_FAIL);

	Safe_Release(m_pLazerOutsideModel);
	m_pLazerOutsideModel = (CModel*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Mesh_Spring));
	NULL_CHECK_RETURN(m_pLazerOutsideModel, E_FAIL);



	Safe_Release(m_pLazerInsideTransformCom);
	m_pLazerInsideTransformCom = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pLazerInsideTransformCom, E_FAIL);
	m_pLazerInsideTransformCom->Set_TurnSpeed(1);
	m_pLazerInsideTransformCom->Set_MoveSpeed(1);

	Safe_Release(m_pLazerOutsideTransformCom);
	m_pLazerOutsideTransformCom = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pLazerOutsideTransformCom, E_FAIL);
	m_pLazerOutsideTransformCom->Set_TurnSpeed(1);
	m_pLazerOutsideTransformCom->Set_MoveSpeed(1);

	Safe_Release(m_pExplosionTransformCom);
	m_pExplosionTransformCom = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pExplosionTransformCom, E_FAIL);
	m_pExplosionTransformCom->Set_TurnSpeed(1);

	
		
	
	return S_OK;
}

HRESULT CSnake_Poison_Raser::Ready_MeshDesc()
{
	m_tLazerOutsideMeshDesc.fMaxTime_Duration = 
		m_tLazerInsideMeshDesc.fMaxTime_Duration =
		m_fMaxTime_Duration = 5.25f;


	m_tLazerInsideMeshDesc.fAppearTime =
		m_tLazerOutsideMeshDesc.fAppearTime = 0.5f;

	{

		m_tLazerInsideMeshDesc.noisingdir = _float2(0, -1);

		m_tLazerInsideMeshDesc.NoiseTextureIndex = 388;
		m_tLazerInsideMeshDesc.MaskTextureIndex = 3;
		m_tLazerInsideMeshDesc.iDiffuseTextureIndex = 394;
		m_tLazerInsideMeshDesc.m_iPassIndex = 19;
		m_tLazerInsideMeshDesc.vEmissive = _float4(1, 0.5f, 1.f, 1);

		m_tLazerInsideMeshDesc.vLimLight = _float4(0.734375f, 0.13671875f, 0.98046875f, 1.f);
		//m_tLazerInsideMeshDesc.vLimLight = _float4(0.98046875f, 0.93359375f, 0.19140625f, 1.f);
		m_tLazerInsideMeshDesc.vColor = _float3(0.2f, 0.2f, 0.2f);


		m_tLazerInsideMeshDesc.RotAxis = FollowingDir_Look;
		m_tLazerInsideMeshDesc.OnceStartRot = 0.f;
		m_tLazerInsideMeshDesc.RotationSpeedPerSec = 1080.f;

		m_tLazerInsideMeshDesc.vSize = _float3(2.88f, 2.88f, 0.85f);

		m_tLazerInsideMeshDesc.MoveDir = FollowingDir_Look;
		m_tLazerInsideMeshDesc.MoveSpeed = 0;

		m_tLazerInsideMeshDesc.fAlphaTestValue = 0.f;
	}
	{


		m_tLazerOutsideMeshDesc.noisingdir = _float2(1, 0);

		m_tLazerOutsideMeshDesc.NoiseTextureIndex = 247;
		m_tLazerOutsideMeshDesc.MaskTextureIndex = 55;
		m_tLazerOutsideMeshDesc.iDiffuseTextureIndex = 404;
		m_tLazerOutsideMeshDesc.m_iPassIndex = 19;
		m_tLazerOutsideMeshDesc.vEmissive = _float4(1, 1.f, 0.001f, 0);
		m_tLazerOutsideMeshDesc.vLimLight = _float4(0.19140625f, 0.93359375f, 0.19140625f, 1.f);
		m_tLazerOutsideMeshDesc.vColor = _float3(0.2f, 0.8f, 0.2f);

		m_tLazerOutsideMeshDesc.RotAxis = FollowingDir_Look;
		m_tLazerOutsideMeshDesc.OnceStartRot = 0.f;
		m_tLazerOutsideMeshDesc.RotationSpeedPerSec = -1080.f;

		m_tLazerOutsideMeshDesc.vSize = _float3(12.f, 12.f, 10.6f);

		m_tLazerOutsideMeshDesc.MoveDir = FollowingDir_Look;
		m_tLazerOutsideMeshDesc.MoveSpeed = 0;
		m_tLazerOutsideMeshDesc.fAlphaTestValue = 0.125f;
	}


	m_ArrDesc[0] = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"Boss_Snake_1");
	m_ArrDesc[0].FollowingTarget = m_pExplosionTransformCom;
	m_ArrDesc[0].eInstanceCount = Prototype_VIBuffer_Point_Instance_256;
	m_ArrDesc[0].iFollowingDir = FollowingDir_Up;

	m_ArrDesc[1] = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"Boss_Snake_2");
	m_ArrDesc[1].FollowingTarget = m_pExplosionTransformCom;
	m_ArrDesc[1].eInstanceCount = Prototype_VIBuffer_Point_Instance_256;
	m_ArrDesc[1].iFollowingDir = FollowingDir_Up;


	m_ArrDesc[0].ParticleStartRandomPosMin = m_ArrDesc[1].ParticleStartRandomPosMin
		= _float3(-8.f, 0.9f, -8.f);
	m_ArrDesc[0].ParticleStartRandomPosMax = m_ArrDesc[1].ParticleStartRandomPosMax
		= _float3(8.f, 1.f, 8.f);

	m_ArrDesc[0].ColorChageFrequency = m_ArrDesc[1].ColorChageFrequency = 3;
	m_ArrDesc[0].TargetColor = m_ArrDesc[1].TargetColor = _float4(0.19140625f, 0.93359375f, 0.19140625f, 1.f);
	m_ArrDesc[0].TargetColor2 = m_ArrDesc[1].TargetColor2 = _float4(0.734375f, 0.13671875f, 0.98046875f, 1.f);
	m_ArrDesc[0].ParticleSize = m_ArrDesc[1].ParticleSize = _float3(7.5f);
	m_ArrDesc[0].EachParticleLifeTime = m_ArrDesc[1].EachParticleLifeTime = 0.5f;
	m_ArrDesc[0].vEmissive_SBB = m_ArrDesc[1].vEmissive_SBB = _float3(1.f, 0.2f, 1.f);
	m_ArrDesc[0].eInstanceCount = m_ArrDesc[1].eInstanceCount = Prototype_VIBuffer_Point_Instance_256;


	m_ArrDesc[2] = GetSingle(CUtilityMgr)->Get_TextureParticleDesc(L"JY_TextureEft_20");
	m_ArrDesc[2].FollowingTarget = m_pExplosionTransformCom;
	m_ArrDesc[2].iFollowingDir = FollowingDir_Look;
	m_ArrDesc[2].EachParticleLifeTime = 5.25f;
	m_ArrDesc[2].ParticleSize = _float3(12);
	m_ArrDesc[2].ParticleStartRandomPosMin = _float3(-3.f, 0.0f, -3.f);
	m_ArrDesc[2].ParticleStartRandomPosMax = _float3(3.f, 0.5f, 3.f);
	
	return S_OK;
}

HRESULT CSnake_Poison_Raser::Setup_MeshDesc(NONINSTNESHEFTDESC & tMeshDesc)
{

	Set_LimLight_N_Emissive(tMeshDesc.vLimLight, tMeshDesc.vEmissive);


	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &tMeshDesc.vColor, sizeof(_float4)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAppearTimer", &tMeshDesc.fAppearTime, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fMaxTime", &tMeshDesc.fMaxTime_Duration, sizeof(_float)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &tMeshDesc.noisingdir, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fDistortionNoisingPushPower", &tMeshDesc.fDistortionNoisingPushPower, sizeof(_float)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAlphaTestValue", &tMeshDesc.fAlphaTestValue, sizeof(_float)));

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", tMeshDesc.NoiseTextureIndex));
	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", tMeshDesc.MaskTextureIndex));
	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_DiffuseTexture", tMeshDesc.iDiffuseTextureIndex));

	return S_OK;
}

CSnake_Poison_Raser * CSnake_Poison_Raser::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CSnake_Poison_Raser*	pInstance = NEW CSnake_Poison_Raser(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CSnake_Poison_Raser");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSnake_Poison_Raser::Clone(void * pArg)
{
	CSnake_Poison_Raser*	pInstance = NEW CSnake_Poison_Raser(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CSnake_Poison_Raser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSnake_Poison_Raser::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pLazerInsideModel );
	Safe_Release(m_pLazerOutsideModel);

	
		

	Safe_Release(m_pLazerInsideTransformCom);
	Safe_Release(m_pLazerOutsideTransformCom);
	Safe_Release(m_pExplosionTransformCom);

	
		
		
}
