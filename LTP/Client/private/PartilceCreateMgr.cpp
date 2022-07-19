#include "stdafx.h"
#include "..\public\PartilceCreateMgr.h"
#include "..\public\NonInstanceMeshEffect_TT.h"


IMPLEMENT_SINGLETON(CPartilceCreateMgr);

CPartilceCreateMgr::CPartilceCreateMgr()
{}

HRESULT CPartilceCreateMgr::Initialize_ParticleMgr()
{
	FAILED_CHECK(Ready_MeshEffect());
	FAILED_CHECK(Ready_TextureEffect());

	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_Texture_Effect(E_TEXTURE_EFFECTJ type, CTransform * parentTransform)
{
	INSTPARTICLEDESC texdesc = Get_TypeDesc_TextureInstance(type);
	texdesc.FollowingTarget = parentTransform;

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	FAILED_CHECK(GetSingle(CUtilityMgr)->Create_TextureInstance(SceneNum,texdesc));

	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_Texture_Effect_World(E_TEXTURE_EFFECTJ type, _float3 WorldPos)
{
	INSTPARTICLEDESC texdesc = Get_TypeDesc_TextureInstance(type);
	texdesc.vFixedPosition = WorldPos;

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	FAILED_CHECK(GetSingle(CUtilityMgr)->Create_TextureInstance(SceneNum, texdesc));

	return S_OK;
}


HRESULT CPartilceCreateMgr::Create_Texture_Effect_Desc(INSTPARTICLEDESC desc, _uint scene)
{

	FAILED_CHECK(GetSingle(CUtilityMgr)->Create_TextureInstance(scene, desc));

	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffect(E_MESH_EFFECTJ type, CTransform * parentTransform,_float3 Offset)
{

	 NONINSTNESHEFTDESC meshDesc = Get_TypeDesc_NonInstacne(type);
	meshDesc.vPosition = Offset;
	meshDesc.vLookDir = parentTransform->Get_MatrixState(CTransform::STATE_LOOK);

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	CGameObject* obj  = g_pGameInstance->Add_GameObject_GetObject(SceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect_TT), &meshDesc);
	NULL_CHECK_BREAK(obj);
	CNonInstanceMeshEffect_TT * meshobj = static_cast<CNonInstanceMeshEffect_TT*>(obj);
	NULL_CHECK_BREAK(meshobj);
	meshobj->Set_ParentTransform(parentTransform);


	//mListEffects.push_back(meshobj);


	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffect_World(E_MESH_EFFECTJ type, _float3 Postion, _float3 LookDir)
{
	 NONINSTNESHEFTDESC meshDesc = Get_TypeDesc_NonInstacne(type);
	meshDesc.vPosition = Postion;
	meshDesc.vLookDir = LookDir;


	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	g_pGameInstance->Add_GameObject_To_Layer(SceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect_TT), &meshDesc);


	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffectDesc(NONINSTNESHEFTDESC desc, MESHADDDATA desc2,CTransform * parentTransform)
{

	_uint SceneNum = GetSingle(CGameInstance)->Get_NowSceneNum();
	CGameObject* obj = g_pGameInstance->Add_GameObject_GetObject(SceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect_TT), &desc);
	NULL_CHECK_BREAK(obj);
	CNonInstanceMeshEffect_TT * meshobj = static_cast<CNonInstanceMeshEffect_TT*>(obj);
	NULL_CHECK_BREAK(meshobj);
	meshobj->Set_ParentTransform(parentTransform);
	meshobj->Set_AddDesc(desc2);

	return S_OK;
}

HRESULT CPartilceCreateMgr::Create_MeshEffectDesc_Hard(E_MESH_EFFECTJ type , CTransform* Transfom)
{
	if (type <= MESHEFFECT_PRE_END)
		return E_FAIL;

	MESHADDDATA			AddDesc;


	// ÇÏµå ÄÚµùÀ¸·Î Á¦ÀÛ
	if (type == MESHEFFECT_ARROW_HEAD)
	{
		NONINSTNESHEFTDESC MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_IMPECTFX_02);

		MeshDesc.fMaxTime_Duration = 3.0f;

		MeshDesc.iDiffuseTextureIndex = 365;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 325;

		MeshDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 1.0f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vEmissive = _float4(0.3f, 0.3f, 1.0f, 1.0f);
		MeshDesc.vLimLight = _float4(0.0f, 0.4f, 1, 1);


		MeshDesc.vLookDir = _float3(0, 1, 0);
		MeshDesc.vSize = _float3(0.3f);

		MeshDesc.RotAxis= FollowingDir_Right;
		MeshDesc.RotationSpeedPerSec = 10.0f;

		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
		MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

	//	MeshDesc.vLookDir = m_pTransformCom->Get_MatrixScale(CTransform::STATE_RIGHT);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	if (type == MESHEFFECT_ARROW_BOW1)
	{
		NONINSTNESHEFTDESC MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_BOW1);

		MeshDesc.fMaxTime_Duration = 2.0f;

		MeshDesc.iDiffuseTextureIndex = 335;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 325;

		MeshDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.5f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vEmissive = _float4(0.3f, 0.3f, 0.5f, 0.8f);
		MeshDesc.vLimLight = _float4(0.1f, 0.76f, 0.77, 0.2f);


		MeshDesc.vPosition = _float3(-0.0f, 0.0f, -0.0f);
		MeshDesc.vSize = _float3(1.3f);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;

	//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
	//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(0, 0, 1);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå


		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);


	}
	if (type == MESHEFFECT_ARROW_BOW_UP)
	{

		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_BOW1);
		MESHADDDATA			AddDesc;

		MeshDesc.fMaxTime_Duration = 3.0f;

		MeshDesc.iDiffuseTextureIndex = 330;
		MeshDesc.MaskTextureIndex = 8;
		MeshDesc.NoiseTextureIndex = 328;

		MeshDesc.noisingdir = _float2(0.0f, 1.0f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 1.5f;
		MeshDesc.fDistortionNoisingPushPower = 3.0f;

		MeshDesc.vLimLight = _float4(0.0f, 0.40f, 0.89f, 1.0f);
		MeshDesc.vEmissive = _float4(0.6f, 0.5f, 0.0f,0.0f);


		MeshDesc.vPosition = _float3(-0.8f, 0.0f, 0.0f);
		MeshDesc.vSize = _float3(1.5f);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(0, 0, 1);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Look;
		AddDesc.vAddDirectAngle = _float3(0,270,0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc,Transfom);
	}


	if (type == MESHEFFECT_ARROW_BOW_SHIFT_FLOOR)
	{

		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_CIRCLE);
		MESHADDDATA			AddDesc;

		MeshDesc.fMaxTime_Duration = 3.0f;

		MeshDesc.iDiffuseTextureIndex = 236;
		MeshDesc.MaskTextureIndex = 31;
		MeshDesc.NoiseTextureIndex = 325;

		MeshDesc.noisingdir = _float2(0.5f, 0.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.5f;
		MeshDesc.fDistortionNoisingPushPower = 5.0f;

		MeshDesc.vLimLight = _float4(0.03f, 0.06f, 0.82f, 1.f);
		MeshDesc.vEmissive = _float4(0.03f, 0.06f, 0.82f, 1.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(15.0f);

		MeshDesc.RotAxis = FollowingDir_Look;
		MeshDesc.RotationSpeedPerSec = 0;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


	//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_ARROW_BOW_SHIFT_ICE)
	{

		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_ICE);
		MESHADDDATA			AddDesc;

		MeshDesc.fMaxTime_Duration = 1.5f;

		MeshDesc.iDiffuseTextureIndex = 363;
		MeshDesc.MaskTextureIndex = 0;
		MeshDesc.NoiseTextureIndex = 285;

		MeshDesc.noisingdir = _float2(0.5f, 1.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 1.0f;
		MeshDesc.fDistortionNoisingPushPower = 10.0f;

		MeshDesc.vLimLight = _float4(0.11f, 0.26f, 0.64f, 0.3f);
		MeshDesc.vEmissive = _float4(1.0f, 0.f, 0.f,0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(3.0f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 30.0f;

		//	_float3 Pos = Transfom->Get_MatrixState(CTransform::STATE_POS);s
		//	_float3 dir =  Transfom->Get_MatrixState(CTransform::STATE_UP);

		MeshDesc.vLookDir = _float3(1, 0, 0);


		//	MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.AccRotSpeed = -28.0f;
		
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}

	if (type == MESHEFFECT_ARROW_BOW_R)
	{
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_BOW2);
		MESHADDDATA			AddDesc;

		MeshDesc.fMaxTime_Duration = 2.0f;

		MeshDesc.iDiffuseTextureIndex = 364;
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.NoiseTextureIndex = 366;

		MeshDesc.noisingdir = _float2(0.5f, 1.5f).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.f;
		MeshDesc.fDistortionNoisingPushPower = 1.0f;

		MeshDesc.vLimLight = _float4(0.98f,0.56f,0.56f, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.f, 0.f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition= _float3(-0.0f, 0, -0.0f);
		MeshDesc.vSize = _float3(0.8f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 0;

		MeshDesc.vLookDir = _float3(1, 0, 0);


		MeshDesc.m_iPassIndex = 16; // ¿Ö°î
		//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
		MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Look;
		// AddDesc.AccRotSpeed = -28.0f;
		AddDesc.vAddDirectAngle = _float3(0,10.0f,0);
		
		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	static bool bWingRand = true;
	bWingRand = !bWingRand;

	if (type == MESHEFFECT_ARROW_BOW_R_JUMP_WING1)
	{
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_WING);
		MESHADDDATA			AddDesc;

		MeshDesc.fMaxTime_Duration = 1.0f;

		MeshDesc.iDiffuseTextureIndex = 364;
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.NoiseTextureIndex = 366;

		MeshDesc.noisingdir = _float2(-1,0).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.5f;
		MeshDesc.fDistortionNoisingPushPower = 3.0f;

		MeshDesc.vLimLight = _float4(0.98f, 0.56f, 0.56f, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.f, 0.f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(0.0f, 0.0f, -0.3f);
		MeshDesc.vSize = _float3(2.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec = 0;

		MeshDesc.vLookDir = _float3(1, 0, 0);


		MeshDesc.m_iPassIndex = bWingRand ? 17 : 19; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.AccRotSpeed = 0;
		AddDesc.vAddDirectAngle = _float3(0, 90, 0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);
	}
	if (type == MESHEFFECT_ARROW_BOW_R_JUMP_WING2)
	{
		NONINSTNESHEFTDESC	MeshDesc = GetSingle(CPartilceCreateMgr)->Get_TypeDesc_NonInstacne(CPartilceCreateMgr::MESHEFFECT_PRE_WING);
		MESHADDDATA			AddDesc;

		MeshDesc.fMaxTime_Duration = 1.0f;

		MeshDesc.iDiffuseTextureIndex = 364;
		MeshDesc.MaskTextureIndex = 57;
		MeshDesc.NoiseTextureIndex = 366;

		MeshDesc.noisingdir = _float2(1, 0).Get_Nomalize();
		MeshDesc.vColor = _float4(1, 1, 1, 1);
		MeshDesc.fAppearTime = 0.8f;
		MeshDesc.fDistortionNoisingPushPower = 3.0f;

		MeshDesc.vLimLight = _float4(0.98f, 0.56f, 0.56f, 1.0f);
		MeshDesc.vEmissive = _float4(1.0f, 0.f, 0.f, 0.f);


		//	MeshDesc.vRotAxis = _float3(1, 0, 0);
		MeshDesc.vPosition = _float3(0.0f, 0.0f, -0.3f);
		MeshDesc.vSize = _float3(2.5f);

		MeshDesc.RotAxis = FollowingDir_Up;
		MeshDesc.RotationSpeedPerSec =0;

		MeshDesc.vLookDir = _float3(1, 0, 0);


		MeshDesc.m_iPassIndex = bWingRand ? 17 : 19; // ¿Ö°î
	//	MeshDesc.m_iPassIndex = 17; // ¿Ö°î µîÀå
	//	MeshDesc.m_iPassIndex = 18; // DisCard
	//	MeshDesc.m_iPassIndex = 19; // ³ëÀÌÁî µîÀå

		AddDesc.LookRotAxis = FollowingDir_Right;
		AddDesc.AccRotSpeed = 0;
		AddDesc.vAddDirectAngle = _float3(0, -90, 0);

		GetSingle(CPartilceCreateMgr)->Create_MeshEffectDesc(MeshDesc, AddDesc, Transfom);

	}

	


	return S_OK;
}




HRESULT CPartilceCreateMgr::Update_MeshEffect(_double timer)
{

	return S_OK;
}

HRESULT CPartilceCreateMgr::Clear_MeshEffect()
{
	return S_OK;
}

HRESULT CPartilceCreateMgr::Ready_MeshEffect()
{
	// #MESHPARTILCEINIT
	mVecMeshEffectDesc.resize((int)MESHEFFECT_END);

	NONINSTNESHEFTDESC tNIMEDesc;
	tNIMEDesc.eMeshType = Prototype_Mesh_ConeMesh;
	tNIMEDesc.fMaxTime_Duration = 10.f;
	mVecMeshEffectDesc[MESHEFFECT_PRE_CONMESH] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_CIRCLE;
	mVecMeshEffectDesc[MESHEFFECT_PRE_CIRCLE] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_CIRCLE_DIST5;
	mVecMeshEffectDesc[MESHEFFECT_PRE_CIRCLE_DIST4] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_CIRCLE_DIST4;
	mVecMeshEffectDesc[MESHEFFECT_PRE_CIRCLE_DIST5] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_IMPECTFX_02;
	mVecMeshEffectDesc[MESHEFFECT_PRE_IMPECTFX_02] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_RING;
	mVecMeshEffectDesc[MESHEFFECT_PRE_RING] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_LOVE;
	mVecMeshEffectDesc[MESHEFFECT_PRE_LOVE] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_WING;
	mVecMeshEffectDesc[MESHEFFECT_PRE_WING] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_BOW1;
	mVecMeshEffectDesc[MESHEFFECT_PRE_BOW1] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_BOW2;
	mVecMeshEffectDesc[MESHEFFECT_PRE_BOW2] = tNIMEDesc;

	tNIMEDesc.eMeshType = Prototype_Mesh_ICE;
	mVecMeshEffectDesc[MESHEFFECT_PRE_ICE] = tNIMEDesc;


	return S_OK;
}

HRESULT CPartilceCreateMgr::Ready_TextureEffect()
{
	// #TEXTUREPARTILCEINIT
	mVecTextureEffectDesc.resize((int)TEXTURE_EFFECTJ_END);

	auto  pUtil = GetSingle(CUtilityMgr);
	
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Default] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Default"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_ArrowHit] = pUtil->Get_TextureParticleDesc(TEXT("Bow_ArrowHit"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Bow_ArrowTrail] = pUtil->Get_TextureParticleDesc(TEXT("Bow_ArrowTrail"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_ArrowHead] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Charze_ArrowHead"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Circle] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Charze_Circle"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Dash] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Charze_Dash"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Long] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Charze_Long"));
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Suck] = pUtil->Get_TextureParticleDesc(TEXT("Bow_Charze_Suck"));

	// DefaultSetting
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Default].TotalParticleTime = 99999.f;

	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Long].ParticleSize = _float3(0.08f, 0.3f, 0.08f);
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Long].ParticleSize2 = _float3(0.08f, 0.8f, 0.08f);
	mVecTextureEffectDesc[TEXTURE_EFFECTJ_Bow_Charze_Long].iFollowingDir = FollowingDir_Look;


	

	return S_OK;
}


void CPartilceCreateMgr::Free()
{
	

}
